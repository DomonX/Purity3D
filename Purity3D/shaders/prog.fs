#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
flat in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture1;

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

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (diffuse + specular);
}

#define MAX_DIR_LIGHTS 4
uniform DirLight[MAX_DIR_LIGHTS] dirLights;
uniform int dirLightsCounter;

vec4 calculateProg(vec4 color) {
    float bri = (color.r + color.g + color.b) / 3;
    if(bri > 0.6) {
        return vec4(1.0);
    }
    if(bri > 0.4) {
        return vec4(vec3(0.5), 1);
    }
   return vec4(vec3(0.0), 1);
}

void main() {
	vec3 result = calculateAmbient();
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(Normal);
	for(int i = 0; i < pointLightsCounter; i++) {
		result += calculatePointLight(pointLights[i], norm, viewDir);
	}
	for(int i = 0; i < dirLightsCounter; i++) {
		result += calculateDirLight(dirLights[i], norm, viewDir);
	}
    
	FragColor = calculateProg(texture(texture1, TexCoord)) * vec4(result, 1.0);
}