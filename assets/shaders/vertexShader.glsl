// Vertex shader
#version 420

// Coming in from VAO
in vec4 vPosition;
in vec4 vColour;
in vec4 vNormal;
in vec4 vUV2;
in vec4 vTangent;
in vec4 vBiNormal;
in vec4 vBoneID;
in vec4 vBoneWeight;

// Out to the geometry shader
out vec4 gColour;
out vec4 gNormal;
out vec4 gWorldLocation;
out vec4 gUV2;
out vec4 gTangent;
out vec4 gBiNormal;
out vec4 gBoneID;
out vec4 gBoneWeight;
out vec3 gReflectedVector;
out vec3 gRefractedVector;

// Uniforms
uniform mat4 Model;
uniform mat4 ModelInverse;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 cameraPosition;
uniform bool u_UseBones;
uniform mat4 BoneMatrices[100];

void main()
{
	vec3 vertPosition = vPosition.xyz;

	mat4 MVP = Projection * View * Model;
	vec4 position = vec4(vertPosition, 1.0f);

	vec4 color = vColour;

	if(u_UseBones)
	{
		mat4 boneTransform = BoneMatrices[int(vBoneID[0])] * vBoneWeight[0];
		boneTransform += BoneMatrices[int(vBoneID[1])] * vBoneWeight[1];
		boneTransform += BoneMatrices[int(vBoneID[2])] * vBoneWeight[2];
		boneTransform += BoneMatrices[int(vBoneID[3])] * vBoneWeight[3];
		position = boneTransform * vec4(vertPosition, 1.0f);
		//color = vec4(10.f, 0.f, 0.f, 1.f);
	}

	gl_Position = MVP * position;

	gWorldLocation.xyz = (Model * position).xyz;
	gWorldLocation.w = 1.f;

	gNormal.xyz = normalize(ModelInverse * vec4(vNormal.xyz, 1.f)).xyz;
	gNormal.w = 1.f;

	// Reflection/Refraction
	vec3 unitNormal = normalize(gNormal.xyz);
	vec3 viewVector = normalize(gWorldLocation.xyz - cameraPosition);

	// Refractive indexes of air and water
	float ratio_refractive_index = 1.0/1.33;

	gReflectedVector = reflect(viewVector, unitNormal);
	gRefractedVector = refract(viewVector, unitNormal, 1.0/1.33);

	gColour = color;
	gUV2 = vUV2;
	gTangent = vTangent;
	gBiNormal = vBiNormal;
	gBoneID = vBoneID;
	gBoneWeight = vBoneWeight;
}
