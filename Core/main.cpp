#include "mainwindow.h"
#include <QApplication>
#include "soundmanager.h"
#include <iostream>
#include <chrono>   //for sleep_for
#include <thread>   //for sleep_for

//This is just a simple testprogram for the SoundManager.

// ************* NB!!! ********************
// You have to take parts of this into your own OpenGL project!!!
// This branch only shows simple use of these classes

// wavefilereader - reads a wav-file (sound file)
// soundsource -    sets up and positions a sound
// soundmanager -   sets up the sound card and OpenAL, and owns the Listener

// You probably want to refactor these classes -
// they are example classes, not very robust written

//oppgave 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
int main(int argc, char* argv[])
{
    //Forces the usage of desktop OpenGL - Qt uses OpenGL ES as default
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes a Qt application
    QApplication a(argc, argv);

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    return a.exec();

}
