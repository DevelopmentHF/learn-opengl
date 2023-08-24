//
// Created by Henry Fielding on 23/8/2023.
//

#ifndef LEARN_OPENGL_TEXTURE_H
#define LEARN_OPENGL_TEXTURE_H

#include "../ext/stb_image.h"
#include <glad/glad.h>
#include <iostream>


class Texture {
private:
    /* texture dimensions */
    int width, height;
    /* number of colour channels to use for the texture */
    int numChannels;
    /* texture data */
    unsigned char* data;
    /* texture id */
    GLuint ID;
public:
    /* build texture */
    explicit Texture(const char* fp, GLint format);
    /* get the texture's id */
    unsigned int getID();
};


#endif //LEARN_OPENGL_TEXTURE_H
