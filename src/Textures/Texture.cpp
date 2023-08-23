//
// Created by Henry Fielding on 23/8/2023.
//

#include "Texture.h"

Texture::Texture(const char* fp) {
    /* load in texture file and set values */
    data = stbi_load(fp, &width, &height, &numChannels, 0);

    /* generate texture id */
    glGenTextures(1, &ID);

    /* bind it so any subsequent GL_TEXTURE_2D commands apply to it */
    glBindTexture(GL_TEXTURE_2D, ID);

    /* configure texture wrapping and filtering options. changes behaviour view docs to see differences */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* ... actually generate the texture */
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "FAILED TO LOAD TEXTURE" << std::endl;
    }

    /* free image data */
    stbi_image_free(data);
}

unsigned int Texture::getID() {
    return ID;
}