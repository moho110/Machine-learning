/*
* =====================================================================================
*
*       Filename:  bezier.cpp
*
*    Description:  bezier
*
*        Version:  1.0
*        Created:  2015-1-19 21:28:54
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  VRer (Linux Programmer), vrml1234@gmail.com
*        Company:  VrmlStudio.inc
*
* =====================================================================================
 */

#include <windows.h>  // Windows��ͷ�ļ�
#include <glew.h>  // �������µ�gl.h,glu.h��
#include <gl/glut.h>  // ����OpenGLʵ�ÿ�


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)    // ����OpenGL���ڴ�С
{
	if (height==0)        // ��ֹ�����
	{
		height=1;       // ��Height��Ϊ1
	}
	
	glViewport(0, 0, width, height);     // ���õ�ǰ���ӿ�
	glMatrixMode(GL_PROJECTION);      // ѡ��ͶӰ����
	glLoadIdentity();       // ����ͶӰ����
	
	// �����ӿڵĴ�С
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);      // ѡ��ģ�͹۲����
	glLoadIdentity();       // ����ģ�͹۲����
}


void InitGL(GLvoid)        // �˴���ʼ��OpenGL������������
{
	glShadeModel(GL_SMOOTH);      // ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);     // ��ɫ����
	glClearDepth(1.0f);       // ������Ȼ���
	glEnable(GL_DEPTH_TEST);      // ������Ȳ���
	glDepthFunc(GL_LEQUAL);       // ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // ����ϵͳ��͸�ӽ������� 
	
	
	
}
void DrawGLScene(GLvoid)        // �����￪ʼ�������еĻ���
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // �����Ļ����Ȼ���
	glLoadIdentity();       // ���õ�ǰ��ģ�͹۲����
	
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("��һ��OpenGL����");
	InitGL();
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);
	glutMainLoop();
	return 0;
}