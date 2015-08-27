#include <GL/glut.h>

#define TITLE "blend"
#define WIDTH 320
#define HEIGHT 240

void drawPolygon(void);
void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void idle(void);
void init(void);

float angle;


void drawPolygon(void)
{
	glBegin(GL_POLYGON);
	glVertex2f(-0.3, -0.5);
	glVertex2f(0.3, -0.5);
	glVertex2f(0.5, 0.);
	glVertex2f(0., 0.45);
	glVertex2f(-0.5, 0.);
	glEnd();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(angle, 0., 1., 0.);

	glPushMatrix();
	glTranslatef(-0.25, 0., 0.5);
	glColor3f(0., 1., 0.);
	drawPolygon();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0., -1.);
	glColor3f(0., 1., 1.);
	drawPolygon();
	glPopMatrix();

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(-0.5, 0., 1.);
	glColor4f(1., 0., 0., 0.5);
	drawPolygon();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0., 0., 0.);
	glColor4f(0., 0., 1., 0.5);
	drawPolygon();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0., -0.5);
	glColor4f(1., 1., 0., 0.5);
	drawPolygon();
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glPopMatrix();

	glutSwapBuffers();
}


void setOrtho(int w, int h)
{
	float aspect;

	if(w > h){
		aspect = (float)w/h;
		glOrtho(-aspect, aspect, -1., 1., 0.1, 100.);
	}else{
		aspect = (float)h/w;
		glOrtho(-1., 1., -aspect, aspect, 0.1, 100.);
	}
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	setOrtho(w, h);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0., 0., 2.5, 0., 0., 0., 0., 1., 0.);

	display();
}


void idle(void)
{
	angle += 0.1;
	
	if(angle > 360.) angle -= 360.;

	glutPostRedisplay();
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}


int main(int argc, char* argv[])
{
	int w, h;

	glutInit(&argc, argv);

	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition((w - WIDTH)/2, (h - HEIGHT)/2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();

	glutMainLoop();

	return 0;
}
