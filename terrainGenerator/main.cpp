#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "faultingTerrain.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>
#include "stb_image.h"
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* setup();

// settings
const unsigned int width = 1200;
const unsigned int height = 600;


/**
 * @brief set up a window for opengl
 */
GLFWwindow* setup()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(width, height, "Terrain", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return window;
    }
    return window;
}

/**
 * @brief change width or height of the window
 * @param window    The window for demonstration
 * @param width     The width of the window
 * @param height    The height of the window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}

int main()
{
    GLFWwindow* window = setup();
 
    //connect to shader
    Shader myShader("vertexShader.vs", "fragmentShader.fs"); 

    //create a terrain
    FaultingTerrain myTerrain = FaultingTerrain(64, -1, 1, -1, 1);

    //define the way to shade
    //0.0 is shading with texture
    //1.0 is shading by elevation
    float shading = 1.0;

    //create buffers
    unsigned int positionBuffer, vertexBuffer, normalBuffer, elementBuffer;
    glGenVertexArrays(1, &vertexBuffer);
    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &elementBuffer);
    glBindVertexArray(vertexBuffer);

    //bind position buffer and send position data
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * myTerrain.positionNumber * 3, myTerrain.position, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * myTerrain.triangleNumber * 3, myTerrain.element, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //bind normal buffer and send normal data
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* myTerrain.positionNumber * 3, myTerrain.normal, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
  
    //bind texture buffer and send texture data
    float textureData[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int texturewidth, textureheight, textureChannels;
    unsigned char* data = stbi_load("texture.jpg", &texturewidth, &textureheight, &textureChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturewidth, textureheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        //clear window in the beginning of every loop
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //set view transform matrix
        glm::vec3 eyePt = glm::vec3(-1.5f, 0.0f, 2.0f);
        glm::vec3 lookAtPt = glm::vec3(0.2f, 0.0f, -0.5f);
        glm::vec3 up = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::mat4 camera = glm::lookAt(eyePt, lookAtPt, up);

        //set projection matrix
        glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        glBindTexture(GL_TEXTURE_2D, texture);

        myShader.use();

        //send view transform matrix to vertex shader
        unsigned int viewTransMatrix = glGetUniformLocation(myShader.ID, "viewTransMatrix");
        glUniformMatrix4fv(viewTransMatrix, 1, GL_FALSE, glm::value_ptr(camera));

        //send projection matrix to vertex shader
        unsigned int projectionMatrix = glGetUniformLocation(myShader.ID, "projTransMatrix");
        glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

        //send minZ, maxZ and shading type to fragment shader
        unsigned int check = glGetUniformLocation(myShader.ID, "check");
        glUniform1f(check, shading);
        unsigned int minZ = glGetUniformLocation(myShader.ID, "minZ");
        glUniform1f(minZ, myTerrain.minZ);
        unsigned int maxZ = glGetUniformLocation(myShader.ID, "maxZ");
        glUniform1f(maxZ, myTerrain.maxZ);


        //draw
        glBindVertexArray(vertexBuffer);
        glDrawElements(GL_TRIANGLES, myTerrain.triangleNumber * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //close window
    glfwTerminate();
    return 0;
}


