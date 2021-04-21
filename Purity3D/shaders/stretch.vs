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

void main()
{
	vec3 pos = aPos;
	if(pos.y > 0) {
		pos.y += 0.5;
	} else {
		pos.y -= 0.5;
	}
	FragPos = vec3(model * vec4(pos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;
}
