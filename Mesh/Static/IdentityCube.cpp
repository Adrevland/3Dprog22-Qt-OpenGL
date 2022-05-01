#include "IdentityCube.h"
#include "Mesh/ObjLoader.h"
#include <glm/gtc/type_ptr.inl>

IdentityCube::IdentityCube(Shader* Shader, glm::mat4 modelMat)
	:Mesh(Shader,modelMat)
{
    
    //identity cube
    mVertices.emplace_back(Vertex(-1.f, 1.f, -1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(1.f, 1.f, -1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(1.f, 1.f, 1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(-1.f, 1.f, 1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(-1.f, -1.f, -1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(1.f, -1.f, -1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(1.f, -1.f, 1.f, color.r, color.g, color.b));
    mVertices.emplace_back(Vertex(-1.f, -1.f, 1.f, color.r, color.g, color.b));

    //indices
    const GLint indices[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        4, 5,
        5, 6,
        6, 7,
        7, 4,

        0, 4,
        1, 5,
        2, 6,
        3, 7,
    };
    for(auto i : indices)
    {
        mIndices.emplace_back(i);
    }
    mDrawMode = GL_LINES;
}

IdentityCube::~IdentityCube()
{
}

void IdentityCube::draw()
{
    Mesh::draw();
 
}

void IdentityCube::initskybox()
{
    ObjLoader* obj = new ObjLoader("../3Dprog22/ObjFiles/identitycube.obj");
    obj->init();
    mVertices = obj->getVertices();
    mIndices = obj->getIndices();
    mModelMat = glm::scale(glm::mat4{ 1.f }, glm::vec3{ 0.5f });
    Mesh::init();
}

void IdentityCube::drawskybox(unsigned int Textureid)
{
    glDisable(GL_CULL_FACE);
    
    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Textureid);
    
    mShaderProgram->use();

    glBindVertexArray(mVAO);
    /*glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);*/

	//send matrix to shader
	glm::mat4 view = glm::mat4(glm::mat3(mCamera->getView()));
	glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getProjection()));
    glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(mModelMat));
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
	glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
}

void IdentityCube::init()
{
	Mesh::init();
}
