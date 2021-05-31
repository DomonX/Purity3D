#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
flat in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture1;
uniform samplerCube depthMap;

uniform float far_plane;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

struct Ambient {
	vec3 color;
	float brightness;
	float min;
	float max;
};
uniform Ambient ambient;

vec3 calculateAmbient() {
	if(ambient.brightness == 0.0) {
		return vec3(0.0, 0.0, 0.0);
	}
	float eyeFragDistance = length(viewPos - FragPos);
	float distance = (eyeFragDistance / ambient.brightness) + 1.0f;
	float mul = ((ambient.max - ambient.min) / distance) + ambient.min;
	return ambient.color * mul;	
}


struct PointLight {
	vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
	
    vec3 diffuse;
    vec3 specular;

	float brightness;
};

#define MAX_POINT_LIGHTS 8
uniform PointLight[MAX_POINT_LIGHTS] pointLights;
uniform int pointLightsCounter;

vec3 calculatePointLight(PointLight light, vec3 norm, vec3 viewDir) {
	float distance = length(light.position - FragPos);
	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	float attuenation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 diffuse = light.color * (diff * material.diffuse * light.diffuse);

	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.color * material.specular * light.specular * spec;
	specular *= attuenation;
	diffuse *= attuenation;

	return ( diffuse + specular) * light.brightness;
}

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - pointLights[0].position;
    float closestDepth = texture(depthMap, fragToLight).r;
    closestDepth *= far_plane;
    float currentDepth = length(fragToLight);
    float bias = 0.05;
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}


void main() {
	//vec3 viewDir = normalize(viewPos - FragPos);
	//vec4 color = texture(texture1, TexCoord);
	//vec3 norm = normalize(Normal);
	//vec3 result = vec3(0.0);
	//for(int i = 0; i < pointLightsCounter; i++) {
	//	result += calculatePointLight(pointLights[i], norm, viewDir);
	//}
   
    //float shadow = ShadowCalculation(FragPos);

	// result = result * shadow;
	//result += calculateAmbient();
	//FragColor = color * vec4(result, 1.0);
	if(far_plane == 0.0) {
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	} else {
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
	// FragColor = vec4(result, 1.0);
}