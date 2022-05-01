QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += Core/main.cpp \
    Core/Camera.cpp \
    Levels/InsideHouse.cpp \
    Levels/Level.cpp \
    Levels/Outside.cpp \
    Levels/Oblig3.cpp \
    Math/Mat4.cpp \
    Math/Vec3.cpp \
    Mesh/Static/BezierCurve.cpp \
    Mesh/Static/Door.cpp \
    Mesh/Static/Graph.cpp \
    Mesh/Static/House.cpp \
    Mesh/Static/IdentityCube.cpp \
    Mesh/Mesh.cpp \
    Mesh/ObjLoader.cpp \
    Mesh/Pawn/Player.cpp \
    Mesh/Static/Trophy.cpp \
    CodeGraveYard/Model.cpp \
    CodeGraveYard/Object.cpp \
    Core/OctTree.cpp \
    Physics/BoundingBox.cpp \
    Core/Texture.cpp \
    Math/Vertex.cpp \
    CodeGraveYard/VisualObject/Cube.cpp \
    CodeGraveYard/VisualObject/TriangelSurface.cpp \
    CodeGraveYard/VisualObject/VisualObject.cpp \
    CodeGraveYard/VisualObject/XYZ.cpp \
    Core/logger.cpp \
    Core/mainwindow.cpp \
    Core/renderwindow.cpp \
    Core/shader.cpp \
    stb_image/stb_image.cpp\
    Audio/soundsource.cpp \
    core/soundmanager.cpp \
    Math/vector3.cpp \
    Audio/wavfilereader.cpp\
    Mesh/Static/Heightmap.cpp\
    Mesh/DebugTriangle.cpp\
    Core/Utils.cpp

HEADERS += \
    Core/Camera.h \
    Levels/InsideHouse.h \
    Levels/Level.h \
    Levels/Outside.h \
    Levels/Oblig3.h \
    Math/Mat4.h \
    Math/Vec3.h \
    Mesh/Static/BezierCurve.h \
    Mesh/Static/Door.h \
    Mesh/Static/Graph.h \
    Mesh/Static/House.h \
    Mesh/Static/IdentityCube.h \
    Mesh/Mesh.h \
    Mesh/ObjLoader.h \
    Mesh/Pawn/Player.h \
    Mesh/Static/Trophy.h \
    CodeGraveYard/Model.h \
    CodeGraveYard/Object.h \
    Core/OctTree.h \
    Physics/BoundingBox.h \
    Core/Texture.h \
    Math/Vertex.h \
    CodeGraveYard/VisualObject/Cube.h \
    CodeGraveYard/VisualObject/TriangelSurface.h \
    CodeGraveYard/VisualObject/VisualObject.h \
    CodeGraveYard/VisualObject/XYZ.h \
    Core/logger.h \
    Core/mainwindow.h \
    Core/renderwindow.h \
    Core/shader.h \
    stb_image/stb_image.h\
    Audio/soundsource.h \
    core/soundmanager.h \
    Math/vector3.h \
    Audio/wavfilereader.h\
    Mesh/Static/Heightmap.h\
    Mesh/DebugTriangle.h\
    Core/Utils.h

FORMS += \
    Core\mainwindow.ui

DISTFILES += \
    .gitignore \
    .qmake.stash \
    .txt \
    3Dprog22.pro.user \
    3Dprog22.pro~RF2e165828.TMP \
    3Dprog22.sln \
    3Dprog22.vcxproj \
    3Dprog22.vcxproj.filters \
    3Dprog22.vcxproj.user \
    OpenAL32.dll \
    README.md \
    Shaders/TextureShader.frag \
    Shaders/TextureShader.vert \
    Shaders/WireFrame.frag \
    Shaders/WireFrame.vert \
    Shaders/color.frag \
    Shaders/color.vert \
    Shaders/light.frag \
    Shaders/light.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    TextureShader.frag \
    TextureShader.vert \
    Textures/Drue.png \
    Textures/Rect.png \
    Textures/UvCheckers.jpg \
    Textures/WoodCabinDif.jpg \
    Textures/awesomeface.png \
    Textures/bird.png \
    Textures/cherry.jpg \
    Textures/cherry.png \
    Textures/container.jpg \
    Textures/dirt.jpg \
    Textures/grape.jpg \
    Textures/grass.jpg \
    Textures/ground.jpg \
    Textures/kirby.jpg \
    Textures/skin.jpg \
    Textures/spike.jpg \
    Textures/square.png \
    Textures/star.jpg \
    Textures/tree.jpg \
    Textures/white.jpg \
    Textures/wood.jpg \
    TriangleSurface.txt \
    graph.txt \
    interpolfunc.txt \
    plainshader.frag \
    quadtraticfunc.txt\
    plainshader.vert \
    plainshader.vert


#Set up dependencies for OpenAL
mac {
    LIBS += -framework OpenAL
}




win32 {
        INCLUDEPATH += ./openal/include/AL
        LIBS += $$PWD/openal/libs/OpenAL32.lib
        INCLUDEPATH += ./assimp
        LIBS += $$PWD/DLL_Libs/assimp-vc143-mt.lib

            # Copy required DLL to output directory
        CONFIG(debug, debug|release){
            OpenAL32.commands = copy /Y \"$$PWD\openal\libs\OpenAL32.lib\" debug
            OpenAL32.target = debug/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += debug/OpenAL32.dll

            # Copy required DLL to output directory
            AssImp.commands = copy /Y \"$$PWD\DLL_Libs\assimp-vc143-mt.dll\" debug
            AssImp.target = debug/assimp-vc143-mt.dll

            QMAKE_EXTRA_TARGETS += AssImp
            PRE_TARGETDEPS += debug/assimp-vc143-mt.dll


        } else:CONFIG(release, debug|release) {
            OpenAL32.commands = copy /Y \"$$PWD\DLL_Libs\OpenAL32.dll\" release
            OpenAL32.target = release/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += release/OpenAL32.dll release/OpenAL32.dll

            AssImp.commands = copy /Y \"$$PWD\DLL_Libs\assimp-vc143-mt.dll\" release
            AssImp.target = release/assimp-vc143-mt.dll

            QMAKE_EXTRA_TARGETS += AssImp
            PRE_TARGETDEPS += release/assimp-vc143-mt.dll release/assimp-vc143-mt.dll
        }
}

