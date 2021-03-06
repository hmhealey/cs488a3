/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#include "AppWindow.hpp"

#include <iostream>
#include <QtWidgets>

#include "Scene.hpp"
#include "scene_lua.hpp"

using namespace std;

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two (Actually Three, but the donated code still says Two)");

    // set up window layout
    QVBoxLayout *layout = new QVBoxLayout;
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    // set up OpenGL
    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    // set up Viewer
    viewer = new Viewer(glFormat, this);
    layout->addWidget(viewer);

    createMenu();
}

AppWindow::~AppWindow() {
    if (scene != NULL) {
        delete scene;
    }
}

void AppWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
#ifndef __APPLE__
    // shortcuts on Ubuntu only work if their respective menu is actually visible, so handle it manually
    } else if (event->key() == Qt::Key_I) {
        viewer->resetPosition();
    } else if (event->key() == Qt::Key_O) {
        viewer->resetOrientation();
    } else if (event->key() == Qt::Key_N) {
        doResetJoints();
    } else if (event->key() == Qt::Key_A) {
        doResetAll();
    } else if (event->key() == Qt::Key_Q) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_P) {
        viewer->setInputMode(Viewer::Puppet);
        puppet->setChecked(true);
        joints->setChecked(false);
    } else if (event->key() == Qt::Key_J) {
        viewer->setInputMode(Viewer::Joints);
        puppet->setChecked(false);
        joints->setChecked(true);
    } else if (event->key() == Qt::Key_U) {
        doUndo();
    } else if (event->key() == Qt::Key_R) {
        doRedo();
    } else if (event->key() == Qt::Key_C) {
        viewer->trackballVisible.toggle();
    } else if (event->key() == Qt::Key_Z) {
        viewer->depthBufferEnabled.toggle();
    } else if (event->key() == Qt::Key_B) {
        viewer->backfaceCullingEnabled.toggle();
    } else if (event->key() == Qt::Key_F) {
        viewer->frontfaceCullingEnabled.toggle();
    } else if (event->key() == Qt::Key_D) {
        viewer->drawPickingBufferEnabled.toggle();
#endif
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

void AppWindow::createMenu() {
    applicationMenu = menuBar()->addMenu(tr("&Application"));
    {
        QAction* resetPosition = new QAction(tr("Reset Pos&ition"), this);
        resetPosition->setStatusTip(tr("Resets the position of the puppet"));
        resetPosition->setShortcuts(QList<QKeySequence>({ Qt::Key_I, Qt::SHIFT + Qt::Key_I }));
        connect(resetPosition, &QAction::triggered, [=] { viewer->resetPosition(); });
        applicationMenu->addAction(resetPosition);

        QAction* resetOrientation = new QAction(tr("Reset &Orientation"), this);
        resetOrientation->setStatusTip(tr("Resets the orientation of the puppet"));
        resetOrientation->setShortcuts(QList<QKeySequence>({ Qt::Key_O, Qt::SHIFT + Qt::Key_O }));
        connect(resetOrientation, &QAction::triggered, [=] { viewer->resetOrientation(); });
        applicationMenu->addAction(resetOrientation);

        QAction* resetJoints = new QAction(tr("Reset Joi&nts"), this);
        resetJoints->setStatusTip(tr("Resets the joints of the puppet"));
        resetJoints->setShortcuts(QList<QKeySequence>({ Qt::Key_N, Qt::SHIFT + Qt::Key_N }));
        connect(resetJoints, &QAction::triggered, [=] { doResetJoints(); });
        applicationMenu->addAction(resetJoints);

        QAction* resetAll = new QAction(tr("Reset &All"), this);
        resetAll->setStatusTip(tr("Resets the position, orientation, and joints of the puppet"));
        resetAll->setShortcuts(QList<QKeySequence>({ Qt::Key_A, Qt::SHIFT + Qt::Key_A }));
        connect(resetAll, &QAction::triggered, [=] { doResetAll(); });
        applicationMenu->addAction(resetAll);

        QAction* quit = new QAction(tr("&Quit"), this);
        quit->setStatusTip(tr("Exits the program"));
        quit->setShortcuts(QList<QKeySequence>({ Qt::Key_Q, Qt::SHIFT + Qt::Key_Q, QKeySequence::Quit }));
        connect(quit, SIGNAL(triggered()), this, SLOT(close()));
        applicationMenu->addAction(quit);
    }

    editMenu = menuBar()->addMenu(tr("&Edit"));
    {
        QAction* undo = new QAction(tr("&Undo"), this);
        undo->setStatusTip(tr("Undo the previous joint transformation"));
        undo->setShortcuts(QList<QKeySequence>({ Qt::Key_U, Qt::SHIFT + Qt::Key_U }));
        undo->setEnabled(false);
        connect(undo, &QAction::triggered, [=] { doUndo(); });
        editMenu->addAction(undo);

        QAction* redo = new QAction(tr("&Redo"), this);
        redo->setStatusTip(tr("Redo the previously undone joint transformation"));
        redo->setShortcuts(QList<QKeySequence>({ Qt::Key_R, Qt::SHIFT + Qt::Key_R }));
        redo->setEnabled(false);
        connect(redo, &QAction::triggered, [=] { doRedo(); });
        editMenu->addAction(redo);

        // attach a handler so that we can update the undo/redo buttons when the undo stack changes
        connect(viewer, &Viewer::undoStackUpdated, [=] {
            undo->setEnabled(viewer->getUndoStackSize() > 0);
            redo->setEnabled(viewer->getRedoStackSize() > 0);
        });
    }

    modeMenu = menuBar()->addMenu(tr("&Mode"));
    {
        puppet = new QAction(tr("&Position/Orientation"), this);
        puppet->setStatusTip(tr("Translate and rotate the entire puppet"));
        puppet->setShortcuts(QList<QKeySequence>({ Qt::Key_P, Qt::SHIFT + Qt::Key_P }));
        puppet->setCheckable(true);
        connect(puppet, &QAction::triggered, [=] { viewer->setInputMode(Viewer::Puppet); });
        modeMenu->addAction(puppet);

        joints = new QAction(tr("&Joints"), this);
        joints->setStatusTip(tr("Control joint angles"));
        joints->setShortcuts(QList<QKeySequence>({ Qt::Key_J, Qt::SHIFT + Qt::Key_J }));
        joints->setCheckable(true);
        connect(joints, &QAction::triggered, [=] { viewer->setInputMode(Viewer::Joints); });
        modeMenu->addAction(joints);

        QActionGroup* group = new QActionGroup(this);
        puppet->setActionGroup(group);
        joints->setActionGroup(group);

        puppet->setChecked(true);
    }

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    {
        QAction* trackballVisibility = new QAction(tr("&Circle"), this);
        trackballVisibility->setStatusTip(tr("Sets visibility of the trackball circle"));
        trackballVisibility->setCheckable(true);
        trackballVisibility->setShortcuts(QList<QKeySequence>({ Qt::Key_C, Qt::SHIFT + Qt::Key_C }));
        connect(trackballVisibility, &QAction::triggered, [=] { viewer->trackballVisible.toggle(); });
        optionsMenu->addAction(trackballVisibility);

        // since keyboard input can trigger these options to change, we attach signals to update the checkedness of each menu option
        connect(&viewer->trackballVisible, &BooleanOption::changed, [=] (bool oldValue, bool newValue) {
            (void) oldValue;
            trackballVisibility->setChecked(newValue);
        });

        QAction* depthBuffer = new QAction(tr("&Z-Buffer"), this);
        depthBuffer->setStatusTip(tr("Toggles the usage of the depth buffer"));
        depthBuffer->setCheckable(true);
        depthBuffer->setChecked(true);
        depthBuffer->setShortcuts(QList<QKeySequence>({ Qt::Key_Z, Qt::SHIFT + Qt::Key_Z }));
        connect(depthBuffer, &QAction::triggered, [=] { viewer->depthBufferEnabled.toggle(); });
        optionsMenu->addAction(depthBuffer);

        connect(&viewer->depthBufferEnabled, &BooleanOption::changed, [=] (bool oldValue, bool newValue) {
            (void) oldValue;
            depthBuffer->setChecked(newValue);
        });

        QAction* backfaceCulling = new QAction(tr("&Backface Cull"), this);
        backfaceCulling->setStatusTip(tr("Toggles the culling of backfacing polygons"));
        backfaceCulling->setCheckable(true);
        backfaceCulling->setChecked(true);
        backfaceCulling->setShortcuts(QList<QKeySequence>({ Qt::Key_B, Qt::SHIFT + Qt::Key_B }));
        connect(backfaceCulling, &QAction::triggered, [=] { viewer->backfaceCullingEnabled.toggle(); });
        optionsMenu->addAction(backfaceCulling);

        connect(&viewer->backfaceCullingEnabled, &BooleanOption::changed, [=] (bool oldValue, bool newValue) {
            (void) oldValue;
            backfaceCulling->setChecked(newValue);
        });

        QAction* frontfaceCulling = new QAction(tr("&Frontface Cull"), this);
        frontfaceCulling->setStatusTip(tr("Toggles the culling of frontfacing polygons"));
        frontfaceCulling->setCheckable(true);
        frontfaceCulling->setShortcuts(QList<QKeySequence>({ Qt::Key_F, Qt::SHIFT + Qt::Key_F }));
        connect(frontfaceCulling, &QAction::triggered, [=] { viewer->frontfaceCullingEnabled.toggle(); });
        optionsMenu->addAction(frontfaceCulling);

        connect(&viewer->frontfaceCullingEnabled, &BooleanOption::changed, [=] (bool oldValue, bool newValue) {
            (void) oldValue;
            frontfaceCulling->setChecked(newValue);
        });

        QAction* drawPickingBuffer = new QAction(tr("&Draw Picking Buffer"), this);
        drawPickingBuffer->setStatusTip(tr("Toggles the drawing of the picking buffer instead of the normal framebuffer"));
        drawPickingBuffer->setCheckable(true);
        drawPickingBuffer->setShortcuts(QList<QKeySequence>({ Qt::Key_D, Qt::SHIFT + Qt::Key_D }));
        connect(drawPickingBuffer, &QAction::triggered, [=] { viewer->drawPickingBufferEnabled.toggle(); });
        optionsMenu->addAction(drawPickingBuffer);

        connect(&viewer->drawPickingBufferEnabled, &BooleanOption::changed, [=] (bool oldValue, bool newValue) {
            (void) oldValue;
            drawPickingBuffer->setChecked(newValue);
        });
    }
}

void AppWindow::doResetJoints() {
    viewer->resetJoints();
}

void AppWindow::doResetAll() {
    viewer->resetAll();
}

void AppWindow::doUndo() {
    if (viewer->getUndoStackSize() > 0) {
        viewer->undo();
    } else {
        QApplication::beep();
    }
}

void AppWindow::doRedo() {
    if (viewer->getRedoStackSize() > 0) {
        viewer->redo();
    } else {
        QApplication::beep();
    }
}
