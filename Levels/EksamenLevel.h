#pragma once
#include "Level.h"
class EksamenLevel :
    public Level
{
public:

    EksamenLevel();
    ~EksamenLevel();

    void init() override;
    void render()override;

    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void exposeEvent() override;
    void deleteMesh(Mesh* mesh) override;

protected:
    float daylightcyclespeed{ 0.1f };
};

