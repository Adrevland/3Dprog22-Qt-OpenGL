#pragma once
#include <QOpenGLFunctions_4_1_Core>

#include "IdentityCube.h"

class SkyBox: public QOpenGLFunctions_4_1_Core
{
public:
	SkyBox(std::vector<std::string> textureFaces);

	~SkyBox();

	void init();
	void draw();

	IdentityCube* mCube{nullptr};

private:
	unsigned int CubemapTexture;
	std::vector<std::string> mTextureFaces;
};

