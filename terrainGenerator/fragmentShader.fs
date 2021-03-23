#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 position;
in vec2 TexCoord;
in vec3 kAmbient;
in vec3 kDiffuse;
uniform sampler2D texture1;
uniform float check;

void main()
{
    //light color
    vec3 kSpecular = vec3(1.0, 1.0, 1.0);
    float shin = 20;
    vec3 lightPosition = vec3(1.0, 1.0, 0.5);
    vec3 ambientLight = vec3(0.1, 0.1, 0.1);
    vec3 diffuseLight = vec3(0.8, 0.8, 0.8);
    vec3 specularLight = vec3(0.12, 0.12, 0.12);

    //calculate color with Phong reflection model
    vec3 cameraPositionView = vec3(0.0, 0.0, 0.0);
    vec3 lightVector = normalize(lightPosition - position);
    vec3 reflectionVector = normalize(reflect(-lightVector, normal));
    vec3 viewVector = normalize(cameraPositionView - position);
    float diffuseWeight = max(dot(normal, lightVector), 0.0);
    float rDotV = max(dot(reflectionVector, viewVector), 0.0);
    float specularWeight = pow(rDotV, shin);
    vec4 vertexColor = vec4((kAmbient * ambientLight + kDiffuse * diffuseLight * diffuseWeight + kSpecular * specularLight * specularWeight), 1.0);

    if (check > 0) {
        FragColor = vertexColor;
    } else {
        FragColor = texture(texture1, TexCoord);
    }
}