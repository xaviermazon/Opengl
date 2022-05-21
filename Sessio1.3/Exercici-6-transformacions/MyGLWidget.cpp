//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
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
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retna.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  modelTranslateRotate();
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO1_caseta);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

  modelTranslateRotateTriangle();
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO2_triangle);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

// Modificacio Translacio
void MyGLWidget::modelTranslateRotate() {
  glm::mat4 TG(1.0);
  TG = glm::rotate(TG, (float)rad, glm::vec3(0, 0, (float)1));
  TG = glm::scale(TG, glm::vec3(sclx, scly, 0.0));
  TG = glm::translate(TG, glm::vec3(x, y, 0.0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTranslateRotateTriangle() {
  glm::mat4 TG(1.0);
  TG = glm::rotate(TG, (float)rad2, glm::vec3(0, 0, (float)1));
  TG = glm::scale(TG, glm::vec3(sclx, scly, 0.0));
  TG = glm::translate(TG, glm::vec3(x, y, 0.0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


// Modificacio Qt per teclat
void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  makeCurrent();
  switch(e->key()) {
    case Qt::Key_P:
      rad += M_PI/6.0;
      rad2 -= M_PI/6.0;
      break;
    case Qt::Key_S:
      sclx += 0.1;
      scly += 0.5;
      break;
    case Qt::Key_D:
      scly -= 0.1;
      sclx += 0.5;
      break;
    case Qt::Key_Left:
      x -= 0.1;
      rad += 45.0;
      break;
    case Qt::Key_Right:
      x += 0.1;
      rad += 45.0;
      break;
    case Qt::Key_Up:
      y += 0.1;
      rad += 45.0;
      break;
    case Qt::Key_Down:
      y -= 0.1;
      rad += 45.0;
      break;
    default: e-> ignore();
  }
  update();
}

void MyGLWidget::creaBuffers ()
{
    glm::vec3 Vertices_Caseta[5], Vertices_Triangle[3];  // Tres vèrtexs amb X, Y i Z
  // Caseta
  Vertices_Caseta[0] = glm::vec3(-1.0, 0.0, 0.0);
  Vertices_Caseta[1] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices_Caseta[2] = glm::vec3(0.0, -1.0, 0.0);
  Vertices_Caseta[3] = glm::vec3(0.0, 0.0, 0.0);
  Vertices_Caseta[4] = glm::vec3(-0.5, 1.0, 0.0);
  // Triangle
  Vertices_Triangle[0] = glm::vec3(1.0, 0.0, 0.0);
  Vertices_Triangle[1] = glm::vec3(0.5, 1.0, 0.0);
  Vertices_Triangle[2] = glm::vec3(0.0, 0.0, 0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1_caseta);
  glBindVertexArray(VAO1_caseta);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1_Caseta, VBO2_Triangle;
  glGenBuffers(1, &VBO1_Caseta);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1_Caseta);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Caseta), Vertices_Caseta, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenVertexArrays(2, &VAO2_triangle);
  glBindVertexArray(VAO2_triangle);
  glGenBuffers(1, &VBO2_Triangle);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2_Triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Triangle), Vertices_Triangle, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  // Desactivem el VAO
  glBindVertexArray(0);
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
  transLoc = glGetUniformLocation (program->programId(), "TG");
  
}
