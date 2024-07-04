// This has been adapted from the Vulkan tutorial

#include "modules/Starter.hpp"

// The uniform buffer object used in this example

// This contains the transform matrices for the object. In this case it is Binding 0 of Set 1
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

// This contains the material parameters for the object. In this case it is Binding 2 of Set 1
struct MaterialUniformBufferObject {
	alignas(16) glm::vec4 specDef;
};

// This are the scene-wise uniforms. In this case the light parameters and the position of the camera.
// They are mapped to Binding 0 of Set 0
struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec4 lightParamsInOutGbeta;
	alignas(16) glm::vec3 eyePos;
};


// This is the Vertex definition
struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

// MAIN ! 
class E08 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	// In this case, DSL1 is used for the shader specific parameters, and it is mapped to Set 0
	// DSLG contains the global parameters and it is mapped to Set 1
	DescriptorSetLayout DSL1, DSLG;

	// Vertex descriptor
	VertexDescriptor VD;

	// Pipelines [Shader couples]
	Pipeline P1;

	// Models, textures and Descriptor Sets (values assigned to the uniforms)
	Model M1;
	Texture T1;
	DescriptorSet DS1, DSG; // Even if we have just one object, since we have two DSL, we also need two sets.
	
	// Other application parameters
	// Current aspect ratio, used to build a correct Projection matrix
	float Ar;

	// Camera attributes
	glm::vec3 CamPos = glm::vec3(0.0, 1.5, 7.0);
	float CamAlpha = 0.0f;
	float CamBeta = 0.0f;

	// Here you set the window parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment _SM";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.6f, 0.8f, 1.0f};
		
		// The initial aspect rati of the window. In this code, we assume square pixels
		Ar = 4.0f / 3.0f;
	}
	
	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		std::cout << "Window resized to: " << w << " x " << h << "\n";
		// The most important thing to do when the window changes size is updating the aspect ratio.
		Ar = (float)w / (float)h;
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts, vertex descriptors and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSL1.init(this, {
					// this array contains the bindings definitions:
					// first  element : the binding number
					// second element : the type of element (buffer or texture) - a Vulkan constant
					// third  element : the pipeline stage where it will be used - a Vulkan constant
					// fourth element : for uniform buffers -> the size of the data to be passed
					//                  for textures        -> the index of the texture in the array passed to the binding function
					// fifth  element : the number of elements of this type to be created. Usually 1
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 1},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 1},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(MaterialUniformBufferObject), 1}
				  }); // This is the Set for the object, that contains the transform matrices in Binding 0,
				      // the diffuse color texture in Binding 1, and the Specular parameters in Binding 2
		DSLG.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, sizeof(GlobalUniformBufferObject), 1}
				  }); // This is the scene-wise set, that contains the light definitions.
		// Vertex definition (used by the models)
		VD.init(this, {
				  // this array contains the bindings
				  // first  element : the binding number
				  // second element : the stride (size of a record) of this binging
				  // third  element : whether this parameter change per vertex or per instance
				  //                  using the corresponding Vulkan constant
				  // CURRENTLY only Single Binding scenario have been tested.
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  // this array contains the location of the components of the vertex
				  // first  element : the binding number
				  // second element : the location number
				  // third  element : the offset of this element in the memory record - usually computed with the offsetof() macro.
				  // fourth element : the data type of the element
				  //                  using the corresponding Vulkan constant
				  // fifth  elmenet : the size in byte of the element - generaly obtained with a sizeof() macro
				  // sixth  element : a constant defining the element usage
				  //                   POSITION - a vec3 with the position
				  //                   NORMAL   - a vec3 with the normal vector
				  //                   UV       - a vec2 with a UV coordinate
				  //                   COLOR    - a vec4 with a RGBA color
				  //                   TANGENT  - a vec4 with the tangent vector
				  //                   OTHER    - anything else
				  //				These constnats are used when loading a model, to fill the provided vertex structure
				  //				with the data contained in the file.
				  // this array must have exactly one element per field of the Cpp structure corresponding to the vertex
				  // ***************** DOUBLE CHECK ********************
				  //    That the Vertex data structure you use in the "offsetoff" and
				  //	in the "sizeof" in the previous array, refers to the correct one,
				  //	if you have more than one vertex format!
				  // ***************************************************
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
				         sizeof(glm::vec2), UV}
				}); // This is a classical vertex of a simple textured smooth model,
				    // which includes the position, the normal vector and the UV coordinates.

		// Pipelines [Shader couples]		// Pipelines [Shader couples]

		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders files containing the SPV code
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, &VD, "shaders/BlinnVert.spv", "shaders/BlinnFrag.spv", {&DSLG, &DSL1});

		// Models, textures and Descriptors (values assigned to the uniforms)

		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ, GLTF or the custom type MGCG
		M1.init(this, &VD, "models/City.obj", OBJ);
		
		// Create the textures
		// The second parameter is the file name containing the image
		T1.init(this, "textures/TexturesCity.png");

		// Descriptor pool sizes
		DPSZs.uniformBlocksInPool = 3;	// Uniform block. In this case we have 3: the global one (Set 0), plus the two blocks
										// in Set 1.
		DPSZs.texturesInPool = 1;		// Just one texture
		DPSZs.setsInPool = 2;			// We have two sets: the global one (Set 0) and the object/materuak one (Set 1).
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		// This creates a descriptor set, according to a given Layout. For texture bindings, the creation assigns the texture.
		// Second parameter is a pointer to the considered Descriptor Set Layout
		// Third parameter is a vector of pointer to textures. The DSL, for each texture, in its linkSize field (fourth element),
		// specifies the index of the texture in this array to pass to the shader
		DS1.init(this, &DSL1, {&T1});
		DSG.init(this, &DSLG, {}); // note that if a DSL has no texture, the array can be empty
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		DS1.cleanup();
		DSG.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		M1.cleanup();

		DSL1.cleanup();
		DSLG.cleanup();
		
		P1.destroy();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		// The resources that needs to be bound for drawing something, are:
		
		// The pipeline with the shader for the object
		P1.bind(commandBuffer);
		
		// The models (both index and vertex buffers)
		M1.bind(commandBuffer);
		
		// The descriptor sets, for each descriptor set specified in the pipeline
		// For this reason, the second parameter refers to the corresponding pipeline
		// And the third is the Set number to which the descriptor set should be bound
		DSG.bind(commandBuffer, P1, 0, currentImage);	// The Global Descriptor Set (Set 0)
		DS1.bind(commandBuffer, P1, 1, currentImage);	// The Material and Position Descriptor Set (Set 1)
					
		// The actual draw call. It uses directly the Vulkan procedure. The number of triangles to draw
		// is passed in the second parameter, and it is taken from the vector containg the index buffer of
		// the corresponding model
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);	
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

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		glm::mat4 M = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 50.0f);
		M[1][1] *= -1;

		glm::mat4 Mv =  glm::rotate(glm::mat4(1.0), -CamBeta, glm::vec3(1,0,0)) *
						glm::rotate(glm::mat4(1.0), -CamAlpha, glm::vec3(0,1,0)) *
						glm::translate(glm::mat4(1.0), -CamPos);

		glm::mat4 ViewPrj =  M * Mv;

		const glm::vec3 lightPos = glm::vec3(30.0f, 30.0f, 20.0f);
		const glm::vec3 lightDir = glm::vec3(cos(glm::radians(135.0f)) * cos(glm::radians(210.0f)), sin(glm::radians(135.0f)), cos(glm::radians(135.0f)) * sin(glm::radians(210.0f)));
		const float cosout = 0.9495f;
		const float cosin  = 0.95f;
		
		GlobalUniformBufferObject gubo{};

		UniformBufferObject ubo{};								
		MaterialUniformBufferObject mubo{};

		// updates global uniforms
		gubo.lightDir = lightDir;
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.lightPos = lightPos;
		gubo.lightParamsInOutGbeta.x = cosin;
		gubo.lightParamsInOutGbeta.y = cosout;
		gubo.lightParamsInOutGbeta.z = 1.0;
		gubo.eyePos = CamPos;

		// Maps (transfers to the shader) the global Descriptor Set
		DSG.map(currentImage, &gubo, 0);

		ubo.mvpMat = ViewPrj;
		ubo.mMat = glm::mat4(1);
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		
		mubo.specDef = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f);

		// Maps (transfers to the shader) the uniform buffers of the Descriptor Set containing the
		// transform matrices (in Binding 0), and the material parameters (in Binding 2).
		DS1.map(currentImage, &ubo, 0);
		DS1.map(currentImage, &mubo, 2);
	}
	};


// This is the main: probably you do not need to touch this!
int main() {
    E08 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}