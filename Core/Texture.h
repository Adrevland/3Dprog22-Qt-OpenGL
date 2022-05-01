#pragma once
#include <QOpenGLFunctions_4_1_Core>

#include "logger.h"

class Texture
	: public QOpenGLFunctions_4_1_Core
{
public:
	Texture(const char* FilePath);
	~Texture();

	void bind();

	unsigned int mWidth{ 1 };
	unsigned int mHeight{ 1 };

private:
	Logger* mLogger{ nullptr };
	//texture
	unsigned int mTexture;
};

