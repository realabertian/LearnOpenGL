#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// �����
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
// ���㶨��⺯��

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

// ���Դ
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

// ������Դ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


// �۹�Դ
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

SpotLight spotLight;

// ����۹�

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shiniess;
};

uniform vec3 viewPos;
uniform Material material;

vec4 CalOnlyMaterail();


void main()
{
    // ���� ��Щ����ȫ������������ϵ
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // ��һ�׶� ���㶨�����
   // vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // �ڶ��׶�: ������Դ
 //   for(int i = 0; i < NR_POINT_LIGHTS; i++)
 //       result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    // �����׶�: ����۹�
//    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    

    //FragColor = vec4(result, 1.0);
    FragColor = CalOnlyMaterail();
}

vec4 CalOnlyMaterail() 
{
    return texture(material.texture_diffuse1, TexCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);

    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiniess);
    // �ϲ�
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // ������
    float diff = max(dot(normal, lightDir), 0.0);

    // �����
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiniess);

    // ˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // �ϲ����
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.cutOff)
    {
        // ������, ��Ҫִ�й��ռ���
        // ������
        float diff = max(dot(normal, lightDir), 0.0);

        // �۹��Եƽ������
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        // �����
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiniess);

        // ˥��
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
        // �ϲ����
        vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

        // �۹��Եƽ��
        diffuse *= intensity;
        specular *= intensity;

        // ���Դ˥������
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        //
        return (ambient + diffuse + specular);
    }
    else // û�б����䵽, ����ʹ�û�����,�ó����ھ۹���ʱ��������ȫ�ڰ�
        return light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
}