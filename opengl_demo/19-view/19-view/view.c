#include <math.h>
#include <string.h>
#include <GL/glut.h>

#include "trackball.h"

#define TITLE "view"
#define WIDTH 320
#define HEIGHT 240

#define PROJECTION_ANGLE 45.
#define PROJECTION_NEAR 0.1
#define PROJECTION_FAR 100.

#define VIEW_EYE_POINT_Z 2.5
#define VIEW_CENTER_POINT_Z 0.

#define VIEW_SCALE 1.1
#define VIEW_SCALE_MOVE_MAX 5.

void setProjection(int w, int h);
void getWindowSize(int *w, int *h);
void applyRotateMatrix(void);
void drawText(int x, int y, const char *str);
void drawBackground(void);
void drawArrow(void);
void drawAxes(void);
void display(void);
void reshape(int w, int h);
void key(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void init(void);

static int perspective;
static int mouseButton;
static int mouseStartX;
static int mouseStartY;
static float curquat[4];
static float lastquat[4];
static float viewScale;
static float viewX;
static float viewY;


void setProjection(int w, int h)
{
	float aspect;

	if(perspective){
		gluPerspective(PROJECTION_ANGLE, (float)w/h,
		  PROJECTION_NEAR, PROJECTION_FAR);
	}else{
		if(w > h){
			aspect = (float)w/h;
			glOrtho(-aspect, aspect, -1., 1.,
		  	  PROJECTION_NEAR, PROJECTION_FAR);
		}else{
			aspect = (float)h/w;
			glOrtho(-1., 1., -aspect, aspect,
		  	  PROJECTION_NEAR, PROJECTION_FAR);
		}
	}
}


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


void drawArrow(void)
{
	glBegin(GL_LINES);
	glVertex3f(0., 0., 0.);
	glVertex3f(1., 0., 0.);
	glVertex3f(0.8, 0.05, 0.);
	glVertex3f(1., 0., 0.);
	glVertex3f(0.8, -0.05, 0.);
	glVertex3f(1., 0., 0.);
	glEnd();
}


void drawAxes()
{
	int w, h;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();

	getWindowSize(&w, &h);
	glOrtho(0., w, 0., h, -30., 30.);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(40., 40., 0.);
	applyRotateMatrix();

	glScalef(30., 30., 30.);

	/* X */
	glColor3f(1., 0., 0.);
	drawArrow();
	glRasterPos3f(1., 0.1, 0.);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'X');

	/* Y */
	glColor3f(0., 1., 0.);
	glPushMatrix();
	glRotatef(90., 0., 0., 1.);
	drawArrow();
	glRasterPos3f(1., 0.1, 0.);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'Y');
	glPopMatrix();

	/* Z */
	glColor3f(0., 0., 1.);
	glPushMatrix();
	glRotatef(-90., 0., 1., 0.);
	drawArrow();
	glRasterPos3f(1., 0.1, 0.);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'Z');
	glPopMatrix();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}


void display(void)
{
	int w, h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	getWindowSize(&w, &h);
	setProjection(w, h);

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();

	glPushMatrix();

	glLoadIdentity();
	gluLookAt(0., 0., VIEW_EYE_POINT_Z,
	  0., 0., VIEW_CENTER_POINT_Z, 0., 1., 0.);
	
	glScalef(viewScale, viewScale, viewScale);
	glTranslatef(viewX, viewY, 0.);
	applyRotateMatrix();

	glColor3f(0., 0., 0.);
	glutWireTeapot(0.5);

	glPopMatrix();

	drawAxes();

	glColor3f(0., 0., 0.);
	if(perspective) drawText(0, 10, "Perspective");
	else drawText(0, 10, "Orthogonal");

	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	display();
}


void key(unsigned char key, int x, int y)
{
	if(key == 'p'){
		perspective = !perspective;
		glutPostRedisplay();
	}
}


void mouse(int button, int state, int x, int y)
{
	mouseButton = button;
	mouseStartX = x;
	mouseStartY = y;
}


void motion(int x, int y)
{
	int w, h;
	float r, H;
	float scale;

	switch(mouseButton){
	case GLUT_LEFT_BUTTON:
		getWindowSize(&w, &h);
		trackball(lastquat,
		  (2.*mouseStartX - w)/w,
		  (h - 2.*mouseStartY)/h,
		  (2.*x - w)/w,
		  (h - 2.*y)/h
		);
		add_quats(lastquat, curquat, curquat);
		break;
	case GLUT_MIDDLE_BUTTON:
		getWindowSize(&w, &h);
		r = fabs(VIEW_EYE_POINT_Z - VIEW_CENTER_POINT_Z);
		H = r*PROJECTION_ANGLE/180.*M_PI;
		if(viewScale < VIEW_SCALE_MOVE_MAX) scale = viewScale;
		else scale = VIEW_SCALE_MOVE_MAX;
		viewX += (float)(x - mouseStartX)/h*H/scale;
		viewY -= (float)(y - mouseStartY)/h*H/scale;
		break;
	case GLUT_RIGHT_BUTTON:
		if(y - mouseStartY > 0) viewScale /= VIEW_SCALE;
		else viewScale *= VIEW_SCALE;
		break;
	default:
		break;
	}

	mouseStartX = x;
	mouseStartY = y;

	glutPostRedisplay();
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);
	glShadeModel(GL_SMOOTH);

	perspective = 1;

	trackball(curquat, 0., 0., 0., 0.);
	viewScale = 1.;
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
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	init();

	glutMainLoop();

	return 0;
}
