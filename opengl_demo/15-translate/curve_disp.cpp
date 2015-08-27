/*
 * =====================================================================================
 *
 *       Filename:  curve_disp.cpp
 *
 *    Description:  curve_disp
 *
 *        Version:  1.0
 *        Created:  2014-11-25 21:39:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  VRer (Linux Programmer), vrml1234@gmail.com
 *        Company:  VrmlStudio.inc
 *
 * =====================================================================================
 */

// curve_disp.c  �ꥵ�`����(Lissajous)���� 
// �������褯�Ȥ��ϡ��̤��ۤ쾀���褯
   
#include <math.h>      // sin(), cos()
#include <GL/glut.h>
 
double a, b, x[3], y[3], rad=0.01745329252;  // ��/180

void display(void)
{
  int i,k0,k1,k2; 
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_LINE_STRIP);       // �ۤ쾀�����ɤ���
    glVertex2d( x[0],y[0] );    // �����2����ۤ쾀���褯
    glVertex2d( x[1],y[1] );
    for( i=0; i<=180; i++ ){ // �u��ʽ�ǴΡ��ȵ�������ۤ쾀���褯
      k0 = i%3;                 // %�����Ӥ� i��3�Ǹ�ä��Ȥ������
      k1 = (i+1)%3;             // k0, k1, k2 �΂��ϡ�혤� 0, 1, 2 ���R�귵��
      k2 = (i+2)%3;
      x[k2] = a*x[k1]-x[k0];    // �u��ʽ�� x[i+2] = a*x[i+1] - x[i]
      y[k2] = b*y[k1]-y[k0];
      glVertex2d( x[k2]*.99,y[k2]*.99 ); 
    }
  glEnd();                      // �ۤ쾀���ɤ����ޤ�
  glFlush();                    // ����˳���
}
 
void init(void)
{
  double p1=6.,p2=8.;            // 2�N���Ӥ�����
  glClearColor( 1.,1.,1.,1. );
  glColor3f( 1.,0.4,0. );
  glLineWidth(2.);
  a = 2.*cos(rad*p1);             // �u��ʽ�΂S��(x����)
  b = 2.*cos(rad*p2);             // �u��ʽ�΂S��(y����)
  x[0] = 0.;  x[1] = sin(rad*p1); // �����2���x����
  y[0] = 0.;  y[1] = sin(rad*p2); // �����2���y����
}
 
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize( 500,500 );
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}

//end of curve_disp.cpp
