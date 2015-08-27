/*
 * =====================================================================================
 *
 *       Filename:  matrix.cpp
 *
 *    Description:  matrix
 *
 *        Version:  1.0
 *        Created:  2015-8-27 9:38:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  VRer (Linux Programmer), vrml1234@gmail.com
 *        Company:  VrmlStudio.inc
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <highgui.h>
#include <time.h>

int main (int argc, char **argv)
{
	int i, j;
	int nrow = 3;
	int ncol = 3;
	CvMat *src, *dst, *mul;
	double det;
	CvRNG rng = cvRNG (time (NULL));      /* �����γ��ڻ� */
	// (1) ���ФΥ���_��
	src = cvCreateMat (nrow, ncol, CV_32FC1);
	dst = cvCreateMat (ncol, nrow, CV_32FC1);
	mul = cvCreateMat (nrow, nrow, CV_32FC1);
	// (2) ����src�����������
	printf ("src\n");
	cvmSet (src, 0, 0, 1);
	for (i = 0; i < src->rows; i++)
	{
		for (j = 0; j < src->cols; j++)
		{
			cvmSet (src, i, j, cvRandReal (&rng));
			printf ("% lf\t", cvmGet (src, i, j));
		}
		printf ("\n");
	}
	// (3) ����src�������Ф����ƣ�����dst�˴���
	det = cvInvert (src, dst, CV_SVD);
	// (4) ����src������ʽ���ʾ
	printf ("det(src)=%lf\n", det);
	// (5) ����dst�α�ʾ
	printf ("dst\n");
	for (i = 0; i < dst->rows; i++)
	{
		for (j = 0; j < dst->cols; j++)
		{
			printf ("% lf\t", cvmGet (dst, i, j));
		}
		printf ("\n");
	}
	// (6) ����src��dst�ηe��Ӌ�㤷�ƴ_�J
	cvMatMul (src, dst, mul);
	printf ("mul\n");
	for (i = 0; i < mul->rows; i++)
	{
		for (j = 0; j < mul->cols; j++)
		{
			printf ("% lf\t", cvmGet (mul, i, j));
		}
		printf ("\n");
	}
	// (7) ���ФΥ�����_��
	cvReleaseMat (&src);
	cvReleaseMat (&dst);
	cvReleaseMat (&mul);
	return 0;
}

/* end of matrix.cpp */
