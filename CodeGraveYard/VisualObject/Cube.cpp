#include "Cube.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include <stb_image/stb_image.h>

Cube::Cube(Shader* shader)
	:VisualObject(shader)
{

    mVertices.emplace_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));

    mVertices.emplace_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));

    mVertices.emplace_back(Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
    mVertices.emplace_back(Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f));

    mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f));

    mVertices.emplace_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
    mVertices.emplace_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f));

    mVertices.emplace_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f));
    mVertices.emplace_back(Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f));
    mVertices.emplace_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f));


}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
}

void Cube::init(Camera& camera)
{
	VisualObject::init(camera);

    mModelMat = glm::rotate(mModelMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //model matrix

    texture = new Texture("./Textures/container.jpg");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::draw()
{
    texture->bind();

    VisualObject::draw();
    /*glBindTexture(GL_TEXTURE_2D, texture1);*/
    
    //draw
    //glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
