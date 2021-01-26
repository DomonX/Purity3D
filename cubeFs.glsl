#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	vec4 AlphaColor = texture(texture2, TexCoord);
	if(AlphaColor.a < 0.1) {
		FragColor = texture(texture1, TexCoord) * vec4(ambient, 1.0);
	} else {
		FragColor = mix(texture(texture1, TexCoord), AlphaColor, 0.2) * vec4(ambient, 1.0);
	}
}
