#include <string.h>
#include <GL/glut.h>

#include "trackball.h"

#define TITLE "stencil"
#define WIDTH 320
#define HEIGHT 240

void getWindowSize(int *w, int *h);
void applyRotateMatrix(void);
void drawText(int x, int y, const char *str);
void drawBackground(void);
void drawFloor(void);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void key(unsigned char key, int x, int y);
void init(void);

static const GLfloat materialAmbient[4] = {0.3, 0.2, 0.1, 1.};
static const GLfloat materialDiffuse[4] = {0.8, 0.6, 0.1, 1.};
static const GLfloat materialSpecular[4] = {1., 0.9, 0.9, 1.};
static const GLfloat materialShininess[4] = {30., 30., 80., 0.8};
static const GLfloat light0Ambient[4] = {0.3, 0.3, 0.3, 1.};
static const GLfloat light0Diffuse[4] = {0.4, 0.4, 0.4, 1.};
static const GLfloat light0Specular[4] = {1., 1., 1., 1.};
static const GLfloat light0Position[4] = {0., 5., 0., 1.};

static int mouseStartX;
static int mouseStartY;
static float curquat[4];
static float lastquat[4];
static int stencil;


void getWindowSize(int *w, int *h)
{
	GLint vp[4];

	glGetIntegerv(GL_VIEWPORT, vp);

	*w = vp[2];
	*h = vp[3];
}


void applyRotateMatrix(void)
{
	GLfloat m[4][4];

	build_rotmatrix(m, curquat);
	glMultMatrixf(&m[0][0]);
}


void drawText(int x, int y, const char *str)
{
	int w, h;
	int i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	getWindowSize(&w, &h);
	gluOrtho2D(0., w, 0., h);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();
	glScalef(1., -1., 1.);
	glTranslatef(0., -h, 0.);

	glRasterPos2f(x, y);
	for(i = 0; i < strlen(str); i++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	}

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
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
	glBegin(GL_QUADS);
	glVertex3f(-2., 0., 2.);
	glVertex3f(2., 0., 2.);
	glVertex3f(2., 0., -2.);
	glVertex3f(-2., 0., -2.);
	glEnd();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	drawBackground();

	glPushMatrix();

	glLoadIdentity();
	gluLookAt(0., 3., 5., 0., 0., 0., 0., 1., 0.);
	applyRotateMatrix();

	if(stencil){
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		drawFloor();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 0xfffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glScalef(1., -1., 1.);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glPushMatrix();
	glTranslatef(0., 0.7, 0.);
	glCullFace(GL_FRONT);
	glutSolidTorus(0.1, 0.5, 20, 20);
	glCullFace(GL_BACK);
	glPopMatrix();
	glPopMatrix();

	if(stencil) glDisable(GL_STENCIL_TEST);

	glDisable(GL_LIGHTING);

	glFrontFace(GL_CW);
	glColor4f(0.3, 0.3, 0.5, 1.);
	drawFloor();
	glFrontFace(GL_CCW);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.3, 0.3, 0.3, 0.8);
	drawFloor();
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glTranslatef(0., 0.7, 0.);
	glutSolidTorus(0.1, 0.5, 20, 20);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPopMatrix();

	glColor3f(0., 0., 0.);
	if(stencil) drawText(0, 10, "Stencil On");
	else drawText(0, 10, "Stencil Off");

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


void mouse(int button, int state, int x, int y)
{
	mouseStartX = x;
	mouseStartY = y;
}


void motion(int x, int y)
{
	int w, h;

	getWindowSize(&w, &h);

	trackball(lastquat,
	  (2.*mouseStartX - w)/w,
	  (h - 2.*mouseStartY)/h,
	  (2.*x - w)/w,
	  (h - 2.*y)/h
	);

	add_quats(lastquat, curquat, curquat);

	mouseStartX = x;
	mouseStartY = y;

	glutPostRedisplay();
}


void key(unsigned char key, int x, int y)
{
	if(key == 's'){
		stencil = !stencil;
		glutPostRedisplay();
	}
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

	trackball(curquat, 0., 0., 0., 0.);
}


int main(int argc, char* argv[])
{
	int w, h;

	glutInit(&argc, argv);

	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition((w - WIDTH)/2, (h - HEIGHT)/2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow(TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);

	init();

	glutMainLoop();

	return 0;
}
