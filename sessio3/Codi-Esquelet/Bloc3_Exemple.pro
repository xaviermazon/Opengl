TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ../figures3D/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp \
        MyGLWidget.cpp ../figures3D/Model/model.cpp
