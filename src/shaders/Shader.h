//
// Created by Henry Fielding on 21/8/2023.
//

#ifndef LEARN_OPENGL_SHADER_H
#define LEARN_OPENGL_SHADER_H

#include <string>
#include <glad/glad.h>
#include "ShaderReader.h"

class Shader {
private:
    // the program id
    unsigned int ID;
public:
    // read and build the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

    // retrieve program id
    unsigned int getID();
};


#endif //LEARN_OPENGL_SHADER_H
