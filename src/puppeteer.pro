######################################################################
# Automatically generated by qmake (3.0) Wed May 28 16:55:50 2014
######################################################################

QT += opengl widgets
CONFIG += c++11
QMAKE_CXXFLAGS += -W -Wall -g 
TEMPLATE = app
TARGET = puppeteer

macx {
    LIBS += -L/usr/local/lib -llua5.1
    INCLUDEPATH += /usr/local/include/lua5.1
}
unix:!macx {
    LIBS += -llua5.1
    INCLUDEPATH += /usr/include/lua5.1
}

# Input
HEADERS += Algebra.hpp \
           AlgebraToQt.hpp \
           AppWindow.hpp \
           Material.hpp \
           Mesh.hpp \
           Option.hpp \
           Primitive.hpp \
           Scene.hpp \
           scene_lua.hpp \
           Shader.hpp \
           Trackball.hpp \
           UndoStack.hpp \
           Viewer.hpp
SOURCES += Algebra.cpp \
           AlgebraToQt.cpp \
           AppWindow.cpp \ 
           Main.cpp \
           Material.cpp \
           Mesh.cpp \
           Option.cpp \
           Primitive.cpp \
           Scene.cpp \
           scene_lua.cpp \
           Shader.cpp \
           Trackball.cpp \
           UndoStack.cpp \
           Viewer.cpp
