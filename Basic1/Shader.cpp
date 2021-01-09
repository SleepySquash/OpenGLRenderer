//
//  Shader.cpp
//  OpenGLCourseApp
//
//  Created by Никита Исаенко on 02.01.2021.
//

#include "Shader.hpp"
#include "../Basic/ResourcePath.hpp"
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    std::string vCode, fCode;
    std::ifstream vFile(resourcePath() + vertexPath),
                  fFile(resourcePath() + fragmentPath);
    
    if (vFile.is_open() && fFile.is_open())
    {
        std::stringstream vStream, fStream;
        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();
        vCode = vStream.str();
        fCode = fStream.str();
        
        const char* _vCode = vCode.c_str();
        const char* _fCode = fCode.c_str();

        GLuint vertex, fragment;
        int ec; char ecode[512];
        
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &_vCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &ec);
        if (!ec)
        {
            glGetShaderInfoLog(vertex, 512, NULL, ecode);
            std::cout << "glCompileShader(vertex) Error: " << ecode << "\n";
        }
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &_fCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &ec);
        if (!ec)
        {
            glGetShaderInfoLog(fragment, 512, NULL, ecode);
            std::cout << "glCompileShader(fragment) Error: " << ecode << "\n";
        }
        
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &ec);
        if (!ec)
        {
            glGetProgramInfoLog(ID, 512, NULL, ecode);
            std::cout << "glLinkProgram() Error(s):\n" << ecode << "\n";
        }
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    else std::cout << "ShaderProgram::CTOR: file does not exists\n";
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value); }
void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z); }
void ShaderProgram::setVec3(const std::string& name, const float& x, const float& y, const float& z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); }
void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(value));
}
