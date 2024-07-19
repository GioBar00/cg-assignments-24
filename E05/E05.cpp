// This has been adapted from the Vulkan tutorial

#include "modules/Starter.hpp"
#include "modules/TextMaker.hpp"
#include "modules/Scene.hpp"


std::vector<SingleText> outText = {
	{2, {"BRDF", "Press SPACE to save the screenshots","",""}, 0, 0},
	{1, {"Saving Screenshots. Please wait.", "", "",""}, 0, 0}
};

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};



// The vertices data structures

struct VertexNormMap {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 texCoord;
};



// MAIN ! 
class E05 : public BaseProject {
	protected:
	
	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSLNM;

	// Vertex formats
	VertexDescriptor VDNM;

	// Pipelines [Shader couples]
	Pipeline PNM;

	// Scenes and texts
	Scene SC;
	std::vector<VertexDescriptorRef>  VDRs;
	std::vector<PipelineRef> PRs;
	
	Model M[2];
	Texture T[9];
	DescriptorSet DS[3];
	
    TextMaker txt;
	
	// Other application parameters
	int currScene = 0;
	int subpass = 0;
		
	glm::vec3 CamPos = glm::vec3(0.0, 1.5, 7.0);
	float CamAlpha = 0.0f;
	float CamBeta = 0.0f;
	float Ar;
	
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "E05 - BRDF and Ambient";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};
		
		Ar = (float)windowWidth / (float)windowHeight;
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
		DSLNM.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject)},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, sizeof(GlobalUniformBufferObject)},
					{3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1},
					{4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2}
				  });

		// Vertex descriptors
		VDNM.init(this, {
				  {0, sizeof(VertexNormMap), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexNormMap, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexNormMap, normal),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(VertexNormMap, tangent),
				         sizeof(glm::vec3), TANGENT},
				  {0, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexNormMap, texCoord),
				         sizeof(glm::vec2), UV}
				});

//		VDRs.resize(1);
//		VDRs[0].init("VDNM", &VDNM);

		// Pipelines [Shader couples]
		PNM.init(this, &VDNM, "shaders/NormMapVert.spv", "shaders/NormMapFrag.spv", {&DSLNM});

//		PRs.resize(1);
//		PRs[0].init("PNM", &PNM, &VDNM);

		// Descriptor pool sizes
		// WARNING!!!!!!!!
		// Must be set before initializing the text and the scene
		DPSZs.uniformBlocksInPool = 7;
		DPSZs.texturesInPool = 10;
		DPSZs.setsInPool = 4;

/*std::cout << "Loading the scene\n";
		if(SC.init(this, VDRs, PRs, "models/scene.json") != 0) {
			std::cout << "ERROR LOADING THE SCENE\n";
			exit(0);
		}*/
		
		M[0].init(this, &VDNM, "models/Sphere.gltf", GLTF);
		M[1].init(this, &VDNM, "models/Plane.gltf", GLTF);
		
		T[0].init(this, "textures/Wood_01_basecolor.png");
		T[1].init(this, "textures/Wood_01_normal.png", VK_FORMAT_R8G8B8A8_UNORM);
		T[2].init(this, "textures/Wood_01_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
		T[3].init(this, "textures/fullwhite.png");
		T[4].init(this, "textures/original.png", VK_FORMAT_R8G8B8A8_UNORM);
		T[5].init(this, "textures/met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
		T[6].init(this, "textures/Rocks_06_basecolor.png");
		T[7].init(this, "textures/Rocks_06_normal.png", VK_FORMAT_R8G8B8A8_UNORM);
		T[8].init(this, "textures/Rocks_06_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
		
std::cout << "Initializing text\n";
		txt.init(this, &outText);

		std::cout << "Initialization completed!\n";
		std::cout << "Uniform Blocks in the Pool  : " << DPSZs.uniformBlocksInPool << "\n";
		std::cout << "Textures in the Pool        : " << DPSZs.texturesInPool << "\n";
		std::cout << "Descriptor Sets in the Pool : " << DPSZs.setsInPool << "\n";
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		PNM.create();

		// Here you define the data set
//		SC.pipelinesAndDescriptorSetsInit();
		Texture *T1[] = {&T[0],&T[1],&T[2]};
		Texture *T2[] = {&T[3],&T[4],&T[5]};
		Texture *T3[] = {&T[6],&T[7],&T[8]};
		DS[0].init(this, &DSLNM, T1);
		DS[1].init(this, &DSLNM, T2);
		DS[2].init(this, &DSLNM, T3);
		txt.pipelinesAndDescriptorSetsInit();		
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		PNM.cleanup();

//		SC.pipelinesAndDescriptorSetsCleanup();
		DS[0].cleanup();
		DS[1].cleanup();
		DS[2].cleanup();
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {	
		
		// Cleanup descriptor set layouts
		DSLNM.cleanup();
		
		// Destroys the pipelines
		PNM.destroy();		

//		SC.localCleanup();	
		M[0].cleanup();	
		M[1].cleanup();
		
		T[0].cleanup();
		T[1].cleanup();
		T[2].cleanup();
		T[3].cleanup();
		T[4].cleanup();
		T[5].cleanup();
		T[6].cleanup();
		T[7].cleanup();
		T[8].cleanup();

		txt.localCleanup();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		// binds the pipeline
//		SC.populateCommandBuffer(commandBuffer, currentImage);
		PNM.bind(commandBuffer);

		M[0].bind(commandBuffer);
		DS[0].bind(commandBuffer, PNM, 0, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M[0].indices.size()), 1, 0, 0, 0);

		M[0].bind(commandBuffer);
		DS[1].bind(commandBuffer, PNM, 0, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M[0].indices.size()), 1, 0, 0, 0);

		M[1].bind(commandBuffer);
		DS[2].bind(commandBuffer, PNM, 0, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M[1].indices.size()), 1, 0, 0, 0);

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
		
		static float cTime = 0.0;
		const float turnTime = 36.0f;
		const float angTurnTimeFact = 2.0f * M_PI / turnTime;
		
		cTime = cTime + deltaT;
		cTime = (cTime > turnTime) ? (cTime - turnTime) : cTime;
		
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
				if(currScene != 1) {
					currScene = (currScene+1) % outText.size();

				}
/*				if(currScene == 1) {
					if(subpass >= 4) {
						currScene = 0;
					}
				}
*/				std::cout << "Scene : " << currScene << "\n";
				
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


		if(glfwGetKey(window, GLFW_KEY_V)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_V;

				printVec3("CamPos  ", CamPos);				
				std::cout << "CamAlpha = " << CamAlpha << ";\n";
				std::cout << "CamBeta  = " << CamBeta  << ";\n";
			}
		} else {
			if((curDebounce == GLFW_KEY_V) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		
/*		if(currScene == 1) {
			switch(subpass) {
			  case 0:
CamPos   = glm::vec3(-3.2829, 3.32563, 6.59965);
CamAlpha = -0.247342;
CamBeta  = -0.289563;
lightOn  = glm::vec4(1,1,1,1);
				break;
			  case 1:
CamPos   = glm::vec3(7.46434, 5.44747, -0.464819);
CamAlpha = -5.12519;
CamBeta  = -0.586596;
lightOn  = glm::vec4(3,0,0,0);
				break;
			  case 2:
CamPos   = glm::vec3(0.803954, 6.86648, -7.92184);
CamAlpha = -3.34843;
CamBeta  = -0.824523;
lightOn  = glm::vec4(0,2,0,0);
				break;
			  case 3:
CamPos   = glm::vec3(-4.55548, 7.91204, -5.93445);
CamAlpha = -2.48351;
CamBeta  = -0.930016;
lightOn  = glm::vec4(0,0,5,0);
				break;
			}
		}
		
		if(currScene == 1) {
			subpassTimer += deltaT;
			if(subpassTimer > 4.0f) {
				subpassTimer = 0.0f;
				subpass++;
				std::cout << "Scene : " << currScene << " subpass: " << subpass << "\n";
				char buf[20];
				sprintf(buf, "A07_%d.png", subpass);
				saveScreenshot(buf, currentImage);
				if(subpass == 4) {
CamPos   = glm::vec3(0, 1.5, 7);
CamAlpha = 0;
CamBeta  = 0;
lightOn  = glm::vec4(1,1,1,1);
					currScene = 0;
					std::cout << "Scene : " << currScene << "\n";
					RebuildPipeline();
				}
			}
		}
*/

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
		gubo.lightDir = glm::vec3(cos(glm::radians(40.0f + 30.0f * cos(cTime * angTurnTimeFact))) * cos(cTime * angTurnTimeFact), sin(glm::radians(40.0f + 30.0f * cos(cTime * angTurnTimeFact))), cos(glm::radians(40.0f + 30.0f * cos(cTime * angTurnTimeFact))) * sin(cTime * angTurnTimeFact));
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = CamPos;
		
		ubo.mMat = baseTr * glm::translate(glm::mat4(1), glm::vec3(-1.5,3,0));
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS[0].map(currentImage, &ubo, sizeof(ubo), 0);
		DS[0].map(currentImage, &gubo, sizeof(gubo), 2);
		
		ubo.mMat = baseTr * glm::translate(glm::mat4(1), glm::vec3(1.5,3,0));
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS[1].map(currentImage, &ubo, sizeof(ubo), 0);
		DS[1].map(currentImage, &gubo, sizeof(gubo), 2);
		
		ubo.mMat = baseTr * glm::translate(glm::mat4(1), glm::vec3(0,-0.1,0));
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS[2].map(currentImage, &ubo, sizeof(ubo), 0);
		DS[2].map(currentImage, &gubo, sizeof(gubo), 2);
	}
};

// This is the main: probably you do not need to touch this!
int main() {
    E05 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
