#pragma once
#include "Level.h"
class InsideHouse :
    public Level
{
public:
	InsideHouse(RenderWindow* rWindow);
	~InsideHouse();

	void init() override;
	void render() override;

	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void exposeEvent() override;

	void resetPlayer() override;

protected:
};

