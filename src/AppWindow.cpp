#include "AppWindow.hpp"

#include <iostream>
#include <QtWidgets>

#include "Scene.hpp"
#include "scene_lua.hpp"

using namespace std;

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    viewer = new Viewer(glFormat, this);
    layout->addWidget(viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
}

AppWindow::~AppWindow() {
    if (scene != NULL) {
        delete scene;
    }
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector 
    QAction* quitAct = new QAction(tr("&Quit"), this);
    menuActions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void AppWindow::createMenu() {
    applicationMenu = menuBar()->addMenu(tr("&Application"));

    for (auto& action : menuActions) {
        applicationMenu->addAction(action);
    }
}

void AppWindow::loadScene(const string& path) {
    // get rid of the old scene
    if (scene != NULL) {
        delete scene;
    }

    scene = import_lua(path);
    viewer->setScene(scene);

    if (scene == NULL) {
        cerr << "AppWindow::loadScene - Could not open " << path << endl;
    }
}
