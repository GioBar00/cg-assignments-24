#version 450#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec4 fragTan;layout(location = 3) in vec2 fragUV;

layout(location = 0) out vec4 outColor;layout(binding = 1) uniform sampler2D tex;
layout(binding = 3) uniform sampler2D normMap;layout(binding = 4) uniform sampler2D matMap;
layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, vec3 Ms, float gamma) {	vec3 Diffuse = Md * clamp(dot(N, L),0.0,1.0);	vec3 Specular = Ms * vec3(pow(clamp(dot(N, normalize(L + V)),0.0,1.0), gamma));	return (Diffuse + Specular);}vec3 AMBIENT(vec3 V, vec3 N) {	const vec3 cxp = vec3(1.0,0.5,0.5) * 0.05;	const vec3 cxn = vec3(0.9,0.6,0.4) * 0.05;	const vec3 cyp = vec3(0.3,1.0,1.0) * 0.05;	const vec3 cyn = vec3(0.5,0.5,0.5) * 0.05;	const vec3 czp = vec3(0.8,0.2,0.4) * 0.05;	const vec3 czn = vec3(0.3,0.6,0.7) * 0.05;		return((N.x > 0 ? cxp : cxn) * (N.x * N.x) +		   (N.y > 0 ? cyp : cyn) * (N.y * N.y) +		   (N.z > 0 ? czp : czn) * (N.z * N.z));}
void main() {
	vec3 Norm = normalize(fragNorm);	vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));	vec3 Bitan = cross(Norm, Tan) * fragTan.w;	mat3 tbn = mat3(Tan, -Bitan, Norm);	vec4 nMap = texture(normMap, fragUV);	vec3 N = normalize(tbn * (nMap.rgb * 2.0 - 1.0));	vec3 albedo = texture(tex, fragUV).rgb;	vec4 MRAO = texture(matMap, fragUV);	float roughness = MRAO.g;	float pex = 1000.0f * (1.0 - roughness) * (1.0 - roughness);	float ao = MRAO.b;	float metallic = MRAO.r;		vec3 L = gubo.lightDir;	vec3 lightColor = gubo.lightColor.rgb;	vec3 V = normalize(gubo.eyePos - fragPos);	vec3 DiffSpec = BRDF(V, N, L, albedo, vec3(1.0 - roughness), pex);	vec3 Ambient =AMBIENT(N, V) * albedo * ao;		outColor = vec4(DiffSpec * lightColor.rgb + Ambient, 1.0f);//	outColor = vec4(0.5f*N+vec3(0.5f), 1.0f);}