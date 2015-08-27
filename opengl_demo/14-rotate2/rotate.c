#include <math.h>
#include <GL/glut.h>

#define TITLE "rotate"
#define WIDTH 320
#define HEIGHT 240

void makeMatrix(GLfloat a[],
  GLfloat a00, GLfloat a01, GLfloat a02, GLfloat a03,
  GLfloat a10, GLfloat a11, GLfloat a12, GLfloat a13,
  GLfloat a20, GLfloat a21, GLfloat a22, GLfloat a23,
  GLfloat a30, GLfloat a31, GLfloat a32, GLfloat a33);
void rotateMatrixY(GLfloat angle, GLfloat rotateMat[]);
void drawBackground(void);
void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void init(void);

static float angle;


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


void drawBackground(void)
{
	GLint vp[4];
	int w, h;
	float l, r, b, t;

	glGetIntegerv(GL_VIEWPORT, vp);

	w = vp[2];
	h = vp[3];

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

	glBegin(GL_QUADS);
	glColor3f(1., 1., 1.);
	glVertex2f(l, b);
	glVertex2f(r, b);
	glColor3f(0.6, 0.8, 1.);
	glVertex2f(r, t);
	glVertex2f(l, t);
	glEnd();
}


void display(void)
{
	GLfloat rotateMat[16];

	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();

	glPushMatrix();

	rotateMatrixY(angle, rotateMat);
	glMultMatrixf(rotateMat);

	glColor3f(0., 0., 0.);
	glutWireTeapot(0.5);

	glPopMatrix();

	glutSwapBuffers();
}


void setOrtho(int w, int h)
{
	float aspect;

	if(w > h){
		aspect = (float)w/h;
		gluOrtho2D(-aspect, aspect, -1., 1.);
	}else{
		aspect = (float)h/w;
		gluOrtho2D(-1., 1., -aspect, aspect);
	}
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glLoadIdentity();
	setOrtho(w, h);

	display();
}


void idle(void)
{
	angle += 1.;

	if(angle > 360.) angle -= 360.;

	glutPostRedisplay();
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);
	glShadeModel(GL_SMOOTH);
}


int main(int argc, char* argv[])
{
	int w, h;

	glutInit(&argc, argv);

	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition((w - WIDTH)/2, (h - HEIGHT)/2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow(TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();

	glutMainLoop();

	return 0;
}
