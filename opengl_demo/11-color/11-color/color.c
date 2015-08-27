#include <GL/glut.h>

#define TITLE "color"
#define WIDTH 320
#define HEIGHT 240

void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void init(void);


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(1., 0., 0.);
	glVertex2f(-0.3, -0.5);
	glColor3f(1., 1., 0.);
	glVertex2f(0.3, -0.5);
	glColor3f(0., 1., 0.);
	glVertex2f(0.5, 0.);
	glColor3f(0., 1., 1.);
	glVertex2f(0., 0.45);
	glColor3f(0., 0., 1.);
	glVertex2f(-0.5, 0.);
	glEnd();

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

	init();

	glutMainLoop();

	return 0;
}
