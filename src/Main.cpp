#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <string>

#include "AppWindow.hpp"
#include "scene_lua.hpp"

using namespace std;

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    AppWindow window;
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();

    int widgetArea = window.width() * window.height();

    if (((float) widgetArea / (float) desktopArea) < 0.75f) {
        window.show();
    } else {
        window.showMaximized();
    }

    string filename = "puppet.lua";
    if (argc >= 2) {
        filename = argv[1];
    }

    window.loadScene(filename);

    return app.exec();
}

