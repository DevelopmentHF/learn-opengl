//
// Created by Henry Fielding on 20/8/2023.
//

#include "ShaderReader.h"

std::string ShaderReader::readShaderToString(const char *filePath) {
    // open the file
    std::ifstream shaderFile(filePath);

    // check file exists and can be opened
    if (!shaderFile.is_open()) {
        // std::cerr << "Current path: " << std::filesystem::current_path();
        std::cerr << "Failed to open shader: " << filePath << std::endl;
        return "";
    }

    // read in data
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    std::string shaderCode = shaderStream.str();
    shaderCode += '\0';

    return shaderCode;
}
