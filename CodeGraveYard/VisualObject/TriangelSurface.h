#pragma once
#include "VisualObject.h"
#include <cmath>
#include "core/texture.h"

class TriangelSurface :
    public VisualObject
{
public:
    TriangelSurface(Shader* shader);
    ~TriangelSurface();

    void init(Camera& camera) override;
    void draw() override;

    void writeFile(float func(float x, float y), std::string filename);
    void readFile(std::string filename);



    static float mFunc(float x, float y)
    {
        static const double PI{ 3.14159265358979323846 };
        return 2 * sin(PI * x * 0.1f) * sin(PI * y * 0.1f);
        //return 2.f*cos(x*0.2f) * pow(2,sin(y*0.2f));

    }
protected:

private:
    Texture* texture;
    int mVertCount{0};
};

