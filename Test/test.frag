#version 330 core

in vec2 tex;
in vec3 nor;
in vec3 FragPos;

out vec4 fragCol;
uniform vec3 camPos;
uniform vec3 dLightDirection;
uniform vec3 dLightColour;

uniform vec3 pLightDirection;
uniform vec3 pLightPos;
uniform vec3 pLightColour;


uniform vec3 sLightDirection;
uniform vec3 sLightPos;
uniform vec3 sLightColour;

uniform sampler2D Texture;


float CalculateAttenuation(vec3 lightPos)
{
    float d = length(lightPos - FragPos);
    float attc = 1.5;
    float attl = 0.05;
    float attq = 0.02;

    return 1.f / (attc + attl * d + attq * pow(d, 2));
}

float CalculateAmbDiffSpec(vec3 ToLight, vec3 lightDire)
{
    float ambient = 0.1;
    float diffuse = max(dot(normalize(nor), normalize(ToLight)), 0.f);

    vec3 NcamDirection = normalize(camPos - FragPos);
    vec3 NrefLight = reflect(normalize(lightDire), normalize(nor));
    float specular = pow(max(dot(NcamDirection, NrefLight), 0.f), 128);

    return ambient + diffuse + specular;
}

float CalculateDirectionalIllumination()
{
    return CalculateAmbDiffSpec(-dLightDirection, dLightDirection);
}

float CalculatePositionalIllumination()
{
    vec3 ToLight = pLightPos - FragPos;
    float attenuation = CalculateAttenuation(pLightPos);
    float ads = CalculateAmbDiffSpec(ToLight, -ToLight);
    return ads * attenuation;
}

float CalculateSpotIllumination()
{
    float ambient = 0.1;
    vec3 NToLight = normalize(sLightPos - FragPos);
    vec3 NFromLight = -NToLight;

    float attenuation = CalculateAttenuation(sLightPos);
    float phong = 0.f;

    float phi = cos(radians(15.f));
    vec3 NSpotDir = normalize(sLightDirection);
    float theta = dot(NFromLight, NSpotDir);

    if(theta > phi)
        phong = CalculateAmbDiffSpec(NToLight, sLightDirection) * attenuation; 
    else
        phong = ambient * attenuation;

    return phong;
}

void main()
{
    vec4 col = texture(Texture, tex);
    
    float phongD = CalculateDirectionalIllumination();
    float phongP = CalculatePositionalIllumination();
    float phongS = CalculateSpotIllumination() ;

    vec3 colD = phongD * col.xyz * dLightColour;
    vec3 colP = phongP * col.xyz * pLightColour;
    vec3 colS = phongS * col.xyz * sLightColour;

    fragCol = vec4(colD + colP + colS, 1.f);
}