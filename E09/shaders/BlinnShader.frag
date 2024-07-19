#version 450#extension GL_ARB_separate_shader_objects : enable
// this defines the variable received from the Vertex Shader// the locations must match the one of its out variables
layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec2 fragUV;
// This defines the color computed by this shader. Generally is always location 0.
layout(location = 0) out vec4 outColor;// Here the Uniform buffers are defined. In this case, the Global Uniforms of Set 0// The texture of Set 1 (binding 1), and the Material parameters (Set 1, binding 2)// are used. Note that each definition must match the one used in the CPP codelayout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	vec3 lightDir;	vec4 lightColor;	vec3 eyePos;} gubo;
layout(set = 1, binding = 1) uniform sampler2D tex;// The main shader, implementing a simple Blinn + Lambert + constant Ambient BRDF model// The scene is lit by a single Spot Lightvoid main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);		vec3 lightDir = gubo.lightDir;	vec3 lightColor = gubo.lightColor.rgb;

	vec3 Diffuse = texture(tex, fragUV).rgb * 0.95f * max(dot(Norm, lightDir),0.0);	vec3 Specular = vec3(pow(max(dot(Norm, normalize(lightDir + EyeDir)),0.0), 200.0f));
	vec3 Ambient = texture(tex, fragUV).rgb * 0.05f;		vec3 col  = (Diffuse + Specular) * lightColor.rgb + Ambient;	
	outColor = vec4(col, 1.0f);}