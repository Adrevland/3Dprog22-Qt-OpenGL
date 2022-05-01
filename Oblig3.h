#pragma once
#include "Levels/Level.h"

class Oblig3 :
    public Level
{
public:
	Oblig3(RenderWindow* rWindow);
	~Oblig3();

	void init() override;
	void render() override;

	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void exposeEvent() override;
	void deleteMesh(Mesh* mesh) override;

};

