#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffers();
  ini_camera();
}

void MyGLWidget::paintGL () 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  modelTransformTerra();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Terra);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);

  projectTransform();
  viewTransform();


  // Carreguem la transformació de model
  modelTransform ();
  
  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Homer);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransformTerra () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  //transform = glm::scale(transform, glm::vec3(0.0));
  //transform = glm::rotate(transform, 0.0f, glm::vec3(0, 0, 0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, (float)(rad*M_PI/4), glm::vec3(0, 1, 0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective(FOV, ra, znear, zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
    glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
		rad += 0.5;
		break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers () 
{
  // Dades de la caseta
  // Dos VBOs, un amb posició i l'altre amb color
  glm::vec3 posicio[6] = {
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(-1.0, -1.0, 1.0)
  }; 
  glm::vec3 color[6] = {
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,1),
	glm::vec3(0,1,0),
	glm::vec3(1,0,0)
  };

// Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  m.load("../figures3D/HomerProves.obj");

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void MyGLWidget::ini_camera() {
    FOV = M_PI/2.0;
    ra = 1.0f;
    znear = 0.4f;
    zfar = 3.0f;
    OBS = glm::vec3(0,0,1);
    VRP = glm::vec3(0,0,0);
    UP = glm::vec3(0,1,0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

