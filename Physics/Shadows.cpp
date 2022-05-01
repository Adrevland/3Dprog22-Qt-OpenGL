#include "Shadows.h"
#include <glm/matrix.hpp>
#include "Core/renderwindow.h"
#include "Mesh/Mesh.h"

Shadows::Shadows()
{
}

Shadows::~Shadows()
{
}

std::vector<glm::vec4> Shadows::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt =
                    inv * glm::vec4(
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}

glm::mat4 Shadows::calcLightView()
{
    mLightDir = RENDERWINDOW->mLightDir;

    glm::vec3 center{ 0.f };

    auto corners = getFrustumCornersWorldSpace(RenderWindow::Get()->getCamera()->getProjection(), RenderWindow::Get()->getCamera()->getView());

    for(const auto& v: corners)
    {
        center += glm::vec3(v);
    }
    center /= corners.size();

    mlightview =  glm::mat4{glm::lookAt(center+ mLightDir, center, glm::vec3(0.f,0.f,1.f))};

    return mlightview;
}

glm::mat4 Shadows::calcLightViewProj( float nearPlane,  float farPlane)
{
    const auto proj = glm::perspective(
        glm::radians(RenderWindow::Get()->getCamera()->getfov()), (float)RenderWindow::Get()->width() / (float)RenderWindow::Get()->height() , nearPlane,
        farPlane);
    const auto corners = getFrustumCornersWorldSpace(proj, RenderWindow::Get()->getCamera()->getView());

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : corners)
    {
        center += glm::vec3(v);
    }
    center /= corners.size();

    const auto lightView = calcLightView();

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();
    for (const auto& v : corners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);

        /*glm::mat4 loc = glm::translate(glm::mat4{ 1.f }, glm::vec3(v));
        loc = glm::scale(loc, glm::vec3(10.f));
        RenderWindow::Get()->drawDebugShape("sphere", loc);*/
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 5.f;
    
    /*if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }*/

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ-1000.f, maxZ+1000.f);

    return lightProjection * lightView;


}

void Shadows::genShadowMaps()
{

    initializeOpenGLFunctions();

    glActiveTexture(GL_TEXTURE1);
    glGenFramebuffers(1, &lightFBO);

    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 4, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenTextures(1, &lightDepthMaps);
    glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);

    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        GL_DEPTH_COMPONENT32F,
        depthMapResolution,
        depthMapResolution,
        int(shadowCascadeLevels.size()),
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

    glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lightDepthMaps, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_REALERROR("ERROR::FRAMEBUFFER::frambuffer is not complete");
        throw 0; // kinda sus;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Shadows::bindShadowMaps()
{
    //bind texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
}

void Shadows::draw(std::vector<Mesh*> meshes)
{
    shadowCascades.clear();
    for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
    {
        if (i == 0)
        {
            shadowCascades.push_back(calcLightViewProj(RenderWindow::Get()->getCamera()->getnear(), shadowCascadeLevels[i]));
        }
        else if (i < shadowCascadeLevels.size())
        {
            shadowCascades.push_back(calcLightViewProj(shadowCascadeLevels[i-1], shadowCascadeLevels[i]));
        }
        else
        {
            shadowCascades.push_back(calcLightViewProj(shadowCascadeLevels[i-1], RenderWindow::Get()->getCamera()->getfar()));
        }
    }

    RenderWindow::Get()->getShader("shadow")->use();

    bindShadowMaps();

    //glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, lightDepthMaps, 0); //gives error

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);

    glViewport(0, 0, depthMapResolution, depthMapResolution);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

	glBufferSubData(GL_UNIFORM_BUFFER,  0, sizeof(glm::mat4x4) * shadowCascades.size(), shadowCascades.data());

    //draws whole scene using shadow geometry shader
    for(auto& mesh :meshes)
    {
        mesh->drawShadowShader();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, static_cast<GLint>(RenderWindow::Get()->width() * RenderWindow::Get()->devicePixelRatio()), static_cast<GLint>(RenderWindow::Get()->height() * RenderWindow::Get()->devicePixelRatio()));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
