#include "SkyBox.h"
#include "Core/logger.h"
#include "Core/renderwindow.h"
#include "stb_image/stb_image.h"

SkyBox::SkyBox(std::vector<std::string> textureFaces)
    :mTextureFaces(textureFaces)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::init()
{
	initializeOpenGLFunctions();

    glGenTextures(1, &CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < mTextureFaces.size(); i++)
    {
        unsigned char* data = stbi_load(mTextureFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            LOG_REALERROR("Cubemap tex failed to load at path" + mTextureFaces[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    mCube = new IdentityCube(RenderWindow::Get()->getShader("skybox"), glm::mat4{1.f});
    mCube->initskybox();
}

void SkyBox::draw()
{
    mCube->drawskybox(CubemapTexture);

}
