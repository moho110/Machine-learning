#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define TITLE "nurbs"
#define WIDTH 320
#define HEIGHT 240

void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void nurbsError(GLenum error);
void init(void);

static const int order = 3;
static const int n = 4;
static const int knotn = 4 + 3; /* n + order */
static const int stride = 4;
GLfloat knot[7] = {0., 1., 2., 3., 4., 5., 6.};
GLfloat ctrlPoint[4][4] = {
	{-0.5, 0., 0., 1.},
	{-0.25, 0.5, 0., 1.},
	{0.25, -0.5, 0., 1.},
	{0.5, 0., 0., 1.}
};

static GLUnurbsObj *nurbs;


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	gluBeginCurve(nurbs);
	gluNurbsCurve(nurbs, knotn, knot, stride, &ctrlPoint[0][0],
	  order, GL_MAP1_VERTEX_4);
	gluEndCurve(nurbs);

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


void nurbsError(GLenum error)
{
	const GLubyte *str;

	str = gluErrorString(error);
	printf("NURBS error: %s\n", str);
	exit(0);
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.);
	glColor3f(0., 0., 0.);

	nurbs = gluNewNurbsRenderer();
	gluNurbsProperty(nurbs, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
	gluNurbsProperty(nurbs, GLU_SAMPLING_TOLERANCE, 10.);
	gluNurbsProperty(nurbs, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsProperty(nurbs, GLU_CULLING, GL_TRUE);
	gluNurbsProperty(nurbs, GLU_AUTO_LOAD_MATRIX, GL_TRUE);
	gluNurbsCallback(nurbs, GLU_ERROR, (void *)nurbsError);
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
