//
// Created by Henry Fielding on 21/8/2023.
//

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    /* Read in shader code */
    std::string vertexShaderSourceStr = ShaderReader::readShaderToString(vertexPath);
    std::string fragmentShaderSourceStr = ShaderReader::readShaderToString(fragmentPath);

    /* make shader code able to be referenced by a pointer */
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    /* init shader compilation vars */
    unsigned int vertex, fragment;
    int success;
    char infolog[512];

    /* create shader object */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    /* attach shader source code to shader object */
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    /* check for compilation errors */
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    }
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    /* Create shader program to use */
    ID = glCreateProgram();

    /* attach and link shaders to shader program */
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }

    /* no longer need shader objects, delete them */
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setUniformBool(const std::string &name, bool value)  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniformInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniformFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

unsigned int Shader::getID() {
    return ID;
}
