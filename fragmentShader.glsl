#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 AlphaColor = texture(texture2, TexCoord);
	if(AlphaColor.a < 0.1) {
		FragColor = texture(texture1, TexCoord);
	} else {
		FragColor = mix(texture(texture1, TexCoord), AlphaColor, 0.2);
	}
}
