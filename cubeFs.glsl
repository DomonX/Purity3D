#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
flat in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;
uniform vec3 viewPos;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight;

struct Light {
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float stren;
	float distStren;
};
  
uniform Material material;

uniform Light light;

void main()
{
	// Distance between Light and eye
	float distance = length(light.position - viewPos);
	// Distance between light and pixel
	float distanceFrag = length(light.position - FragPos);
	// Distance between eye and pixel
	float eyeDist = length(viewPos - FragPos);

	float distanceReductionStren = distanceFrag * light.distStren;
	distanceReductionStren < 1.0 ? 1.0 : distanceReductionStren;

	eyeDist < 1.0 ? 1.0 : eyeDist;

	vec3 eyeAmbient = vec3(0.04, 0.04, 0.04) / eyeDist;

	vec3 ambient = vec3(0.8, 0.8, 1.0) * light.ambient;
	ambient = ambient / distanceReductionStren;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * (diff * material.diffuse * light.diffuse);
	diffuse = diffuse * (1.0 / (1.0 + (0.1 * distance)));
	diffuse = diffuse / distanceFrag;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor * material.specular * light.specular * spec;
	specular = specular / distanceFrag;

	// Light result
	vec3 result = ((ambient + diffuse + specular) * light.stren) + eyeAmbient;

	FragColor = texture(texture1, TexCoord) * vec4(result, 1.0);
}
