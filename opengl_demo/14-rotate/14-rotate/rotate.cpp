//文件名：rotate.cpp
//功能描述：空间场景显示茶壶并且mouse旋转。
//调用函数库：GLUT32

#include <GL/glut.h>

#define TITLE "rotate"
#define WIDTH 320
#define HEIGHT 240

void drawBackground(void);
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


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();

	glPushMatrix();

	glRotatef(angle, 0., 1., 0.);

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

//end of rotate.