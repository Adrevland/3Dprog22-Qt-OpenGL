#pragma once
#include "Level.h"
class Graph;

class Outside :
    public Level
{
public:
	Outside(RenderWindow* rWindow);
	~Outside();

	 void init() override;
	 void render() override;

	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void exposeEvent() override;
	void deleteMesh(Mesh* mesh) override;
protected:


private:
	Graph* mGraph;
};

