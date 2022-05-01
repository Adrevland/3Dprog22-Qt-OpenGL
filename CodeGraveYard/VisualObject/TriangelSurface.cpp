#include "TriangelSurface.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stb_image/stb_image.h>
#include <algorithm>

TriangelSurface::TriangelSurface(Shader* shader)
	:VisualObject(shader)
{
    
}

TriangelSurface::~TriangelSurface()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

void TriangelSurface::init(Camera& camera)
{
    writeFile(mFunc, "./TriangleSurface.txt");
    readFile("./TriangleSurface.txt");

	VisualObject::init(camera);
    mModelMat = glm::mat4(1.f);

    texture = new Texture("./Textures/dirt.jpg");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TriangelSurface::draw()
{
	VisualObject::draw();

    texture->bind();

    //only draw mesh
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}


void TriangelSurface::writeFile(float func(float x, float y), std::string filename)
{
    std::ofstream File(filename);

    float size{ 100 };
    float h{ 4.0f };

    File << size * size * 6 << "\n"; // line count

    //rgb colors

    //with clamped colors
    //for (float ix = -(size / 2.f); ix < (size / 2.f); ix += h)
    //{
    //    for (float iy = -(size / 2.f); iy < (size / 2.f); iy += h)
    //    {

    //        float r = std::clamp(ix, 0.f, 1.f);
    //        float g = std::clamp(iy, 0.f, 1.f);

    //        float z = func(ix, iy);
    //        float b = std::clamp(z, 0.05f, 1.f); // 0.05 since i dont want black
    //        File << (Vertex{ ix,iy,z,r,g,b,0.f,1.f }); // down left
    //        z = func(ix + h, iy);
    //        b = std::clamp(z, 0.05f, 1.f); // 0.05 since i dont want black
    //        File << (Vertex{ ix + h,iy,z,r,g,b,1.f,1.f }); // down right
    //        z = func(ix, iy + h);
    //        b = std::clamp(z, 0.05f, 1.f); // 0.05 since i dont want black
    //        File << (Vertex{ ix,iy + h,z,r,g,b,0.f,0.f }); // left up

    //        File << (Vertex{ ix,iy + h,z,r,g,b,0.f,0.f }); // left up
    //        z = func(ix + h, iy);
    //        b = std::clamp(z, 0.05f, 1.f); // 0.05 since i dont want black
    //        File << (Vertex{ ix + h,iy,z,r,g,b, 1.f,1.f}); // right down
    //        z = func(ix + h, iy + h);
    //        b = std::clamp(z, 0.05f, 1.f); // 0.05 since i dont want black
    //        File << (Vertex{ ix + h,iy + h,z,r,g,b, 1.f,0.f });  // right up
    //    }
    //}

    //white color
    for (float ix = -(size / 2.f); ix < (size / 2.f); ix += h)
    {
        for (float iy = -(size / 2.f); iy < (size / 2.f); iy += h)
        {

            float r = 1;
        	float g = 1;
            float b = 1;
            float z = func(ix, iy);
            
            File << (Vertex{ ix,iy,z,r,g,b,0.f,1.f }); // down left
            z = func(ix + h, iy);
           
            File << (Vertex{ ix + h,iy,z,r,g,b,1.f,1.f }); // down right
            z = func(ix, iy + h);
            
            File << (Vertex{ ix,iy + h,z,r,g,b,0.f,0.f }); // left up

            File << (Vertex{ ix,iy + h,z,r,g,b,0.f,0.f }); // left up
            z = func(ix + h, iy);
            
            File << (Vertex{ ix + h,iy,z,r,g,b, 1.f,1.f }); // right down
            z = func(ix + h, iy + h);

            File << (Vertex{ ix + h,iy + h,z,r,g,b, 1.f,0.f });  // right up
        }
    }
    File.close();
}

void TriangelSurface::readFile(std::string filename)
{
    std::ifstream file(filename);
    std::string line;

    if(file >> line)
    mVertCount = std::stoi(line);
    float numb;

    for (int j = 0; j < mVertCount; ++j)
    {
        float vertdata[8];
        for (int i = 0; i < 8; ++i)
        {
            file >> line;
            vertdata[i] = std::stof(line);
        }
        mVertices.emplace_back(Vertex(vertdata[0],vertdata[1], vertdata[2], vertdata[3], vertdata[4], vertdata[5], vertdata[6], vertdata[7]));
    }

    file.close();
}
