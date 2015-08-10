// CvRotate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
	IplImage* img;
	IplImage* img2;
	char imgfile[] = "lena.jpg";
	
	CvMat* rotationMat; //Ðý×ª¾ØÕó
	
	//Í¼Ïñ¶ÁÈ¡
	img = cvLoadImage(imgfile, CV_LOAD_IMAGE_COLOR);
	img2 = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
	
	rotationMat = cvCreateMat(2,3,CV_32FC1);
	
	cv2DRotationMatrix(cvPoint2D32f(img->height/2,img->width/2),30,1,rotationMat);
	
	cvWarpAffine(img,img2,rotationMat);
	
	cvNamedWindow ("lena", CV_WINDOW_AUTOSIZE);
	cvShowImage ("lena", img2);
	cvWaitKey (0);
	cvDestroyWindow("lena");
	
	cvReleaseImage(&img);
	cvReleaseImage(&img2);
	
	cvReleaseMat(&rotationMat);
	
	return 0;
}

//end of CvRotate.cpp