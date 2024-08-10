// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "TextMaker.hpp"


 std::vector<SingleText> demoText = {
	{4, {"1 - Rep. H,  2 - Rep. V (Clamp, Rep, Mir)", "3 - Mip Map (OFF, ON near, ON linear)",
		 "4 - Min Filt, 5 - Mag Filt (Near, Linear)", "6 - Anistropic (ON, OFF)"}, 0, 0}
};

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 selector;
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

class Example07;
void GameLogic(Example07 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World);

// MAIN ! 
class Example07 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	VertexDescriptor VD;
	Pipeline P1;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model<Vertex> M1;
	DescriptorSet DS1, DS2;
	Texture T1, T2;

	TextMaker txt;
	
	// Other application parameters
	int currScene = 0;
	float Ar;
	glm::mat4 ViewPrj;
	glm::vec3 Pos = glm::vec3(0,1,0);
	glm::vec3 cameraPos;
	float Yaw = glm::radians(30.0f);
	float Pitch = glm::radians(22.5f);
	float Roll = glm::radians(0.0f);

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Example 07";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.015f, 0.03f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 7;
		texturesInPool = 4;
		setsInPool = 4;
		
		Ar = 4.0f / 3.0f;
	}
	
	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		std::cout << "Window resized to: " << w << " x " << h << "\n";
		Ar = (float)w / (float)h;
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSL1.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				});

		// Vertex descriptors
		VD.init(this, {
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
				         sizeof(glm::vec2), UV}
				});

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, &VD, "shaders/BlinnVert.spv", "shaders/BlinnFrag.spv", {&DSL1});
		P1.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);

		// Models, textures and Descriptors (values assigned to the uniforms)
		createPlaneMesh(M1.vertices, M1.indices);
		M1.initMesh(this, &VD);

		T1.init(this, "textures/Airport.png", VK_FORMAT_R8G8B8A8_SRGB, false);
		T1.createTextureSampler(
							 VK_FILTER_NEAREST,
							 VK_FILTER_NEAREST,
							 VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
							 VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
							 VK_SAMPLER_MIPMAP_MODE_NEAREST,
							 VK_FALSE, 1, 0
		);

		T2.init(this, "textures/Airport.png");
		
		txt.init(this, &demoText);
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		DS1.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{2, TEXTURE, 0, &T1}
				});
		DS2.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{2, TEXTURE, 0, &T2}
				});

		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		DS1.cleanup();
		DS2.cleanup();
		
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		T2.cleanup();

		M1.cleanup();

		DSL1.cleanup();
		
		P1.destroy();		
		
		txt.localCleanup();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	
	int curTexture = 0;
	
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		P1.bind(commandBuffer);
		M1.bind(commandBuffer);
		
		if(curTexture == 0) {
			DS1.bind(commandBuffer, P1, currentImage);
		} else {
			DS2.bind(commandBuffer, P1, currentImage);
		}
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);

		txt.populateCommandBuffer(commandBuffer, currentImage, currScene);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static bool debounce = false;
		static int curDebounce = 0;
//std::cout << xpos << " " << ypos << " " << m_dx << " " << m_dy << "\n";


		static bool showNormal = false;
		static bool showUV = false;
		

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		static VkFilter magFilter = VK_FILTER_NEAREST;
		static VkFilter minFilter = VK_FILTER_NEAREST;
		static VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		static VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		static VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		static VkBool32 anisotropyEnable = VK_FALSE;
		static float maxAnisotropy = 1;
		static float maxLod = 0;
		static int mmm = 0;

		bool update = false;

		if(glfwGetKey(window, GLFW_KEY_1)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_1;
				
				if(addressModeU ==  VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE) {addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				std::cout << "U: Repeat\n";}
				else if(addressModeU ==  VK_SAMPLER_ADDRESS_MODE_REPEAT) {addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
				std::cout << "U: Mirror\n";}
				else {addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				std::cout << "U: Clamp\n";}
				update = true;
			}
		} else {
			if((curDebounce == GLFW_KEY_1) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_2)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_2;
				
				if(addressModeV ==  VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE) {addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				std::cout << "V: Repeat\n";}
				else if(addressModeV ==  VK_SAMPLER_ADDRESS_MODE_REPEAT) {addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
				std::cout << "V: Mirror\n";}
				else {addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				std::cout << "V: Clamp\n";}
				update = true;
			}
		} else {
			if((curDebounce == GLFW_KEY_2) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_3)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_3;

				if(mmm == 0) {mmm = 1; mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST; maxLod = -1;
				std::cout << "MipMap Nearest\n";}
				else if(mmm == 1) {mmm = 2; mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; maxLod = -1;
				std::cout << "MipMap Linear\n";}
				else {mmm = 0; mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST; maxLod = 0;
				std::cout << "No MipMap\n";}
				update = true;
			}
		} else {
			if((curDebounce == GLFW_KEY_3) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_4)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_4;

				if(minFilter == VK_FILTER_NEAREST) {minFilter = VK_FILTER_LINEAR; std::cout << "Min: Linear\n";}
				else {minFilter = VK_FILTER_NEAREST; std::cout << "Min: Nearest\n";}
				update = true;
			}
		} else {
			if((curDebounce == GLFW_KEY_4) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_5)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_5;

				if(magFilter == VK_FILTER_NEAREST) {magFilter = VK_FILTER_LINEAR; std::cout << "Mag: Linear\n";}
				else {magFilter = VK_FILTER_NEAREST; std::cout << "Mag: Nearest\n";}
				update = true;
			}
		} else {
			if((curDebounce == GLFW_KEY_5) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_6)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_6;

				if(anisotropyEnable == VK_FALSE) {anisotropyEnable = VK_TRUE; maxAnisotropy = 16; std::cout << "Anisotropy ON\n";}
				else {anisotropyEnable = VK_FALSE; maxAnisotropy = 1; std::cout << "Anisotropy OFF\n";}
				update = true;
			}
		} else {
			if((curDebounce == GLFW_KEY_6) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(update) {
			curTexture = 1 - curTexture;
			Texture *T;
			if(curTexture == 1) {
				T = &T2;
			} else {
				T = &T1;
			}
			T->cleanup();
			T->init(this, "textures/Airport.png", VK_FORMAT_R8G8B8A8_SRGB, false);
			T->createTextureSampler(
							  magFilter,
							  minFilter,
							  addressModeU,
							  addressModeV,
							  mipmapMode,
							  anisotropyEnable,
							  maxAnisotropy,
							  maxLod
			);				
			RebuildPipeline();
		}

			
		GameLogic();
		
//printMat4("ViewPrj", ViewPrj);
//printMat4("WM", WM);
		
		UniformBufferObject ubo{};								
		// Here is where you actually update your uniforms

		// updates global uniforms
		ubo.mMat = glm::mat4(1);
		ubo.mvpMat = ViewPrj;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		
		float dang = Pitch + glm::radians(15.0f);

		GlobalUniformBufferObject gubo{};

		ubo.mMat = glm::scale(glm::mat4(1), glm::vec3(3));
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));

		gubo.selector = glm::vec3(showNormal || showUV ? 0 : 1, showNormal ? 1 : 0, showUV ? 1 : 0);
		gubo.lightDir = glm::normalize(glm::vec3(1, 2, 3));
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = cameraPos;

		DS1.map(currentImage, &ubo, sizeof(ubo), 0);
		DS1.map(currentImage, &gubo, sizeof(gubo), 1);
		DS2.map(currentImage, &ubo, sizeof(ubo), 0);
		DS2.map(currentImage, &gubo, sizeof(gubo), 1);
	}
	
	void GameLogic() {
		// Parameters
		// Camera FOV-y, Near Plane and Far Plane
		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 1000.f;
		// Camera target height and distance
		const float camHeight = 1.25;
		const float camDist = 1.5;
		// Camera Pitch limits
		const float minPitch = glm::radians(-60.0f);
		const float maxPitch = glm::radians(60.0f);
		// Rotation and motion speed
		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 2.0f;

		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);

		// Game Logic implementation
		// Current Player Position - statc variable make sure its value remain unchanged in subsequent calls to the procedure

		// To be done in the assignment
		ViewPrj = glm::mat4(1);
		glm::mat4 World = glm::mat4(1);
		
		// World
		// Position
		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1);
		Pos = Pos + MOVE_SPEED * m.x * ux * deltaT;
		Pos = Pos + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
		Pos.y = Pos.y < 0.0f ? 0.0f : Pos.y;
		Pos = Pos + MOVE_SPEED * m.z * uz * deltaT;
		// Rotation
		Yaw = Yaw - ROT_SPEED * deltaT * r.y;
		Pitch = Pitch + ROT_SPEED * deltaT * r.x;
		Pitch  =  Pitch < minPitch ? minPitch :
				   (Pitch > maxPitch ? maxPitch : Pitch);
		Roll   = Roll   - ROT_SPEED * deltaT * r.z;
		Roll   = Roll < glm::radians(-175.0f) ? glm::radians(-175.0f) :
				   (Roll > glm::radians( 175.0f) ? glm::radians( 175.0f) : Roll);

//std::cout << Pos.x << ", " << Pos.y << ", " << Pos.z << ", " << Yaw << ", " << Pitch << ", " << Roll << "\n";

		// Final world matrix computaiton
		World = glm::translate(glm::mat4(1), Pos) * glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0,1,0));
		
		// Projection
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		// View
		// Target
		glm::vec3 target = Pos + glm::vec3(0.0f, camHeight, 0.0f);

		// Camera position, depending on Yaw parameter, but not character direction
		cameraPos = World * glm::vec4(0.0f, camHeight + camDist * sin(Pitch), camDist * cos(Pitch), 1.0);
		// Damping of camera
		glm::mat4 View = glm::rotate(glm::mat4(1.0f), -Roll, glm::vec3(0,0,1)) *
						 glm::lookAt(cameraPos, target, glm::vec3(0,1,0));

		ViewPrj = Prj * View;
	}
	
	void createPlaneMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
		vDef = {};
		vDef.push_back({{20,0,40}, {0,1,0}, {-1,-1}});	// vertex 0 - Position, Normal and uv
		vDef.push_back({{-10,0,40}, {0,1,0}, {2,-1}});	// vertex 1 - Position and Normal
		vDef.push_back({{20,0,-80}, {0,1,0}, {-1,2}});	// vertex 2 - Position and Normal
		vDef.push_back({{-10,0,-80}, {0,1,0}, {2,2}});// vertex 3 - Position and Normal

		// The procedures also fill the array vIdx with the indices of the vertices of the triangles
		vIdx = {};
		vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
		vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	}
};


// This is the main: probably you do not need to touch this!
int main() {
    Example07 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}