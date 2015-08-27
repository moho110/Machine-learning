#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

#define TITLE "text"
#define WIDTH 320
#define HEIGHT 240

#define MAX_STR_LEN 16

void display(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passiveMotion(int x, int y);
void init(void);

static char mousePositionStr[MAX_STR_LEN];
static char mouseButtonStr[MAX_STR_LEN];


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


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawText(0, 10, mousePositionStr);
	drawText(0, 25, mouseButtonStr);

	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
	switch(state){
	case GLUT_DOWN:
		switch(button){
		case GLUT_LEFT_BUTTON:
			sprintf(mouseButtonStr, "LEFT");
			break;
		case GLUT_MIDDLE_BUTTON:
			sprintf(mouseButtonStr, "MIDDLE");
			break;
		case GLUT_RIGHT_BUTTON:
			sprintf(mouseButtonStr, "RIGHT");
			break;
		default:
			break;
		}
		break;
	case GLUT_UP:
		sprintf(mouseButtonStr, " ");
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


void motion(int x, int y)
{
	sprintf(mousePositionStr, "(%4d, %4d)", x, y);
	glutPostRedisplay();
}


void passiveMotion(int x, int y)
{
	sprintf(mousePositionStr, "(%4d, %4d)", x, y);
	glutPostRedisplay();
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);
	glColor3f(0., 0., 0.);

	sprintf(mousePositionStr, "(%4d, %4d)", 0, 0);
	sprintf(mouseButtonStr, " ");
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
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);

	init();

	glutMainLoop();

	return 0;
}
