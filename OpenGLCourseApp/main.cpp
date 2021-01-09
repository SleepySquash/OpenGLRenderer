//
//  main.cpp
//  OpenGLCourseApp
//
//  Created by Никита Исаенко on 31.12.2020.
//

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::cout;
using std::cerr;
using std::endl;

int ec;
char ecode[512];

void framebufferSizeCallback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, const char * argv[])
{
    if (!glfwInit()) { cout << "glfwInit() error\n"; glfwTerminate(); return 1; }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4 - max
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 1 - max
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Error outdated functionality
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Title", nullptr, nullptr);
    if (!window) { cout << "Error window creating\n"; glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window); // Use this window as OpenGL context
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK) { cout << "glewInit() error\n"; glfwDestroyWindow(window); glfwTerminate(); return 1; }
    
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    
    
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShader_code, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ec);
    if (!ec)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, ecode);
        cout << "glCompileShader(vertexShader) Error: " << ecode << "\n";
        exit(1);
    }
    
    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShader_code, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ec);
    if (!ec)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, ecode);
        cout << "glCompileShader(fragmentShader) Error: " << ecode << "\n";
        exit(1);
    }
    
    // shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ec);
    if (!ec)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, ecode);
        cout << "glLinkProgram(shaderProgram) Error: " << ecode << "\n";
        exit(1);
    }
    
    // layout's location 0 for one vertex is 3 floats (x, y, z) with stride of 0; (stride might be (void*)(3 * sizeof(float)))
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, sin(glfwGetTime()) / 2.0f + 0.5f, 0.f, 1.f);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
