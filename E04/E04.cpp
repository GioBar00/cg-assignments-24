// This has been adapted from the Vulkan tutorial

// TO MOVE
#define JSON_DIAGNOSTICS 1
#include "modules/Starter.hpp"
#include "modules/TextMaker.hpp"


std::vector<SingleText> outText = {
	{2, {"Grid for Vertex", "Press SPACE to change"}, 0, 0},
	{2, {"Grid for Fragment", "Press SPACE to change"}, 0, 0},
	{2, {"Grid Combined", "Press SPACE to change"}, 0, 0},
	{2, {"Scene for Vertex", "Press SPACE to change","",""}, 0, 0},
	{2, {"Scene Combined", "Press SPACE to change","",""}, 0, 0},
	{2, {"Scene Reference", "Press SPACE to change","",""}, 0, 0},
};

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
	alignas(16) glm::vec4 color;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
	alignas(16) glm::vec4 eyeDir;
};



// The vertices data structures
// Example
struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
	glm::vec3 norm;
};

void MakeSquare(float size, std::vector<glm::vec3> &vertices, std::vector<uint32_t> &indices) {
	int i,j;
	vertices.resize(33*33);
	
	for(i = 0; i <= 32; i++) {
		for(j = 0; j <=32; j++) {
			int vs = i*33+j;
			vertices[vs] = glm::vec3(((float)i / 16.0f - 1.0f), 0.0f,((float)j / 16.0f - 1.0f));
		}
	}

	indices.resize(32*32*6);
	for(i = 0; i < 32; i++) {
		for(j = 0; j <32; j++) {
			int is = (i*32+j)*6;
			int vs = i*33+j;
			indices[is+0] = vs;
			indices[is+1] = vs+1;
			indices[is+2] = vs+34;
			indices[is+3] = vs;
			indices[is+5] = vs+33;
			indices[is+4] = vs+34;
		}
	}
}

// MAIN ! 
class E04 : public BaseProject {
	protected:
	
	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSL;

	// Vertex formats
	VertexDescriptor VD;

	// Pipelines [Shader couples]
	Pipeline P1, P2, P3, P4, PW1, PW2;
	Pipeline *PF[6], *PW[6];

	// Models
	Model M1, M2, MAx;
	// Descriptor sets
	DescriptorSet DS1, DS2, DSAx;
	// Textures
	Texture T1, T2;
	
	TextMaker txt;
	
	// Other application parameters
	int currScene = 0;
	int subpass = 0;
		
	glm::vec3 CamPos = glm::vec3(0.0, 1.5, 7.0);
	float CamAlpha = 0.0f;
	float CamBeta = 0.0f;
	float Ar;
	bool WireFrame = false;
	bool ShowAxis = false;

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "E04 - Shaders";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.85f, 1.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 28 * 2 + 2;
		texturesInPool = 28 + 1;
		setsInPool = 28 + 1;

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
		DSL.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
				});

		// Vertex descriptors
		VD.init(this, {
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
				         sizeof(glm::vec2), UV},
				  {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
				         sizeof(glm::vec3), NORMAL}
				});

		// Pipelines [Shader couples]
		P1.init(this, &VD, "shaders/MyVert.spv", "shaders/PhongFrag.spv", {&DSL});
		P1.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);
		P2.init(this, &VD, "shaders/GFFVert.spv", "shaders/MyFrag.spv", {&DSL});
		P2.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);
		P3.init(this, &VD, "shaders/MyVert.spv", "shaders/MyFrag.spv", {&DSL});
		P3.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);
		P4.init(this, &VD, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", {&DSL});
		P4.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_BACK_BIT, false);

		PW1.init(this, &VD, "shaders/MyVert.spv", "shaders/PhongFrag.spv", {&DSL});
		PW1.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_LINE,
 								    VK_CULL_MODE_NONE, false);
		PW2.init(this, &VD, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", {&DSL});
		PW2.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_LINE,
 								    VK_CULL_MODE_NONE, false);

		PF[0] = &P1; PF[1] = &P2; PF[2] = &P3;  PF[3] = &P1;  PF[4] = &P3;  PF[5] = &P4;
		PW[0] = &PW1; PW[1] = &P2; PW[2] = &P3; PW[3] = &PW1; PW[4] = &P3; PW[5] = &PW2;

		// Models, textures and Descriptors (values assigned to the uniforms)

		std::vector<glm::vec3> vertices_pos;

		MakeSquare(2.0, vertices_pos, M1.indices);
		
		int mainStride = VD.Bindings[0].stride;
		M1.vertices = std::vector<unsigned char>(vertices_pos.size()*sizeof(Vertex), 0);
		for(int i = 0; i < vertices_pos.size(); i++) {
			Vertex *V_vertex = (Vertex *)(&(M1.vertices[i * mainStride]));
			V_vertex->pos = vertices_pos[i];						
			V_vertex->UV = glm::vec2(vertices_pos[i].x, vertices_pos[i].z);						
			V_vertex->norm = glm::vec3(0.0f,1.0f,0.0f);		

		}
		M1.initMesh(this, &VD);
		M2.init(this, &VD, "models/City.obj", OBJ);
		MAx.init(this, &VD, "models/axis.obj", OBJ);
		
		// Create the textures
		T1.init(this,   "textures/Textures.png");
		T2.init(this,   "textures/TexturesCity.png");
		
		txt.init(this, &outText);

		// Init local variables
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();
		P2.create();
		P3.create();
		P4.create();
		PW1.create();
		PW2.create();

		// Here you define the data set
		DS1.init(this, &DSL, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T1},
				{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
		DS2.init(this, &DSL, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T2},
				{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
		DSAx.init(this, &DSL, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T1},
				{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		P1.cleanup();
		P2.cleanup();
		P3.cleanup();
		P4.cleanup();
		PW1.cleanup();
		PW2.cleanup();

		DS1.cleanup();
		DS2.cleanup();
		DSAx.cleanup();
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {	
		// Cleanup textures
		T1.cleanup();
		T2.cleanup();
		
		// Cleanup models
		M1.cleanup();
		M2.cleanup();
		MAx.cleanup();
		
		// Cleanup descriptor set layouts
		DSL.cleanup();
		
		// Destroies the pipelines
		P1.destroy();		
		P2.destroy();		
		P3.destroy();		
		P4.destroy();		
		PW1.destroy();		
		PW2.destroy();		
		txt.localCleanup();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		// binds the pipeline
		Pipeline *P;
		
		if(WireFrame) {
			P = PW[currScene];
		} else {
			P = PF[currScene];
		}
		P->bind(commandBuffer);
		
		if(currScene < 3) {
			M1.bind(commandBuffer);
			DS1.bind(commandBuffer, *P, 0, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);
		} else {
			M2.bind(commandBuffer);
			DS2.bind(commandBuffer, *P, 0, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
		}		

		if(ShowAxis) {
			P4.bind(commandBuffer);
			MAx.bind(commandBuffer);
			DSAx.bind(commandBuffer, P4, 0, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(MAx.indices.size()), 1, 0, 0, 0);
		}

		txt.populateCommandBuffer(commandBuffer, currentImage, currScene);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static bool debounce = false;
		static int curDebounce = 0;

		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);
		
		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 2.0f;
		
		CamAlpha = CamAlpha - ROT_SPEED * deltaT * r.y;
		CamBeta  = CamBeta  - ROT_SPEED * deltaT * r.x;
		CamBeta  =  CamBeta < glm::radians(-90.0f) ? glm::radians(-90.0f) :
				   (CamBeta > glm::radians( 90.0f) ? glm::radians( 90.0f) : CamBeta);

		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(0,0,1,1);
		CamPos = CamPos + MOVE_SPEED * m.x * ux * deltaT;
		CamPos = CamPos + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
		CamPos = CamPos + MOVE_SPEED * m.z * uz * deltaT;
		
		static float subpassTimer = 0.0;

		if(glfwGetKey(window, GLFW_KEY_SPACE)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_SPACE;
				currScene = (currScene+1) % outText.size();
				std::cout << "Scene : " << currScene << "\n";
				
				RebuildPipeline();
			}
		} else {
			if((curDebounce == GLFW_KEY_SPACE) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		// Standard procedure to quit when the ESC key is pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	
		if(glfwGetKey(window, GLFW_KEY_X)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_X;

				WireFrame = !WireFrame;
				RebuildPipeline();
			}
		} else {
			if((curDebounce == GLFW_KEY_X) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_Z)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_Z;

				ShowAxis = !ShowAxis;
				RebuildPipeline();
			}
		} else {
			if((curDebounce == GLFW_KEY_Z) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}


		// Here is where you actually update your uniforms
		glm::mat4 M = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 50.0f);
		M[1][1] *= -1;

		glm::mat4 Mv =  glm::rotate(glm::mat4(1.0), -CamBeta, glm::vec3(1,0,0)) *
						glm::rotate(glm::mat4(1.0), -CamAlpha, glm::vec3(0,1,0)) *
						glm::translate(glm::mat4(1.0), -CamPos);

		glm::mat4 ViewPrj =  M * Mv;
		UniformBufferObject ubo{};
		glm::mat4 baseTr = glm::mat4(1.0f);								

		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::vec3(cos(glm::radians(135.0f)), sin(glm::radians(135.0f)), 0.0f);
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = CamPos;

		if(currScene < 3) {
			ubo.mMat = baseTr;
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			if(WireFrame) {
				ubo.color = glm::vec4(0,0,0,1);
			} else {
				ubo.color = glm::vec4(1);
			}

			DS1.map(currentImage, &ubo, sizeof(ubo), 0);
			DS1.map(currentImage, &gubo, sizeof(gubo), 2);
		} else {
			ubo.mMat = baseTr;
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			if(WireFrame) {
				ubo.color = glm::vec4(0,0,0,1);
			} else {
				ubo.color = glm::vec4(1,1,1,0);
			}


			DS2.map(currentImage, &ubo, sizeof(ubo), 0);
			DS2.map(currentImage, &gubo, sizeof(gubo), 2);
		}

		ubo.mMat = baseTr * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f,1.0f,0.0f));
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		ubo.color = glm::vec4(1,1,1,0);
		DSAx.map(currentImage, &ubo, sizeof(ubo), 0);
		DSAx.map(currentImage, &gubo, sizeof(gubo), 2);
	}
};

// This is the main: probably you do not need to touch this!
int main() {
    E04 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
