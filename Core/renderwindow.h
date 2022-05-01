#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QElapsedTimer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QWindow>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Camera.h"
#include "logger.h"
#include "Mesh/Mesh.h"
#include "Levels/Level.h"
#include "Mesh/Static/DebugShapes.h"

class QOpenGLContext;
class Shader;
class MainWindow;

#define RENDERWINDOW RenderWindow::Get()
#define LEVEL RenderWindow::Get()->getLevel()

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    bool mRotate{false};     //Check if triangle should rotate
    bool mWireFrame{ false };
    void showWireFrame(bool show);
    void changeViewMode(); // swap how shader render
    void changeCameraMode(); // swap cameratype
    void setPlayerspeed(float speed);
    void toggleDebugLines();
    void drawDebugShape(std::string shape, glm::mat4 modelmat);

   static RenderWindow* Get() { return sRenderWindow; }
   void log(std::string message, LogType logtype);
   
   void changeScene(int scene);
   Shader* getShader(std::string shader) { return mShaderProgram[shader]; }
   Camera* getCamera() { return mCamera; }
   double deltatime() { return Deltatime; }
   
   void destroy(Mesh* mesh);

   glm::vec3 mLightDir{ glm::normalize(glm::vec3(0.1f,0.2f,0.3f))};
   //void setLightDir(glm::vec3 lightdir) { mLightDir = glm::normalize(lightdir); }

   Level* getLevel() { return mLevels[mActiveLevel]; };
private slots:
    void render();          //the actual render - function

private:
    void init();            //initialize things we need before rendering

    int mActiveLevel{0};

    Camera* mCamera{nullptr};

	inline static RenderWindow* sRenderWindow{ nullptr };

    std::unordered_map<std::string, Shader*> mShaderProgram;
    std::unordered_map<std::string, DebugShapes*> mDebugShapes;

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};


    std::vector<Level*> mLevels;

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
	Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

    //movement
    QPointF mCurrentMousePos{0,0};
    QPointF mLastMousePos{0,0};

    double Deltatime{0};
    double LastFrame{0};

    unsigned long long FramesRendered;
protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;              //the only one we use now
    void keyReleaseEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
};

#endif // RENDERWINDOW_H
