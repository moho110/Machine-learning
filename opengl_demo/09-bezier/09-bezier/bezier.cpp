#include <GL/glut.h>

#define TITLE "bezier"
#define WIDTH 320
#define HEIGHT 240

void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void init(void);

static const int n = 4;
static const int stride = 3;
static const GLfloat ctrlPoint[4][3] = {
	{-0.5, 0., 0.},
	{-0.25, 0.5, 0.},
	{0.25, -0.5, 0.},
	{0.5, 0., 0.},
};


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEvalMesh1(GL_LINE, 0, 100);

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

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.);
	glColor4f(0., 0., 0., 1.);

	glMap1f(GL_MAP1_VERTEX_3, 0., 1., stride, n, &ctrlPoint[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0., 1.);
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

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
