#include "renderwindow.h"
#include <QDebug>
#include <QKeyEvent>
//#include <QMatrix4x4>
#include <QDateTime>
#include <QOpenGLContext>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QStatusBar>
#include <QTimer>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "logger.h"
#include "mainwindow.h"
#include "Oblig3.h"
#include "shader.h"
#include "Levels/HeightmapLevel.h"
#include "Levels/InsideHouse.h"
#include "Levels/Outside.h"
#include "stb_image/stb_image.h"
#include "Levels/EksamenLevel.h"


RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //delete all the pointers
    /*delete(mContext);
    delete(mCamera);
    delete(mShaderProgram);
    delete(mMainWindow);
    delete(mLogger);
    delete(mRenderTimer);*/
    //mObjects.clear();

    //cleans up the GPU memory
    /*glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
    glDeleteBuffers(1, &mEBO);*/
}


// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

   

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));

    sRenderWindow = this;

	//********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE); // AA
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(21.f/255, 17.f/255.f, 51.f/255.f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT


    mCamera = new Camera(width(), height(), devicePixelRatio());

    ////math test
    ////auto p = glm::vec2(-1.f / 4.f, 1.f / 2.f);
    //auto p = glm::vec2(1.f, 1.f);
    //auto a = glm::vec2(-1.f, 0.f);
    //auto b = glm::vec2(1.f / 2.f, 1.f / 2.f);
    //auto c = glm::vec2(-1.f / 2.f, 1.f);
    //auto ans = Utils::Barycentric(p, a, b,c );
    

    mShaderProgram["Texture"] = new Shader("../3Dprog22/Shaders/TextureShader.vert", "../3Dprog22/Shaders/TextureShader.frag");
    mShaderProgram["light"] = new Shader("../3Dprog22/Shaders/light.vert", "../3Dprog22/Shaders/light.frag");
    mShaderProgram["color"] = new Shader("../3Dprog22/Shaders/color.vert", "../3Dprog22/Shaders/color.frag");
    mShaderProgram["debug"] = new Shader("../3Dprog22/Shaders/plainshader.vert", "../3Dprog22/Shaders/plainshader.frag");
    mShaderProgram["skybox"] = new Shader("../3Dprog22/Shaders/Skybox.vert", "../3Dprog22/Shaders/Skybox.frag");
    mShaderProgram["shadow"] = new Shader("../3Dprog22/Shaders/Shadow.vert", "../3Dprog22/Shaders/Shadow.frag", "../3Dprog22/Shaders/Shadow.geom");
    mShaderProgram["lightshadow"] = new Shader("../3Dprog22/Shaders/lightshadow.vert", "../3Dprog22/Shaders/lightshadow.frag");
    mShaderProgram["billboard"] = new Shader("../3Dprog22/Shaders/Billboard.vert", "../3Dprog22/Shaders/Billboard.frag");

	//oppgave 6
	//Make debug Shapes
    std::vector<Vertex> xyzvert;
    xyzvert.emplace_back(Vertex(0.f,0.f,0.f,
								1.f,0.f,0.f,
								0.f,0.f)); // x red
    xyzvert.emplace_back(Vertex(1.f, 0.f, 0.f,
						        1.f, 0.f, 0.f,
						        0.f, 0.f)); // x red 
    xyzvert.emplace_back(Vertex(0.f, 0.f, 0.f,
						        0.f, 1.f, 0.f,
						        0.f, 0.f)); // y green
    xyzvert.emplace_back(Vertex(0.f, 1.f, 0.f,
						        0.f, 1.f, 0.f,
						        0.f, 0.f)); // y green
    xyzvert.emplace_back(Vertex(0.f, 0.f, 0.f,
						        0.f, 0.f, 1.f,
						        0.f, 0.f)); // z blue
    xyzvert.emplace_back(Vertex(0.f, 0.f, 1.f,
						        0.f, 0.f, 1.f,
						        0.f, 0.f)); // z blue
    std::vector<unsigned int> xyzindices;
    mDebugShapes["xyz"] = new DebugShapes(xyzvert, xyzindices);
    ObjLoader* cube = new ObjLoader("../3Dprog22/ObjFiles/IdentityCube.obj");
    cube->init();
    mDebugShapes["cube"] = new DebugShapes(cube->getVertices(),cube->getIndices());
    ObjLoader* sphere = new ObjLoader("../3Dprog22/ObjFiles/Sphere.obj");
    sphere->init();
    mDebugShapes["sphere"] = new DebugShapes(sphere->getVertices(), sphere->getIndices());
    
    for (auto& mesh: mDebugShapes)
    {
        mesh.second->init();
    }
    /*mLevels.emplace_back(std::make_unique<Outside>(this));
    mLevels.emplace_back(std::make_unique<InsideHouse>(this));
    mLevels.emplace_back(std::make_unique<HeightmapLevel> (this));*/
    //mLevels.emplace_back(new Outside(this));
    //mLevels.emplace_back(new Oblig3(this));
    mLevels.emplace_back(new EksamenLevel());
    //mLevels.emplace_back(new InsideHouse(this));
    //mLevels.emplace_back(new HeightmapLevel(this));
    for (auto& mLevel : mLevels)
    {
	    mLevel->init();
    }

    glBindVertexArray(0);       //unbinds any VertexArray - good practice

    mCamera->rotate(0, 0);

}

void RenderWindow::showWireFrame(bool show)
{
    if (show)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

void RenderWindow::changeViewMode()
{
    //todo send data to shader, change between light and show normals
    mLevels[mActiveLevel]->setViewMode();

}

void RenderWindow::changeCameraMode()
{
    mLevels[mActiveLevel]->setCameramode();
}

void RenderWindow::setPlayerspeed(float speed )
{
    mLevels[mActiveLevel]->setPlayerSpeed(speed);
}

void RenderWindow::toggleDebugLines()
{
    mLevels[mActiveLevel]->setDebugLines();
}

void RenderWindow::drawDebugShape(std::string shape, glm::mat4 modelmat)
{
    if(shape == "xyz")
        glClear(GL_DEPTH_BUFFER_BIT); // clear debpth buffer to make xyz in front of everything
    mDebugShapes[shape]->draw(modelmat);
}

void RenderWindow::log(std::string message, LogType logtype)
{
    mLogger->logText(message, logtype);
}

void RenderWindow::changeScene(int scene)
{
    //reset input
    std::unordered_map<int, bool> empty;
    mLevels[mActiveLevel]->keyBoardinput(empty);

    if (mActiveLevel + scene < mLevels.size())
        mActiveLevel += scene;
    else 
        mActiveLevel = 0;

    mLevels[mActiveLevel]->resetPlayer();
    
}


void RenderWindow::destroy(Mesh* mesh)
{
    mLevels[mActiveLevel]->deleteMesh(mesh);
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)


    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*glDisable(GL_CULL_FACE);*/

    mLevels[mActiveLevel]->render();

    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();


    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);

    calculateFramerate();
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();
    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    mLevels[mActiveLevel]->exposeEvent();
    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(2);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0}; //counting actual frames for a quick "timer" for the statusbar
   
    
    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }

}


//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}


void RenderWindow::mousePressEvent(QMouseEvent* event)
{
    mLevels[mActiveLevel]->mousePressEvent(event);

	QWindow::mousePressEvent(event);
}

void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
    mLevels[mActiveLevel]->mouseReleaseEvent(event);

	QWindow::mouseReleaseEvent(event);
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    mCurrentMousePos = event->globalPosition();

    mLevels[mActiveLevel]->mouseMoveEvent(event);

	QWindow::mouseMoveEvent(event);
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }

    mLevels[mActiveLevel]->keyPressEvent(event);

    QWindow::keyReleaseEvent(event);

    
}

void RenderWindow::keyReleaseEvent(QKeyEvent* event)
{

    mLevels[mActiveLevel]->keyReleaseEvent(event);

	QWindow::keyReleaseEvent(event);
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{

    mLevels[mActiveLevel]->wheelEvent(event);

	QWindow::wheelEvent(event);
}
