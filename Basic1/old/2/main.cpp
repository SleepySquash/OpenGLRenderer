//
//  main.m
//  Basic1
//
//  Created by Никита Исаенко on 02.01.2021.
//

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.hpp"
#include "Camera.hpp"

using std::cout;
using std::cerr;
using std::endl;

int ec;
char ecode[512];

void framebufferSizeCallback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

Camera camera;
float lastX = 400, lastY = 300;
bool firstMouse = true;
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos; lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (!camera.hasFocus && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FOCUS_GAIN, 0.f);
        glfwSetInputMode(window, GLFW_CURSOR, camera.hasFocus ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (camera.hasFocus && key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FOCUS_LOST, 0.f);
        glfwSetInputMode(window, GLFW_CURSOR, camera.hasFocus ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    
    if (key == GLFW_KEY_LEFT_SHIFT)
    {
        if (action == GLFW_PRESS) camera.MovementSpeed = camera.ShiftSpeed;
        else camera.MovementSpeed = camera.NormalSpeed;
    }
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;
void processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
}

struct RenderObject
{
    GLuint VAO, VBO, EBO;
    GLuint texture;
};

int main(int argc, const char * argv[])
{
    if (!glfwInit()) { cout << "glfwInit() error\n"; glfwTerminate(); return 1; }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4 - max
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 1 - max
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Error outdated functionality
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    
    GLFWwindow* window = glfwCreateWindow(960, 600, "Title", nullptr, nullptr);
    if (!window) { cout << "Error window creating\n"; glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window); // Use this window as OpenGL context
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK) { cout << "glewInit() error\n"; glfwDestroyWindow(window); glfwTerminate(); return 1; }
    
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    
    glfwSetInputMode(window, GLFW_CURSOR, camera.hasFocus ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    stbi_set_flip_vertically_on_load(true);
    
    GLfloat cube_vertices[] = {
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f, 1.0f,
        
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f, 1.0f,
        
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,     1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
        
       -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,     1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
        
       -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
        
       -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
    };
    GLuint cube_indices[36];
    for (int i = 0; i < 6; ++i)
    {
        cube_indices[i*6 + 0] = i*4 + 0;
        cube_indices[i*6 + 1] = i*4 + 1;
        cube_indices[i*6 + 2] = i*4 + 2;
        cube_indices[i*6 + 3] = i*4 + 2;
        cube_indices[i*6 + 4] = i*4 + 3;
        cube_indices[i*6 + 5] = i*4 + 0;
    }
    
    // Vertices, buffers, indices, texture
    RenderObject cube;
    GLuint diffuseMap, specularMap;
    {
        // VAO
        glGenVertexArrays(1, &cube.VAO);
        glBindVertexArray(cube.VAO);
        
        // VBO
        glGenBuffers(1, &cube.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        
        // EBO
        glGenBuffers(1, &cube.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
        
        // Diffuse Texture
        {
            glGenTextures(1, &diffuseMap);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            // settings
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load the file
            int width, height, channels;
            unsigned char* data = stbi_load("Images/1.jpg", &width, &height, &channels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else { cout << "stbi_load() Error: failed to load texture.\n"; glfwDestroyWindow(window); glfwTerminate(); return 1; }
            stbi_image_free(data);
        }
        
        // Specular Texture
        {
            glGenTextures(1, &specularMap);
            glBindTexture(GL_TEXTURE_2D, specularMap);
            // settings
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load the file
            int width, height, channels;
            unsigned char* data = stbi_load("Images/3.jpg", &width, &height, &channels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else { cout << "stbi_load() Error: failed to load texture.\n"; glfwDestroyWindow(window); glfwTerminate(); return 1; }
            stbi_image_free(data);
        }
        
        // layout's location 0 for one vertex is 3 floats (x, y, z) with stride of 0; (stride might be (void*)(3 * sizeof(float)))
        // layout's location 1 for one vertex is 2 float (s, t) with stride of 3 (x, y, z)
        // total: 5 floats
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(0));                 glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    }
    
    
    RenderObject light;
    {
        // VAO
        glGenVertexArrays(1, &light.VAO);
        glBindVertexArray(light.VAO);
        
        // VBO
        glGenBuffers(1, &light.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, light.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        
        // EBO
        glGenBuffers(1, &light.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    
        // Texture
        glGenTextures(1, &light.texture);
        glBindTexture(GL_TEXTURE_2D, light.texture);
        // settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load the file
        int width, height, channels;
        unsigned char* data = stbi_load("Images/2.jpg", &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else { cout << "stbi_load() Error: failed to load texture.\n"; glfwDestroyWindow(window); glfwTerminate(); return 1; }
        stbi_image_free(data);
        
        // layout's location 0 for one vertex is 3 floats (x, y, z) with stride of 0; (stride might be (void*)(3 * sizeof(float)))
        // layout's location 1 for one vertex is 2 float (s, t) with stride of 3 (x, y, z)
        // total: 5 floats
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(0)); glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    }
    
    
    
    // Shader program
    ShaderProgram program("Shaders/triangle.vert", "Shaders/triangle.frag");
    
    
    glm::mat4 view;
    glm::mat4 proj = glm::mat4(1.f);
    proj = glm::perspective(glm::radians(45.f), (float)bufferWidth/bufferHeight, 0.1f, 100.f);
    
    
#define NUM_LIGHTS 4
    int lights = NUM_LIGHTS;
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    program.use();
    program.setVec3("viewPos", camera.Position);
    program.setVec3("dirLight.direction",  -0.2f, -1.f, -0.3f);
    program.setVec3("dirLight.ambient",  0.2f, 0.2f, 0.2f);
    program.setVec3("dirLight.diffuse",  0.7f, 0.7f, 0.7f);
    program.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < lights; ++i)
    {
        program.setVec3("pointLights[" + std::to_string(i) + "].position",  pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
        program.setVec3("pointLights[" + std::to_string(i) + "].ambient",  0.2f, 0.2f, 0.2f); // 0.2f, 0.2f, 0.2f
        program.setVec3("pointLights[" + std::to_string(i) + "].diffuse",  0.7f, 0.7f, 0.7f); // 0.7f, 0.7f, 0.7f
        program.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f); // 1.0f, 1.0f, 1.0f
        program.setFloat("pointLights[" + std::to_string(i) + "].constant",  1.0f);
        program.setFloat("pointLights[" + std::to_string(i) + "].linear",    0.09f);
        program.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }
    program.setVec3("spotLight.position", camera.Position);
    program.setVec3("spotLight.direction", camera.Front);
    program.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    program.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    program.setFloat("spotLight.constant", 1.f);
    program.setFloat("spotLight.linear", 0.09f);
    program.setFloat("spotLight.quadratic", 0.032f);
    program.setVec3("spotLight.ambient",  0.2f, 0.2f, 0.2f);
    program.setVec3("spotLight.diffuse",  0.7f, 0.7f, 0.7f);
    program.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    program.setInt("material.diffuse", 0);
    program.setInt("material.specular", 1);
    program.setFloat("material.shininess", 32.0f);
    program.setMat4("proj", proj);
    
    
#define NUM_CUBES 10
    int cubes = NUM_CUBES;
    glm::vec3 cubePositions[20];
    cubePositions[0] = glm::vec3(0.f, 0.f, 0.f);
    cubePositions[1] = glm::vec3(2.f, 5.f, -15.f);
    cubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
    cubePositions[3] = glm::vec3(-3.8f, -2.f, -12.3f);
    cubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
    for (int i = 5; i < NUM_CUBES; ++i)
        cubePositions[i] = glm::vec3( (rand() % 10000)/1000.f - 5.f, (rand() % 10000)/1000.f - 5.f, (rand() % 10000)/1000.f - 5.f );
    
    
    float outputFramerateTimer = 0;
    int outputFramerateCount = 0;
    
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);
        
        std::chrono::high_resolution_clock timer;
        auto start = timer.now();
        
        view = camera.GetViewMatrix();
        program.setMat4("view", view);
        program.setVec3("viewPos", camera.Position);
        program.setVec3("spotLight.position",  camera.Position);
        program.setVec3("spotLight.direction", camera.Front);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glBindVertexArray(light.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, light.VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light.EBO);
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, light.texture);
        for (int i = 0; i < lights; ++i)
        {
            glm::mat4 light_model = glm::mat4(1.f);
            pointLightPositions[i] = glm::vec3(pow(-1, i) * sin(glfwGetTime()) * (i + 1),
                                               pointLightPositions[i].y,
                                               pow(-1, i) * cos(glfwGetTime()) * (i + 1));
            program.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            light_model = glm::translate(light_model, pointLightPositions[i]);
            light_model = glm::scale(light_model, glm::vec3(0.3f, 0.3f, 0.3f));
            program.setMat4("model", light_model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        
        glBindVertexArray(cube.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.EBO);
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, specularMap);
        for (int i = 0; i < cubes; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            program.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        
        
        glfwSwapBuffers(window);
        
        auto stop = timer.now(); outputFramerateCount += 1;
        float deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(stop - start).count();
        outputFramerateTimer += deltaTime;
        if (outputFramerateTimer > 400) {
            glfwSetWindowTitle(window, ("FPS: " + std::to_string(outputFramerateCount/outputFramerateTimer * 1000.f)).c_str());
            outputFramerateCount = 0; outputFramerateTimer = 0.f; }
    }
    
    glDeleteVertexArrays(1, &cube.VAO);
    glDeleteVertexArrays(1, &light.VAO);
    glDeleteBuffers(1, &cube.VBO);
    glDeleteBuffers(1, &light.VBO);

    
    glfwTerminate();
    return 0;
}
