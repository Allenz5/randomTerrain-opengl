#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNorm;
layout (location = 1) in vec2 aTexCoord;

out vec3 normal;
out vec3 position;
out vec2 TexCoord;
out vec3 kAmbient;
out vec3 kDiffuse;

uniform mat4 viewTransMatrix;
uniform mat4 projTransMatrix;
uniform float check;
uniform float minZ;
uniform float maxZ;


void main()
{
    //generate color by elevation
    if (check > 0) {
        float nrmZ = (aPos.z-minZ)/(maxZ-minZ);
        if (nrmZ <= 0.30) {
            kAmbient = vec3(0.0, max(0.0, (nrmZ - 0.20)*10.0), 1.0 - max(0.0, (nrmZ - 0.20)*10.0));
            kDiffuse = vec3(0.0, max(0.0, (nrmZ - 0.20)*10.0), 1.0 - max(0.0, (nrmZ - 0.20)*10.0));
        } else if (nrmZ <= 0.50 && nrmZ > 0.30) {
            kAmbient = vec3(max(0.0, (nrmZ - 0.40)*10.0), 1.0 - max(0.0, (nrmZ - 0.40)*10.0), 0.0);
            kDiffuse = vec3(max(0.0, (nrmZ - 0.40)*10.0), 1.0 - max(0.0, (nrmZ - 0.40)*10.0), 0.0);
        } else if (nrmZ <= 0.75 && nrmZ > 0.50){
            kAmbient = vec3(1.0, max(0.0, (nrmZ - 0.65)*10.0), max(0.0, (nrmZ - 0.65)*10.0));
            kDiffuse = vec3(1.0, max(0.0, (nrmZ - 0.65)*10.0), max(0.0, (nrmZ - 0.65)*10.0));
        } else {
            kAmbient = vec3(1.0, 1.0 ,1.0);
            kDiffuse = vec3(1.0, 1.0 ,1.0);
        }
    } else {
        kAmbient = vec3(1.0, 1.0, 1.0);
        kDiffuse = vec3(1.0, 1.0, 1.0);
    }


    //calculate vertex position
    gl_Position = projTransMatrix * viewTransMatrix * vec4(aPos, 1.0);

    //send position value and normal value to fragment shader
    position = (viewTransMatrix * vec4(aPos, 1.0)).xyz;
    normal = aNorm;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);

}