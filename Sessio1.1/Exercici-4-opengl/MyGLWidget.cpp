
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

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1_Caseta);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
  
  // Desactivem el VAO
  glBindVertexArray(0);
  
    // Activem l'Array a pintar 
  glBindVertexArray(VAO2_Triangle);
 
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
  glGenVertexArrays(1, &VAO1_Caseta);
  glBindVertexArray(VAO1_Caseta);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1_Caseta, VBO2_Triangle;
  glGenBuffers(1, &VBO1_Caseta);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1_Caseta);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Caseta), Vertices_Caseta, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenVertexArrays(2, &VAO2_Triangle);
  glBindVertexArray(VAO2_Triangle);
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
}
