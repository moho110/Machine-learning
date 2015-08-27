//文件名：texture.cpp
//功能描述：图片纹理显示，并且读取文件名为：ppm
//调用函数库:GLUT32.lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <GL/glut.h>

#define TITLE "texture"
#define WIDTH 320
#define HEIGHT 240

#define TEXTURE_FILE "adelie.ppm"

#define LOAD_TEXTURE_SUCCESS 0
#define LOAD_TEXTURE_OPEN_ERROR 1
#define LOAD_TEXTURE_FILETYPE_ERROR 2
#define LOAD_TEXTURE_MEMORY_ERROR 3

#define BUFFER_SIZE 256

GLubyte* loadTexture(const char *filename, int *w, int *h, int *info);
void checkLoadTexture(int info);
void display(void);
void setOrtho(int w, int h);
void reshape(int w, int h);
void init(void);

GLubyte *texture;
GLuint textureID;


GLubyte* loadTexture(const char *filename, int *w, int *h, int *info)
{
	FILE *fp;
	char buffer[BUFFER_SIZE];
	int i;
	GLubyte *p;

	fp = fopen(filename, "r");
	if(fp == NULL){
		*info = 1;
		return NULL;
	}

	printf("Loading texture file '%s'...\n", filename);
	
	i = 0;
	while(1){
		fgets(buffer, sizeof(buffer), fp);
		if(buffer[0] == '#') continue;
		switch(i){
		case 0:
			if(strncmp(buffer, "P6", 2) != 0){
				*info = 2;
				return NULL;
			}
			break;
		case 1:
			sscanf(buffer, "%d%d", w, h);
			break;
		case 2:
			goto end_loop;
			break;
		default:
			assert(0);
			break;
		}

		i++;
	}
end_loop:

	printf("Size: %d x %d\n", *w, *h);

	p = malloc(sizeof(GLubyte)*(*w)*(*h)*3);
	if(p == NULL){
		*info = 3;
		return NULL;
	}
	
	fread(p, (*w)*(*h)*3, 1, fp);
	fclose(fp);

	*info = 0;

	printf("Done.\n");

	return p;
}


void checkLoadTexture(int info)
{
	switch(info){
	case LOAD_TEXTURE_SUCCESS:
		return;
		break;
	case LOAD_TEXTURE_OPEN_ERROR:
		printf("error: Can't open texture file\n");
		break;
	case LOAD_TEXTURE_FILETYPE_ERROR:
		printf("error: Unknown texture file type\n");
		break;
	case LOAD_TEXTURE_MEMORY_ERROR:
		printf("error: Memory allocation for texture failed\n");
		break;
	default:
		assert(0);
		break;
	}

	exit(0);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glPushMatrix();

	glRotatef(-15., 0., 1., 0.);
	glRotatef(-15., 1., 0., 0.);

	glBegin(GL_QUADS);
	glTexCoord2f(0., 2.);
	glVertex2f(-0.5, -0.5);
	glTexCoord2f(2., 2.);
	glVertex2f(0.5, -0.5);
	glTexCoord2f(2., 0.);
	glVertex2f(0.5, 0.5);
	glTexCoord2f(0., 0.);
	glVertex2f(-0.5, 0.5);
	glEnd();

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

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(45., (float)w/h, 1., 10.);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0., 0., 2.5, 0., 0., 0., 0., 1., 0.);

	display();
}


void init(void)
{
	int w, h;
	int info;

	glClearColor(0.8, 0.9, 1., 1.);
	
	texture = loadTexture(TEXTURE_FILE, &w, &h, &info);
	checkLoadTexture(info);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
	  GL_RGB, GL_UNSIGNED_BYTE, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
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
