#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
flat out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool reverse_normals;

void main()
{
	vec3 NormalTemp = reverse_normals ? aNormal * -1.0f : aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * NormalTemp;
}
