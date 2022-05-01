#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <vector>

#include "math/Vertex.h"
#include "VisualObject.h"

class XYZ : public VisualObject
{
public:
	XYZ(Shader* shader);
	~XYZ();
	void init(Camera& camera) override;
	void draw() override;
private:

	QOpenGLContext* mContext;

};

