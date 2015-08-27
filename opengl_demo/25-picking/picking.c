#include <stdio.h>
#include <GL/glut.h>

#define TITLE "picking"
#define WIDTH 320
#define HEIGHT 240

#define SELECT_BUFFER_SIZE 512

void draw(GLenum mode);
void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
int selectHits(GLint hits, const GLuint *buffer);
void pick(int x, int y);
void mouse(int button, int state, int x, int y);
void init(void);

int select;


void draw(GLenum mode)
{
	if(mode == GL_SELECT) glLoadName(1);

	if(select == 1) glColor3f(1., 0., 0.);
	else glColor3f(0.5, 0.5, 0.5);

	glPushMatrix();
	glTranslatef(-0.3, -0.1, 1.);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0., 0.5);
	glEnd();
	glPopMatrix();

	if(mode == GL_SELECT) glLoadName(2);

	if(select == 2) glColor3f(1., 0., 0.);
	else glColor3f(0.3, 0.3, 0.3);

	glPushMatrix();
	glTranslatef(0.3, 0.1, 0.);
	glBegin(GL_QUADS);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, 0.5);
	glEnd();
	glPopMatrix();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	draw(GL_RENDER);

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

	glLoadIdentity();

	display();
}


int selectHits(GLint hits, const GLuint *buffer)
{
	int i;
	const GLuint *p;
	GLuint names;
	GLuint depth1;
	GLuint depth2;
	GLuint depth = -1;
	int select = -1;
	
	if(hits == 0) return -1;
	
	p = buffer;

	for(i = 0; i < hits; i++){
		names = *p++;
		depth1 = *p++;
		depth2 = *p++;
		
		if(depth > depth1){
			depth = depth1;
			select = *p;
			p += names;
		}
	}
	
	return select;
}


void pick(int x, int y)
{
	GLuint selectBuffer[SELECT_BUFFER_SIZE];
	GLint hits;
	GLint vp[4];
	
	glGetIntegerv(GL_VIEWPORT, vp);

	glSelectBuffer(SELECT_BUFFER_SIZE, selectBuffer);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	gluPickMatrix(x, vp[3] - y, 5., 5., vp);
	setOrtho(vp[2], vp[3]);

	glMatrixMode(GL_MODELVIEW);
	draw(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	hits = glRenderMode(GL_RENDER);

	printf("hits = %d\n", hits);

	select = selectHits(hits, selectBuffer);
}


void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		pick(x, y);
		glutPostRedisplay();
	}
}


void init(void)
{
	glClearColor(1., 1., 1., 1.);
	glEnable(GL_DEPTH_TEST);

	select = -1;
}


int main(int argc, char *argv[])
{
	int w, h;

	glutInit(&argc, argv);

	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize (WIDTH, HEIGHT); 
	glutInitWindowPosition((w - WIDTH)/2, (h - HEIGHT)/2);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow (TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	init();

	glutMainLoop();

	return 0;
}
