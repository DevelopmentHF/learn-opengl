//
// Created by Henry Fielding on 20/8/2023.
//

#ifndef LEARN_OPENGL_SHADERREADER_H
#define LEARN_OPENGL_SHADERREADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class ShaderReader {
public:
    /* Ports a .glsl file into a c-style string */
    static std::string readShaderToString(const char* filePath);
};


#endif //LEARN_OPENGL_SHADERREADER_H
