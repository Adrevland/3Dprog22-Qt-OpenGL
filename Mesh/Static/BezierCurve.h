#pragma once
#include "Mesh/Mesh.h"
class BezierCurve :
    public Mesh
{
public:

    BezierCurve(Shader* Shader, glm::mat4 modelMat);
    ~BezierCurve();

    void draw() override;
    void init() override;


    void setControlPoints(std::vector<glm::vec3> controlPoints) { mControlPoints = controlPoints; }
    std::vector<glm::vec3> getControlPoints() { return mControlPoints; }
    glm::vec3 calcBezier(float t);

private:

    std::vector<glm::vec3> mControlPoints;

    std::vector<Mesh*> mVisualPoints;
    Mesh* mVisualLines;
};

