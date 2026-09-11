#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirectionalLight{
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
};

struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;

	//atentuation
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;


	vec3 position;
	
	vec3 direction;

	//atentuation
	float constant;
	float linear;
	float quadratic;

	//spot
	float cutOff;
	float outerCutOff;
};

const int maxPointSize = 100;

uniform Material material;
uniform DirectionalLight directionalLight; 
uniform PointLight pointLights[maxPointSize];
uniform SpotLight spotLight;

out vec4 FragColor;

uniform vec3 viewPos;

in vec2 texCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuseTexture;

uniform int pointLightSize;

uniform vec3 fogColor;
uniform float fogDensity;

vec3 CalcDirectional(DirectionalLight _light, vec3 _normal, vec3 view){
	vec4 texColor = texture(diffuseTexture, texCoords);

	vec3 baseColor = texColor.rgb;

	vec3 ambient = material.ambient	 * _light.ambient;

	//normalize it for unit vectors
	vec3 normal = normalize(_normal);
	vec3 lightDirection = normalize(-_light.direction);

	//we use max if both vectors end up greater than 90.0 and result negative
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = (diff * material.diffuse) * _light.diffuse;

	vec3 viewDir = normalize(view - FragPos);
	//-lightDir, bc it expects from the light source 
	vec3 reflectDir = reflect(-lightDirection, normal);

	//raise it to the power of 32, value of shininess
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * _light.specular;

	vec3 result = (ambient + diffuse + specular) * baseColor;
	return result;
}

vec3 CalcPoint(PointLight _light, vec3 _normal, vec3 _view, vec3 _fragPos){
	
	vec4 texColor = texture(diffuseTexture, texCoords);

	vec3 baseColor = texColor.rgb;

	//first, calculate the length of a vector
	float distance = length(_light.position - _fragPos);

	//formula time
	float attenuation = 1.0 / (_light.constant + _light.linear * distance + 
    		    _light.quadratic * (distance * distance)); 

	vec3 ambient = material.ambient	 * _light.ambient;

	//normalize it for unit vectors
	vec3 normal = normalize(_normal);
	vec3 lightDirection = normalize(_light.position - _fragPos);

	//we use max if both vectors end up greater than 90.0 and result negative
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = (diff * material.diffuse) * _light.diffuse;

	vec3 viewDir = normalize(_view - _fragPos);
	//-lightDir, bc it expects from the light source 
	vec3 reflectDir = reflect(-lightDirection, normal);

	//raise it to the power of 32, value of shininess
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * _light.diffuse;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (ambient + diffuse + specular) * baseColor;
	return result;
}

vec3 CalcSpot(SpotLight _light, vec3 _normal, vec3 _view, vec3 FragPos){
	
	float theta = dot(normalize(_light.position - FragPos), normalize(-_light.direction));
	
	float distance = length(_light.position - FragPos);
	float attenuation = 1.0 / (_light.constant + _light.linear * distance + _light.quadratic * distance * distance);


	vec4 texColor = texture(diffuseTexture, texCoords);

	vec3 baseColor = texColor.rgb;

	//normalize it for unit vectors
	vec3 normal = normalize(_normal);
	vec3 lightDirection = normalize(_light.position - FragPos);

	//we use max if both vectors end up greater than 90.0 and result negative
	float diff = max(dot(normal, lightDirection), 0.0);

	vec3 viewDir = normalize(_view - FragPos);
	//-lightDir, bc it expects from the light source 
	vec3 reflectDir = reflect(-lightDirection, normal);

	//raise it to the power of 32, value of shininess
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = (material.specular * spec) * _light.specular;
	vec3 diffuse = (diff * material.diffuse) * _light.diffuse;
	vec3 ambient = material.ambient	 * _light.ambient;

	float epsilon   = _light.cutOff - _light.outerCutOff;
	float intensity = clamp((theta - _light.outerCutOff) / epsilon, 0.0, 1.0); 

	diffuse *= intensity;
	specular *= intensity;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (ambient + diffuse + specular) * baseColor;
	return result;
}

void main(){
	vec3 result = vec3(0.0);

	result = CalcDirectional(directionalLight, Normal, viewPos);

	for(int i = 0; i < pointLightSize; i++){
		//result += CalcPoint(pointLights[i], Normal, viewPos, FragPos);
	}

	result += CalcSpot(spotLight, Normal, viewPos, FragPos);

	float distance = length(viewPos - FragPos);
   	 float fogFactor = 1.0 - exp(-fogDensity * distance);
    	//result = mix(result, fogColor, fogFactor);

	FragColor = vec4(result, 1.0);
}