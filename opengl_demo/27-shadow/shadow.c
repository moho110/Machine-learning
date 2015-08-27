#include <math.h>
#include <GL/glut.h>

#define TITLE "shadow"
#define WIDTH 320
#define HEIGHT 240

void shadowMatrix(GLfloat shadowMat[4][4],
  GLfloat groundplane[4], GLfloat lightpos[4]);
void findPlane(GLfloat plane[4],
  GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
void makeMatrix(GLfloat a[],
  GLfloat a00, GLfloat a01, GLfloat a02, GLfloat a03,
  GLfloat a10, GLfloat a11, GLfloat a12, GLfloat a13,
  GLfloat a20, GLfloat a21, GLfloat a22, GLfloat a23,
  GLfloat a30, GLfloat a31, GLfloat a32, GLfloat a33);
void rotateMatrixY(GLfloat angle, GLfloat rotateMat[]);
void getWindowSize(int *w, int *h);
void drawText(int x, int y, const char *str);
void drawBackground(void);
void drawFloor(void);
void display(void);
void reshape(int w, int h);
void idle(void);
void init(void);

static const GLfloat materialAmbient[4] = {0.3, 0.2, 0.1, 1.};
static const GLfloat materialDiffuse[4] = {0.8, 0.6, 0.1, 1.};
static const GLfloat materialSpecular[4] = {1., 0.9, 0.9, 1.};
static const GLfloat materialShininess[4] = {30., 30., 80., 0.8};
static const GLfloat light0Ambient[4] = {0.3, 0.3, 0.3, 1.};
static const GLfloat light0Diffuse[4] = {0.4, 0.4, 0.4, 1.};
static const GLfloat light0Specular[4] = {1., 1., 1., 1.};
static const GLfloat light0Position0[4] = {8., 5., 0., 1.};
static const GLfloat floorVertex[4][3] = {
	{-2., 0.,  2.},
	{ 2., 0.,  2.},
	{ 2., 0., -2.},
	{-2., 0., -2.},
};

static float angle;
static GLfloat floorPlane[4];


/* create a matrix that will project the desired shadow */
void shadowMatrix(GLfloat shadowMat[4][4],
  GLfloat groundplane[4], GLfloat lightpos[4])
{
	GLfloat dot;

	/* find dot product between light position vector and ground plane normal */
	dot = groundplane[0] * lightpos[0] +
	groundplane[1] * lightpos[1] +
	groundplane[2] * lightpos[2] +
	groundplane[3] * lightpos[3];

	shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
	shadowMat[1][0] = 0. - lightpos[0] * groundplane[1];
	shadowMat[2][0] = 0. - lightpos[0] * groundplane[2];
	shadowMat[3][0] = 0. - lightpos[0] * groundplane[3];

	shadowMat[0][1] = 0. - lightpos[1] * groundplane[0];
	shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
	shadowMat[2][1] = 0. - lightpos[1] * groundplane[2];
	shadowMat[3][1] = 0. - lightpos[1] * groundplane[3];

	shadowMat[0][2] = 0. - lightpos[2] * groundplane[0];
	shadowMat[1][2] = 0. - lightpos[2] * groundplane[1];
	shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
	shadowMat[3][2] = 0. - lightpos[2] * groundplane[3];

	shadowMat[0][3] = 0. - lightpos[3] * groundplane[0];
	shadowMat[1][3] = 0. - lightpos[3] * groundplane[1];
	shadowMat[2][3] = 0. - lightpos[3] * groundplane[2];
	shadowMat[3][3] = dot - lightpos[3] * groundplane[3];
}


/* find the plane equation given 3 points */
void findPlane(GLfloat plane[4],
  GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
	GLfloat vec0[3], vec1[3];
	enum {X, Y, Z};
	enum {A, B, C, D};

  	/* need 2 vectors to find cross product */
	vec0[X] = v1[X] - v0[X];
	vec0[Y] = v1[Y] - v0[Y];
	vec0[Z] = v1[Z] - v0[Z];

	vec1[X] = v2[X] - v0[X];
	vec1[Y] = v2[Y] - v0[Y];
	vec1[Z] = v2[Z] - v0[Z];

	/* find cross product to get A, B, and C of plane equation */
	plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
	plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

	plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}


void makeMatrix(GLfloat a[],
  GLfloat a00, GLfloat a01, GLfloat a02, GLfloat a03,
  GLfloat a10, GLfloat a11, GLfloat a12, GLfloat a13,
  GLfloat a20, GLfloat a21, GLfloat a22, GLfloat a23,
  GLfloat a30, GLfloat a31, GLfloat a32, GLfloat a33)
{
	a[0] = a00;
	a[1] = a01;
	a[2] = a02;
	a[3] = a03;

	a[4] = a10;
	a[5] = a11;
	a[6] = a12;
	a[7] = a13;

	a[8] = a20;
	a[9] = a21;
	a[10] = a22;
	a[11] = a23;

	a[12] = a30;
	a[13] = a31;
	a[14] = a32;
	a[15] = a33;
}


void rotateMatrixY(GLfloat angle, GLfloat rotateMat[])
{
	GLfloat c, s;

	c = cos(angle/180.*M_PI);
	s = sin(angle/180.*M_PI);

	makeMatrix(
	  rotateMat,
	   c, 0.,  s, 0.,
	  0., 1., 0., 0.,
	  -s, 0.,  c, 0.,
	  0., 0., 0., 1.
	);
}


void getWindowSize(int *w, int *h)
{
	GLint vp[4];

	glGetIntegerv(GL_VIEWPORT, vp);

	*w = vp[2];
	*h = vp[3];
}


void drawBackground(void)
{
	int w, h;
	float l, r, b, t;

	getWindowSize(&w, &h);

	if(w > h){
		l = -(float)w/h;
		r = -l;
		b = -1.;
		t = 1.;
	}else{
		l = -1.;
		r = 1.;
		b = -(float)h/w;
		t = -b;
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	gluOrtho2D(l, r, b, t);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glBegin(GL_QUADS);
	glColor3f(1., 1., 1.);
	glVertex2f(l, b);
	glVertex2f(r, b);
	glColor3f(0.6, 0.8, 1.);
	glVertex2f(r, t);
	glVertex2f(l, t);
	glEnd();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}


void drawFloor(void)
{
	int i;

	glBegin(GL_QUADS);
	for(i = 0; i < 4; i++) glVertex3fv(floorVertex[i]);
	glEnd();
}


void display(void)
{
	GLfloat rotateMat[4][4];
	GLfloat light0Position[4];
	GLfloat floorShadow[4][4];
	int i, j;

	/* calculate light position */
	rotateMatrixY(angle, &rotateMat[0][0]);
	for(i = 0; i < 4; i++){
		light0Position[i] = 0.;
		for(j = 0; j < 4; j++){
			light0Position[i] +=
			  rotateMat[i][j]*light0Position0[j];
		}
	}

	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	shadowMatrix(floorShadow, floorPlane, light0Position);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
	  GL_STENCIL_BUFFER_BIT);

	drawBackground();

	glPushMatrix();

	glLoadIdentity();
	gluLookAt(0., 3., 5., 0., 0., 0., 0., 1., 0.);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
	drawFloor();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 0xfffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glPushMatrix();
	glColor3f(0., 0., 0.);
	glMultMatrixf(&floorShadow[0][0]);
	glTranslatef(0., 0.7, 0.);
	glutSolidTorus(0.1, 0.5, 20, 20);
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);

	glFrontFace(GL_CW);
	glColor4f(0.3, 0.3, 0.5, 1.);
	drawFloor();
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.3, 0.3, 0.3, 0.8);
	drawFloor();
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(0., 0.7, 0.);
	glutSolidTorus(0.1, 0.5, 20, 20);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPopMatrix();

	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(45., (float)w/h, 1., 10.); 

	glMatrixMode(GL_MODELVIEW);

	display();
}


void idle(void)
{
	angle += 1.;
	glutPostRedisplay();
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);

	glEnable(GL_CULL_FACE);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	glEnable(GL_LIGHT0);

	findPlane(floorPlane, (GLfloat *)floorVertex[0],
	  (GLfloat *)floorVertex[1], (GLfloat *)floorVertex[2]);
}


int main(int argc, char* argv[])
{
	int w, h;

	glutInit(&argc, argv);

	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition((w - WIDTH)/2, (h - HEIGHT)/2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |
	  GLUT_STENCIL);
	glutCreateWindow(TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();

	glutMainLoop();

	return 0;
}
