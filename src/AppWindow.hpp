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

    SceneNode* scene = NULL;

public:
    AppWindow();
    ~AppWindow();

    void loadScene(const std::string& path);

private:
    void createActions();
    void createMenu();

    QMenu* applicationMenu;

    std::vector<QAction*> menuActions;
    Viewer* viewer;
};

#endif
