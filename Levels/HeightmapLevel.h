#pragma once
#include "Level.h"
class HeightmapLevel :
    public Level
{
public:

    HeightmapLevel(RenderWindow* rWindow);
    ~HeightmapLevel();

	void init() override;
	void render() override;

	void deleteMesh(Mesh* mesh) override;
    void wheelEvent(QWheelEvent* event)override;
    void keyPressEvent(QKeyEvent* event)override;
    void keyReleaseEvent(QKeyEvent* event)override;
	void exposeEvent()override;
    void resetPlayer()override;



private:

protected:
};

