#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//Figura 3D
#include "./../figures3D/Model/model.h"
//Depuracio en Qt + Opengl
#include <QDebug>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransformTerra ();
    void projectTransform();
    void viewTransform();
    void ini_camera();
    void ini_camera_3persona();
    
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Homer, VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // Internal vars
    float scale, rad = 0.0;
    glm::vec3 pos;
    //Valors de camera
    float FOV, ra, znear, zfar;
    glm::vec3 OBS, VRP, UP;
    //Figura 3D
    Model m;
    //Calcul de camera en 3D
    glm::vec3 min, max, centrat;
    float radi, diametre;
};

