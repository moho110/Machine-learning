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

#include <windows.h>  // Windows的头文件
#include <glew.h>  // 包含最新的gl.h,glu.h库
#include <gl/glut.h>  // 包含OpenGL实用库


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)    // 重置OpenGL窗口大小
{
	if (height==0)        // 防止被零除
	{
		height=1;       // 将Height设为1
	}
	
	glViewport(0, 0, width, height);     // 重置当前的视口
	glMatrixMode(GL_PROJECTION);      // 选择投影矩阵
	glLoadIdentity();       // 重置投影矩阵
	
	// 设置视口的大小
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);      // 选择模型观察矩阵
	glLoadIdentity();       // 重置模型观察矩阵
}


void InitGL(GLvoid)        // 此处开始对OpenGL进行所有设置
{
	glShadeModel(GL_SMOOTH);      // 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);     // 黑色背景
	glClearDepth(1.0f);       // 设置深度缓存
	glEnable(GL_DEPTH_TEST);      // 启用深度测试
	glDepthFunc(GL_LEQUAL);       // 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // 告诉系统对透视进行修正 
	
	
	
}
void DrawGLScene(GLvoid)        // 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // 清除屏幕和深度缓存
	glLoadIdentity();       // 重置当前的模型观察矩阵
	
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("第一个OpenGL程序");
	InitGL();
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);
	glutMainLoop();
	return 0;
}