TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h \
    ../figures3D/Model/model.h

SOURCES += main.cpp MyForm.cpp \
        ../figures3D/Model/model.cpp \
        MyGLWidget.cpp 

DISTFILES += \
    shaders/basicShader.frag \
    shaders/basicShader.vert
