#version 330 core


in vec2 tex;
in vec3 nor;
in vec3 FragPos;

out vec4 fragCol;

uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform sampler2D Texture;

float CalculateAttenuation()
{
	float d = length(lightPos - FragPos);
	float attc = 1.5;
	float attl = 0.05;
	float attq = 0.02;

	return 1.f / (attc + attl * d + attq * pow(d,2));
}

float CalculateAmbDiffSpec(vec3 ToLight, vec3 lightDire)
{
	float ambient = 0.1;
	float diffuse = max(dot(normalize(nor), normalize(ToLight)), .0f);

	vec3 NcamDirection = normalize(camPos - FragPos);
	vec3 NrefLight = reflect(normalize(lightDire), normalize(nor));
	float specular = pow(max(dot(NcamDirection, NrefLight), .0f ), 128);

	return ambient + diffuse + specular; 
}

float CalculateDirectionalIllumination()
{
	return CalculateAmbDiffSpec(-lightDirection, lightDirection); 
}

float CalculatePositionalIllumination()
{
	vec3 ToLight = lightPos - FragPos;
	float attenuation = CalculateAttenuation();
	float ads = CalculateAmbDiffSpec(ToLight, -ToLight);
	return ads * attenuation; 
}

float CalculateSpotIllumination()
{
	float ambient = 0.5;

	vec3 NToLight = normalize(lightPos - FragPos);
	vec3 NFromLight = -NToLight;

	float attenuation = CalculateAttenuation();
	float phong = 0.f; 

	float phi = cos(radians(15.f));
	vec3 NSpotDir = normalize(lightDirection);
	float theta = dot(NFromLight, NSpotDir);

	if(theta > phi)
		phong = CalculateAmbDiffSpec(NToLight, lightDirection) * attenuation; 
	else
		phong = ambient * attenuation;


	return phong;
}

void main()
{
	//vec4 col = texture(Texture, tex);
	float phong = CalculatePositionalIllumination();
	//fragCol = vec4(phong * col2 * lightColour, 1.f);
	//fragCol = vec4(1.f, 0.f, 0.f, 1.f);
	vec4 col = texture(Texture, tex);
	fragCol =  vec4(phong * col.xyz * lightColour, 1.f);
}
