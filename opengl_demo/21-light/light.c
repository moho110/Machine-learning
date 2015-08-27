#include <string.h>
#include <GL/glut.h>

#define TITLE "light"
#define WIDTH 320
#define HEIGHT 240

void setProjection(int w, int h);
void getWindowSize(int *w, int *h);
void drawText(int x, int y, const char *str);
void drawBackground(void);
void display(void);
void reshape(int w, int h);
void idle(void);
void key(unsigned char key, int x, int y);
void init(void);

static const GLfloat materialAmbient[4] = {0.3, 0.2, 0.1, 1.};
static const GLfloat materialDiffuse[4] = {0.8, 0.6, 0.1, 1.};
static const GLfloat materialSpecular[4] = {1., 0.9, 0.9, 1.};
static const GLfloat materialShininess[4] = {30., 30., 80., 0.8};
static const GLfloat light0Ambient[4] = {0.3, 0.3, 0.3, 1.};
static const GLfloat light0Diffuse[4] = {0.4, 0.4, 0.4, 1.};
static const GLfloat light0Specular[4] = {1., 1., 1., 1.};
static const GLfloat light0Position[4] = {5., 0., 0., 1.};

static int perspective;
static int smooth;
static float angle;


void setProjection(int w, int h)
{
	float aspect;

	if(perspective){
		gluPerspective(45., (float)w/h, 1., 10.);
	}else{
		if(w > h){
			aspect = (float)w/h;
			glOrtho(-aspect, aspect, -1., 1., 1., 10.);
		}else{
			aspect = (float)h/w;
			glOrtho(-1., 1., -aspect, aspect, 1., 10.);
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


void display(void)
{
	int w, h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	getWindowSize(&w, &h);
	setProjection(w, h);

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackground();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPushMatrix();

	glLoadIdentity();
	glRotatef(angle, 0., 0., 1.);

	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	glPopMatrix();

	glPushMatrix();

	glLoadIdentity();
	gluLookAt(0., 0., 2.5, 0., 0., 0., 0., 1., 0.);
	glRotatef(angle, 0., 1., 0.);

	glutSolidTeapot(0.5);

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(0., 0., 0.);
	if(perspective) drawText(0, 10, "Perspective");
	else drawText(0, 10, "Orthogonal");
	if(smooth) drawText(0, 20, "Smooth");
	else drawText(0, 20, "Flat");

	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	display();
}


void idle(void)
{
	angle += 1.;

	if(angle > 360.) angle -= 360.;

	glutPostRedisplay();
}


void key(unsigned char key, int x, int y)
{
	switch(key){
	case 'p':
		perspective = !perspective;
		glutPostRedisplay();
		break;
	case 's':
		smooth = !smooth;
		if(smooth) glShadeModel(GL_SMOOTH);
		else glShadeModel(GL_FLAT);
		glutPostRedisplay();
		break;
	}
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);

	perspective = 1;
	smooth = 1;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	glEnable(GL_LIGHT0);
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
