//
//  Shader.hpp
//  OpenGLCourseApp
//
//  Created by Никита Исаенко on 02.01.2021.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct ShaderProgram
{
    unsigned int ID;
    
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, const float& x, const float& y, const float& z) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
};

#endif /* Shader_hpp */
