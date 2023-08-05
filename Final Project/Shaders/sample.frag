#version 330 core 

out vec4 FragColor;

uniform sampler2D tex0;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;

uniform float ambientStr;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

uniform sampler2D norm_tex;

in mat3 TBN;


void main() {

	vec4 pixelColor = texture(tex0, texCoord);
	//if alpha is low enough
	if (pixelColor.a < 0.1){
	discard;
	}

	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	//vec3 normal = normalize(normCoord);

	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 ambientCol = ambientColor * ambientStr;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec* specStr * lightColor;

	//calculate attenuation (light intensity with distance)
    float distance = distance(fragPos, lightPos); // calculates the distance between the fragment position 
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    
    //calculate point light contribution (realistic lighting)
    vec3 pointLight = (diffuse + specColor) * attenuation;
    
    //calculate final color (lighting calculations)
    //FragColor = texture(tex0, texCoord) * vec4(pointLight + ambientCol, 1.0);

	//FragColor = vec4(0.7f, 0.0f, 0.0f, 1.0f);
	//FragColor = texture(tex0, texCoord);
	FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * texture(tex0, texCoord);


}