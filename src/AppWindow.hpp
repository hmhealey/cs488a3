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

public:
    AppWindow();

private:
    void createActions();
    void createMenu();

    QMenu* applicationMenu;

    std::vector<QAction*> menuActions;
    Viewer* viewer;
};

#endif
