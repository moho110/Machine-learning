/*
 * =====================================================================================
 *
 *       Filename:  deform_disp.c
 *
 *    Description:  deform_disp
 *
 *        Version:  1.0
 *        Created:  2014-11-25 20:42:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  VRer (Linux Programmer), vrml1234@gmail.com
 *        Company:  VrmlStudio.inc
 *
 * =====================================================================================
 */

#include  <stdio.h>
#include  <string.h>
#include  <malloc.h>
#include  <ctype.h>
#include  <pthread.h>
#include  <stdlib.h>
#include  <math.h>
#include  <GL/glut.h>

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //背景上色
	glColor3d(0.5,0.5,0.5); //颜色指定红绿蓝
	glLineWidth(1.5); //线宽（像素单位）
	glPointSize(6.0); //
	glBegin(GL_TRIANGLES);
	glVertex2d(-0.69,0.4);
	glVertex2d(-0.69,-0.4);
	glVertex2d(0. ,0.8);
	glVertex2d(0. ,-0.8);
	glVertex2d(0.69,0.4);
	glVertex4d(0.69,-0.4,3,4);
	glEnd();
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(480,480);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glClearColor(1.0,1.0,1.0,1.0);
	glutMainLoop();
	return 0;
}

//end of deform_disp.c
