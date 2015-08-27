#include <string.h>
#include <GL/glut.h>

#define TITLE "depth"
#define WIDTH 320
#define HEIGHT 240

void drawText(int x, int y, const char *str);
void drawPolygon(void);
void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void idle(void);
void key(unsigned char key, int x, int y);
void init(void);

float angle;
int depth;


void drawText(int x, int y, const char *str)
{
	GLint vp[4];
	int w, h;
	int i;

	glGetIntegerv(GL_VIEWPORT, vp);

	w = vp[2];
	h = vp[3];

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();

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

	if(depth) glEnable(GL_DEPTH_TEST);

	glPushMatrix();

	glRotatef(angle, 0., 1., 0.);

	glPushMatrix();
	glTranslatef(-0.25, 0., 1.);
	glColor3f(1., 0., 0.);
	drawPolygon();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0., -1.);
	glColor3f(0., 1., 0.);
	drawPolygon();
	glPopMatrix();

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);

	glColor3f(0., 0., 0.);
	if(depth) drawText(0, 10, "Depth Test ON");
	else drawText(0, 10, "Depth Test OFF");

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


void key(unsigned char key, int x, int y)
{
	if(key == 'd'){
		depth = !depth;
		glutPostRedisplay();
	}
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(key);

	init();

	glutMainLoop();

	return 0;
}
