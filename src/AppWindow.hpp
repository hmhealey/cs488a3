#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <vector>

#include "Viewer.hpp"

class AppWindow : public QMainWindow {
    Q_OBJECT

    Viewer* viewer;

    QMenu* applicationMenu;
    QMenu* modeMenu;
    QMenu* editMenu;
    QMenu* optionsMenu;

    // keep a handle to these events so that we can check/enable/disable/fire them from key events
    QAction* puppet;
    QAction* joints;

    SceneNode* scene = NULL;

public:
    AppWindow();
    ~AppWindow();

    void keyPressEvent(QKeyEvent* event);

    void loadScene(const std::string& path);

private:
    void createMenu();

    void doResetJoints();
    void doResetAll();

    void doUndo();
    void doRedo();
};

#endif
