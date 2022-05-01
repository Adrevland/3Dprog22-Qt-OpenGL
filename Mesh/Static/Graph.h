#pragma once
#include "Mesh/Mesh.h"
#include <glm/glm.hpp>

#include <cmath>

#include "Core/Camera.h"
#include "Core/shader.h"
#include "Math/Vertex.h"

enum Function
{
	Quadtratic = 0,
    Interpol = 1
};

class Graph :
    public Mesh
{
public:
    Graph(Shader* Shader,  glm::mat4 modelMat, const char* ObjFile, const char* texture);
    ~Graph();

    void init() override;
    void draw() override;
    void ButtonPress(int Button);
    void sendTime(unsigned long long elapsedTime) override;

    void writeFile(float func(float x), std::string filename);
    std::vector<Vertex> readFile(std::string filename, int vecloc);

    void swapGraph(Function mathFunc);

    static float quadtratic(float x)
    {
        //return (-55.f / 168.f) * powf(x, 2) + (21.f / 8.f) * x - (47.f / 56.f);
        return  (-20366086359.f/1064524583410.f)* powf(x, 2) + (25858222907.f / 106452458341.f) * x + (333794591171.f / 1064524583410.f); // based on trophy points
            
    }
    static float interpol(float x)
    {
        //return (-181.f / 840.f) * powf(x, 3) + (841.f / 280.f) * powf(x, 2) - (4687.f / 420.f) * x + 398.f / 35;

        return (1773.f / 913880.f) * powf(x, 3) + (16299.f / 913880.f) * powf(x, 2) + (-369561.f / 182776.f) * x + (856665.f / 182776.f);

    }

    void updateNpc(int xloc);

    //Mesh* getNpc() { return mFollowNpc; };

protected:

private:

    Mesh* QuadtraticGraph{ nullptr };
    Mesh* InterpolGraph{ nullptr };

    int mVertCount{ 0 };

    int mActiveGraph{Quadtratic};

    glm::vec3 npcLoc{0.f};
    float mNpcSpeed{0.1f};

    //for graph
    const float startValue{ -45 };
    const float endValue{ 45 };
};
