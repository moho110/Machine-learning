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
static const int sn = 3;
static const int tn = 4;
static const int sknotn = 3 + 3; /* sn + order */
static const int tknotn = 4 + 3; /* tn + order */
static const int sstride = 4*4; /* tn*4 */
static const int tstride = 4;
GLfloat sknot[6] = {0., 1., 2., 3., 4., 5.};
GLfloat tknot[7] = {0., 1., 2., 3., 4., 5., 6.};
GLfloat ctrlPoint[3][4][4] = {
	{
		{-0.5, -0.5, 0., 1.},
		{-0.75, -0.25, 0., 1.},
		{-0.25, 0.25, 0., 1.},
		{-0.5, 0.5, 0., 1.},
	},
	{
		{0., -0.5, 0., 1.},
		{0., -0.25, 0., 1.},
		{0., 0.25, 0., 1.},
		{0., 0.5, 0., 1.},
	},
	{
		{0.5, -0.5, 0., 1.},
		{0.25, -0.25, 0., 1.},
		{0.75, 0.25, 0., 1.},
		{0.5, 0.5, 0., 1.},
	},
};

static GLUnurbsObj *nurbs;


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	gluBeginSurface(nurbs);
	gluNurbsSurface(nurbs, sknotn, sknot, tknotn, tknot,
	  sstride, tstride, &ctrlPoint[0][0][0], order, order, GL_MAP2_VERTEX_4);
	gluEndSurface(nurbs);

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

	glColor3f(0.5, 0.5, 0.5);

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
