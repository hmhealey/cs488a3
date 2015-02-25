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
        puppet->toggle();
    } else if (event->key() == Qt::Key_J) {
        viewer->setInputMode(Viewer::Joints);
        joints->toggle();
    } else if (event->key() == Qt::Key_U) {
        doUndo();
    } else if (event->key() == Qt::Key_R) {
        doRedo();
    } else if (event->key() == Qt::Key_C) {
        doToggleTrackballVisibility();
    } else if (event->key() == Qt::Key_Z) {
        doToggleDepthBuffer();
    } else if (event->key() == Qt::Key_B) {
        doToggleBackfaceCulling();
    } else if (event->key() == Qt::Key_F) {
        doToggleFrontfaceCulling();
    } else if (event->key() == Qt::Key_D) {
        doToggleDrawPickingBuffer();
#endif
    }
}

void AppWindow::mouseReleaseEvent(QMouseEvent* event) {
    // we just finished manipulating a joint so we've wiped out the redo stack
    if (viewer->getInputMode() == Viewer::Joints) {
        redo->setEnabled(false);
    }

    viewer->mouseReleaseEvent(event);
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

        puppet->toggle();
    }

    editMenu = menuBar()->addMenu(tr("&Edit"));
    {
        undo = new QAction(tr("&Undo"), this);
        undo->setStatusTip(tr("Undo the previous joint transformation"));
        undo->setShortcuts(QList<QKeySequence>({ Qt::Key_U, Qt::SHIFT + Qt::Key_U }));
        undo->setEnabled(false);
        connect(undo, &QAction::triggered, [=] { doUndo(); });
        editMenu->addAction(undo);

        redo = new QAction(tr("&Redo"), this);
        redo->setStatusTip(tr("Redo the previously undone joint transformation"));
        redo->setShortcuts(QList<QKeySequence>({ Qt::Key_R, Qt::SHIFT + Qt::Key_R }));
        redo->setEnabled(false);
        connect(redo, &QAction::triggered, [=] { doRedo(); });
        editMenu->addAction(redo);
    }

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    {
        toggleTrackballVisibility = new QAction(tr("&Circle"), this);
        toggleTrackballVisibility->setStatusTip(tr("Sets visibility of the trackball circle"));
        toggleTrackballVisibility->setCheckable(true);
        toggleTrackballVisibility->setShortcuts(QList<QKeySequence>({ Qt::Key_C, Qt::SHIFT + Qt::Key_C }));
        connect(toggleTrackballVisibility, &QAction::triggered, [=] { doToggleTrackballVisibility(); });
        optionsMenu->addAction(toggleTrackballVisibility);

        toggleDepthBuffer = new QAction(tr("&Z-Buffer"), this);
        toggleDepthBuffer->setStatusTip(tr("Toggles the usage of the depth buffer"));
        toggleDepthBuffer->setCheckable(true);
        toggleDepthBuffer->setChecked(true);
        toggleDepthBuffer->setShortcuts(QList<QKeySequence>({ Qt::Key_Z, Qt::SHIFT + Qt::Key_Z }));
        connect(toggleDepthBuffer, &QAction::triggered, [=] { doToggleDepthBuffer(); });
        optionsMenu->addAction(toggleDepthBuffer);

        toggleBackfaceCulling = new QAction(tr("&Backface Cull"), this);
        toggleBackfaceCulling->setStatusTip(tr("Toggles the culling of backfacing polygons"));
        toggleBackfaceCulling->setCheckable(true);
        toggleBackfaceCulling->setChecked(true);
        toggleBackfaceCulling->setShortcuts(QList<QKeySequence>({ Qt::Key_B, Qt::SHIFT + Qt::Key_B }));
        connect(toggleBackfaceCulling, &QAction::triggered, [=] { doToggleBackfaceCulling(); });
        optionsMenu->addAction(toggleBackfaceCulling);

        toggleFrontfaceCulling = new QAction(tr("&Frontface Cull"), this);
        toggleFrontfaceCulling->setStatusTip(tr("Toggles the culling of frontfacing polygons"));
        toggleFrontfaceCulling->setCheckable(true);
        toggleFrontfaceCulling->setShortcuts(QList<QKeySequence>({ Qt::Key_F, Qt::SHIFT + Qt::Key_F }));
        connect(toggleFrontfaceCulling, &QAction::triggered, [=] { doToggleFrontfaceCulling(); });
        optionsMenu->addAction(toggleFrontfaceCulling);

        toggleDrawPickingBuffer = new QAction(tr("&Draw Picking Buffer"), this);
        toggleDrawPickingBuffer->setStatusTip(tr("Toggles the drawing of the picking buffer instead of the normal framebuffer"));
        toggleDrawPickingBuffer->setCheckable(true);
        toggleDrawPickingBuffer->setShortcuts(QList<QKeySequence>({ Qt::Key_D, Qt::SHIFT + Qt::Key_D }));
        connect(toggleDrawPickingBuffer, &QAction::triggered, [=] { doToggleDrawPickingBuffer(); });
        optionsMenu->addAction(toggleDrawPickingBuffer);
    }
}

void AppWindow::doResetJoints() {
    viewer->resetJoints();

    undo->setEnabled(false);
    redo->setEnabled(false);
}

void AppWindow::doResetAll() {
    viewer->resetAll();

    undo->setEnabled(false);
    redo->setEnabled(false);
}

void AppWindow::doUndo() {
    if (viewer->getUndoStackSize() > 0) {
        int remaining = viewer->undo();

        undo->setEnabled(remaining > 0);
    } else {
        QApplication::beep();
        undo->setEnabled(false);
    }
}

void AppWindow::doRedo() {
    if (viewer->getRedoStackSize() > 0) {
        int remaining = viewer->redo();

        redo->setEnabled(remaining > 0);
    } else {
        QApplication::beep();
        redo->setEnabled(false);
    }
}

void AppWindow::doToggleTrackballVisibility() {
    bool visible = viewer->isTrackballVisible();

    viewer->setTrackballVisible(!visible);
    toggleTrackballVisibility->setChecked(!visible);
}

void AppWindow::doToggleDepthBuffer() {
    bool enabled = viewer->isDepthBufferEnabled();

    viewer->setDepthBufferEnabled(!enabled);
    toggleDepthBuffer->setChecked(!enabled);
}

void AppWindow::doToggleBackfaceCulling() {
    bool enabled = viewer->isBackfaceCullingEnabled();

    viewer->setBackfaceCullingEnabled(!enabled);
    toggleBackfaceCulling->setChecked(!enabled);
}

void AppWindow::doToggleFrontfaceCulling() {
    bool enabled = viewer->isFrontfaceCullingEnabled();

    viewer->setFrontfaceCullingEnabled(!enabled);
    toggleFrontfaceCulling->setChecked(!enabled);
}

void AppWindow::doToggleDrawPickingBuffer() {
    bool enabled = viewer->isDrawPickingBufferEnabled();

    viewer->setDrawPickingBufferEnabled(!enabled);
    toggleDrawPickingBuffer->setChecked(!enabled);
}
