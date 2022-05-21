#include "MyGLWidget.h"

#include <iostream>

/**
 * He fet us de Blender per establir a origen de coordenades el model del patricio
 */

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL) {
    setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget() {
    if(program != NULL) delete program;
}

void MyGLWidget::initializeGL() {
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();  
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST);
    carregaShaders();
    iniEscena ();
    iniCamera ();
}

void MyGLWidget::iniEscena() {
    creaBuffersTerra();
    creaBuffersCub();
    creaBuffersPatricio();
    float escalat = 0.0;
    glm::vec3 centrePatricio;
    calculaCapsaModel(patricio, escalat, centreBasePatricio);
    if(escalat == (1.0/altura)) rescalar = false;
    else {
	rescalar = true;
	alturaActual = 1.0/escalat;
    }
    centreEsc = glm::vec3(10,2,10);
    radiEsc = glm::sqrt(glm::sqrt((centreEsc.x*centreEsc.x)+(centreEsc.z*centreEsc.z))*glm::sqrt((centreEsc.x*centreEsc.x)+(centreEsc.z*centreEsc.z)) + centreEsc.y * centreEsc.y);
    //centreEsc = glm::vec3(0,2,0);
    posPatricio = glm::vec3(0, 1, 0);
}

void MyGLWidget::iniCamera() {
    angleY = angleX = 0.0;
    obs = centreEsc + glm::vec3(0, 2, 2*radiEsc);
    vrp = centreEsc;
    up = glm::vec3(0,1,0);
    ra = 1.0;
    fov = M_PI/3.0;
    zn = radiEsc;
    zf = 3.0*radiEsc;
    projectTransform();
    viewTransform();
}

void MyGLWidget::paintGL() {
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
  
    // Esborrem el frame-buffer i el depth-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activem el VAO per a pintar el Terra 
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Activem el VAO per a pintar la Cub
    glBindVertexArray (VAO_Cub);
    modelTransformParet1 ();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Activem el VAO per a pintar la Cub
    glBindVertexArray (VAO_Cub);
    modelTransformParet2 ();
    glDrawArrays(GL_TRIANGLES, 0, 36);
  
    // Activem el VAO per a pintar el Homer
    glBindVertexArray (VAO_Patricio);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
  
    glBindVertexArray(0);
}

void MyGLWidget::resizeGL(int w, int h) {
    ample = w;
    alt = h;
    ra = float(ample)/float(alt);
    if(ra < 1) fov = 2*glm::atan(glm::tan(glm::asin(radiEsc/(radiEsc*2.0)))/ra);
    else fov = M_PI/3.0;
    projectTransform ();
}

void MyGLWidget::modelTransformHomer() {
    glm::mat4 TG(1.0f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3 (2, 1, 0.5));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio() {
    glm::mat4 TG(1.0f);  // Matriu de transformació
    // Comprovarem si s'ha de rescalar o no
    if(rescalar) {
	// Mirarem el percentatge de rescalar que s'ha de fer
	porcentatgeDeLaAlturaDesitjableARescalar = (altura/alturaActual);
	/* 
	   Explicacio:
	   Dividir entre altura i l'alturaActual ens donarà un percentatge ja dividit 
	   entre 100, el restar en 1 tindrem el percentatge que s'ha de rescalar el model.
	*/
	// Aplicquem el percentatge en tots els eixos, per mantenir la proporcio
	TG = glm::scale(TG,glm::vec3(porcentatgeDeLaAlturaDesitjableARescalar,porcentatgeDeLaAlturaDesitjableARescalar, porcentatgeDeLaAlturaDesitjableARescalar));
    }
    TG = glm::translate(TG, glm::vec3(24.5+(20.0*porcentatgeDeLaAlturaDesitjableARescalar)+posX,3.5,24.5+(20.0*porcentatgeDeLaAlturaDesitjableARescalar)+posZ));
    if(FPSmode) {
	vrp = glm::vec3(14.5+(10.0*porcentatgeDeLaAlturaDesitjableARescalar)+posX,3.5,14.5+(10.0*porcentatgeDeLaAlturaDesitjableARescalar)+posZ) + glm::vec3(0, 2, 2*radiEsc);
	obs = glm::vec3(14.5+(10.0*porcentatgeDeLaAlturaDesitjableARescalar)+posX,3.5,14.5+(10.0*porcentatgeDeLaAlturaDesitjableARescalar)+posZ);
    }
    TG = glm::rotate(TG,glm::radians(rotacioY),glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformParet1() { // Cal modificar aquesta funció...
    glm::mat4 TG(1.0f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3(10.0, 0.0, 16.0));
    //TG = glm::rotate(TG,(float) glm::radians(90.0), glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3(0.6, 6.0, 8.0));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformParet2() { // Cal modificar aquesta funció...
    glm::mat4 TG(1.0f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3(10.0, 0.0, 4.0));
    //TG = glm::rotate(TG,(float) glm::radians(90.0), glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3(0.6, 6.0, 8.0));  
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra() {
    glm::mat4 TG(1.0f);  // Matriu de transformació
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform() { // Cal modificar aquesta funció...
    glm::mat4 Proj;  // Matriu de projecció
    if(FPSmode) {
	Proj = glm::perspective(fov, ra, zn+1.0f, zf*1.0f);
    } else {
	Proj = glm::perspective(fov, ra, zn, zf);
    }
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() { // Cal modificar aquesta funció...
    glm::mat4 View(1.0f);  // Matriu de posició i orientació
    //View = glm::lookAt(obs, vrp, up);
    if(not FPSmode) {
	View = glm::translate(View,(vrp-obs));
	View = glm::rotate(View, (float) (0.0 - glm::radians(angleY)), glm::vec3(1.0f,0.0f,0.0f));
	View = glm::rotate(View, (float) (M_PI/6.0 - glm::radians(angleX)), glm::vec3(0.0f,1.0f,0.0f));
	View = glm::translate(View,glm::vec3(-10.0f, 0.0f, -10.0f));
    } else {
	switch(rotacioYFPSmode) {
	    case 0:
		View = glm::lookAt(glm::vec3(10.0+posXFPS,2.0,10.0+posZFPS), glm::vec3(10.0+posXFPS,2.0,20.0+posZFPS), up);
	        break;
	    case 1:
		View = glm::lookAt(glm::vec3(10.0+posXFPS,2.0,10.0+posZFPS), glm::vec3(20.0+posXFPS,2.0,10.0+posZFPS), up);
		break;
	    case 2:
		View = glm::lookAt(glm::vec3(10.0+posXFPS,2.0,10.0+posZFPS), glm::vec3(10.0+posXFPS,2.0,0.0+posZFPS), up);
	        break;
	    case 3:
		View = glm::lookAt(glm::vec3(10.0+posXFPS,2.0,10.0+posZFPS), glm::vec3(0.0+posXFPS,2.0,10.0+posZFPS), up);
	        break;
	}
	//View = glm::rotate(View, (float) (glm::radians(rotacioY)), glm::vec3(0.0f,1.0f,0.0f));
	//View = glm::rotate(View, (float) (M_PI/6.0 - glm::radians(angleX)), glm::vec3(0.0f,1.0f,0.0f));
	//View = glm::translate(View,glm::vec3(10.0f+posX, 0.0f, 10.0f+posZ));
    }

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) { // Cal modificar aquesta funció...
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_I:
	    centreEsc = glm::vec3(10,2,10);
	    posX = 0.0;
	    posZ = 0.0;
	    rotacioY = 0.0;
	    posXFPS = 0.0;
	    posZFPS = 0.0;
	    break;
        case Qt::Key_R:
	    rotacioY += 90.0;
	    rotacioYFPSmode++;
	    break;
        case Qt::Key_C:
	    if(not FPSmode) {
		FPSmode = true;
		//fov = glm::radians(60.0);
		fov = M_PI/3.0;
		zn = 0.0;
	    } else {
		FPSmode = false;
		obs = centreEsc + glm::vec3(0, 2, 2*radiEsc);
                vrp = centreEsc;
		ra = 1.0;
		fov = M_PI/3.0;
		zn = radiEsc;
	    }
	    break;
        case Qt::Key_Left:
	    if(not colisionX(posX-1.5)) {
		posX -= 1.5;
		posXFPS -= 0.5;
	    }
	    break;
        case Qt::Key_Right:
	    if(not colisionX(posX+1.5)) {
		posX += 1.5;
		posXFPS += 0.5;
	    }
	    break;
        case Qt::Key_Up:
	    if(not colisionZ(posZ+1.5)) {
		posZ += 1.5;
		posZFPS += 0.5;
	    }
	    break;
        case Qt::Key_Down:
	    if(not colisionZ(posZ-1.5)) {
		posZ -= 1.5;
		posZFPS -= 0.5;
	    }
	    break;
        default:
	    event->ignore();
	    break;
    }
    projectTransform();
    viewTransform();
    update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *e) {
    xClick = e->x();
    yClick = e->y();
    if (e->button() & Qt::LeftButton &&! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) DoingInteractive = ROTATE;
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *) {
    DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
    makeCurrent();
    if (DoingInteractive == ROTATE) {
	// Aquí cal completar per fer la rotació...
	angleX += e->x() - xClick;
	angleY += e->y() - yClick;
	viewTransform ();
    }
    xClick = e->x();
    yClick = e->y();
    update ();
}

void MyGLWidget::calculaCapsaModel(Model &p, float &escala, glm::vec3 &centreBase) {
    // Càlcul capsa contenidora i valors transformacions inicials
    float minx, miny, minz, maxx, maxy, maxz;
    minx = maxx = p.vertices()[0];
    miny = maxy = p.vertices()[1];
    minz = maxz = p.vertices()[2];
    for (unsigned int i = 3; i < p.vertices().size(); i+=3) {
	if (p.vertices()[i+0] < minx) minx = p.vertices()[i+0];
	if (p.vertices()[i+0] > maxx) maxx = p.vertices()[i+0];
	if (p.vertices()[i+1] < miny) miny = p.vertices()[i+1];
	if (p.vertices()[i+1] > maxy) maxy = p.vertices()[i+1];
	if (p.vertices()[i+2] < minz) minz = p.vertices()[i+2];
	if (p.vertices()[i+2] > maxz) maxz = p.vertices()[i+2];
    }
    escala = 1.0/(maxy-miny);
    centreBase[0] = (minx+maxx)/2.0; 
    centreBase[1] = miny; 
    centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::creaBuffersCub() {
    // Dades del cub
    // Vèrtexs del cub
    glm::vec3 vertexs[8] = {
	/* 0*/ glm::vec3( -0.5, 0.0, -0.5),  /* 1*/ glm::vec3( 0.5, 0.0, -0.5),
	/* 2*/ glm::vec3( -0.5, 1.0, -0.5),  /* 3*/ glm::vec3( 0.5, 1.0, -0.5),
	/* 4*/ glm::vec3( -0.5, 0.0, 0.5),  /* 5*/ glm::vec3( 0.5, 0.0, 0.5),
	/* 6*/ glm::vec3( -0.5, 1.0, 0.5),  /* 7*/ glm::vec3( 0.5, 1.0, 0.5)
    };

    // VBO amb la posició dels vèrtexs
    glm::vec3 poscub[36] = {  // 12 triangles
	vertexs[0], vertexs[2], vertexs[1],
	vertexs[1], vertexs[2], vertexs[3],
	vertexs[5], vertexs[1], vertexs[7],
	vertexs[1], vertexs[3], vertexs[7],
	vertexs[2], vertexs[6], vertexs[3],
	vertexs[3], vertexs[6], vertexs[7],
	vertexs[0], vertexs[4], vertexs[6],
	vertexs[0], vertexs[6], vertexs[2],
	vertexs[0], vertexs[1], vertexs[4],
	vertexs[1], vertexs[5], vertexs[4],
	vertexs[4], vertexs[5], vertexs[6],
	vertexs[5], vertexs[7], vertexs[6]
    };
  
    glm::vec3 color(0, 0, 1.0);

    glm::vec3 colorcub[36] = {
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color
    };
  
    // Creació del Vertex Array Object del cub
    glGenVertexArrays(1, &VAO_Cub);
    glBindVertexArray(VAO_Cub);

    GLuint VBO_Cub[2];
    glGenBuffers(2, VBO_Cub);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorcub), colorcub, GL_STATIC_DRAW);

    // Activem l'atribut normalLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersHomer() {
    // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
    homer.load("./models/HomerProves.obj");
  
    // Creació del Vertex Array Object del Homer
    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    // Creació dels buffers del model
    GLuint VBO_Homer[2];
    // Buffer de posicions
    glGenBuffers(2, VBO_Homer);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Buffer de component difusa usada com a color
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersPatricio() {
    // Cal completar aquest mètode afegint també en el .h els atributs que 
    // siguin necessaris...
    // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
    patricio.load("./models/Patricio.obj");
  
    // Creació del Vertex Array Object del Patricio
    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    // Creació dels buffers del model
    GLuint VBO_Patricio[2];
    // Buffer de posicions
    glGenBuffers(2, VBO_Patricio);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3, patricio.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Buffer de component difusa usada com a color
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3, patricio.VBO_matdiff(), GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTerra() {
    // Dades del terra
    // VBO amb la posició dels vèrtexs
    glm::vec3 posterra[6] = {
	glm::vec3(0.0, 0.0, 20.0),
	glm::vec3(20.0, 0.0, 20.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(20.0, 0.0, 20.0),
	glm::vec3(20.0, 0.0, 0.0),
    }; 

    glm::vec3 color(0.0,0.6,0.0);

    // Fem que aquest material afecti a tots els vèrtexs per igual
    glm::vec3 colterra[6] = {
	color, color, color, color, color, color
    };

    // Creació del Vertex Array Object del terra
    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    GLuint VBO_Terra[2];
    glGenBuffers(2, VBO_Terra);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);
}

void MyGLWidget::carregaShaders() {
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

    // Demanem identificadors per als uniforms del vertex shader
    transLoc = glGetUniformLocation (program->programId(), "TG");
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

bool MyGLWidget::colisionZ(float pos) {
    bool colision = false;
    if(pos < -27.0 or pos > 27.5) colision = true;
    if(not colision and FPSmode and (posXFPS < -9.0 or posXFPS > 9.0)) colision = true;
    return colision;
}

bool MyGLWidget::colisionX(float pos) {
    bool colision = false;
    if(pos < -27.0 or pos > 27.5) colision = true;
    if(not colision and FPSmode and (posZFPS < -9.0 or posZFPS > 9.0)) colision = true;
    return colision;
}
