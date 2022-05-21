#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
//Modificacio - Qt per teclat
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"
//Modificacio per radians
#define GLM_FORCE_RADIANS

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // Modificacio QT per teclat
    virtual void keyPressEvent(QKeyEvent *e);
    
  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTranslateRotate();
    void modelTranslateRotateTriangle();

    // attribute locations
    GLuint vertexLoc, vertexLocTriangle, transLoc;

    // Posicions xy
    float x = 0.5;
    float y = 0.5;
    // Graus
    float rad = 0.0, rad2 = 0.0;

    // Escalat no uniformat
    float sclx = 0.5;
    float scly = 0.5;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1_caseta,VAO2_triangle;
    GLint ample, alt;
};
