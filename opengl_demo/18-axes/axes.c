#include <GL/glut.h>

#define TITLE "axes"
#define WIDTH 320
#define HEIGHT 240

void drawBackground(void);
void drawArrow(void);
void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void idle(void);
void init(void);

static float angle;


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


void display(void)
{
	GLint vp[4];
	int w, h;

	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();

	glPushMatrix();
	glRotatef(angle, 0., 1., 0.);
	glColor3f(0., 0., 0.);
	glutWireTeapot(0.5);
	glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glGetIntegerv(GL_VIEWPORT, vp);

	w = vp[2];
	h = vp[3];

	glLoadIdentity();
	glOrtho(0., w, 0., h, -30., 30.);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(40., 40., 0.);
	glRotatef(angle, 0., 1., 0.);

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

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	setOrtho(w, h);

	glMatrixMode(GL_MODELVIEW);

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
