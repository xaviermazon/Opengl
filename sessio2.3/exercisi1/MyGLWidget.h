#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "./../figures3D/Model/model.h"


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
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransformTerra ();
    void projectTransform();
    void viewTransform();
    void ini_camera();
	void ini_camera_3persona();
	void ini_camera_ortogonal();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Casa, VAO_model;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // Internal vars
    float scale, rad = 0.0, FOV, ra, znear, zfar, diametre, radi;
	float bottom, left, right, top;
	float dist, theta = 0.0, psi = 0.0, phi;
    glm::vec3 pos, OBS, VRP, up, min, max, centrat;
    Model m;
	bool ortogonal = false;
	int xClick, yClick;
	typedef enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
};

