#include "Texture.h"
#include <stb_image/stb_image.h>

Texture::Texture(const char* FilePath)
{

    mLogger = Logger::getInstance();

    initializeOpenGLFunctions();

    //texture loading
    int width, height, nrChannels;

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(FilePath, &width, &height, &nrChannels, 0);
    if (data) {
        stbi_set_flip_vertically_on_load(true);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        mHeight = height;
        mWidth = width;
    }
    else {
         if(mLogger) mLogger->logText("Failed loading image texture", LogType::REALERROR);
    }
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture


    stbi_image_free(data);      //free image memory after bitmap generation
}

Texture::~Texture()
{
    /*delete mLogger;*/
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}
