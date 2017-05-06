#include <stdio.h>
#include <cv.h>
#include <highgui.h> 

IplImage *origImage, *bayerImage, *BImage;
CvScalar scalar;

void main()
{
	unsigned char bayer[600][480];
	int i, j;

	origImage = cvLoadImage("flower.jpg", 1); 
	bayerImage = cvCreateImage(cvSize(480, 600), IPL_DEPTH_8U, 3);  
	BImage = cvCreateImage(cvSize(480, 600), IPL_DEPTH_8U, 3);
	
	for(i = 0; i <= 559; i = i+2)
	{
		for(j = 0; j <=479 ; j = j+2)
		{
			
			scalar = cvGet2D(origImage, i, j);
			bayer[i][j] = (unsigned char)scalar.val[1];
			scalar = CV_RGB(0, bayer[i][j], 0);
			cvSet2D(bayerImage, i, j, scalar);

			//[i][j+1]
			scalar = cvGet2D(origImage, i, j+1);
			bayer[i][j+1] = (unsigned char)scalar.val[0];
			scalar = CV_RGB(0, 0, bayer[i][j+1]);
			cvSet2D(bayerImage, i, j+1, scalar);

			// [i+1][j]
			scalar = cvGet2D(origImage, i+1, j);
			bayer[i+1][j] = (unsigned char)scalar.val[2];
			scalar = CV_RGB(bayer[i+1][j], 0, 0);
			cvSet2D(bayerImage, i+1, j, scalar);

			// [i+1][j+1]
			scalar = cvGet2D(origImage, i+1, j+1);
			bayer[i+1][j+1] = (unsigned char)scalar.val[1];
			scalar = CV_RGB(0, bayer[i+1][j+1], 0);
			cvSet2D(bayerImage, i+1, j+1, scalar);
		}
	}

	//bilinear interpolation
	for(i = 0; i <= 559; i ++)
	{
		for(j = 0; j <= 479; j ++)
		{
			if(i == 0 || i == 559 || j == 0 || j== 479) 
				scalar = cvGet2D(bayerImage, i, j);
			else
			{
				if (i%2 == 0 && j%2 == 0) 
					scalar = CV_RGB((bayer[i-1][j]+bayer[i+1][j])/2, bayer[i][j], (bayer[i][j-1]+bayer[i][j+1])/2);
				else if(i%2 == 0 && j%2 == 1) 
					scalar = CV_RGB((bayer[i-1][j-1]+bayer[i+1][j-1]+bayer[i+1][j+1]+bayer[i-1][j+1])/4, (bayer[i-1][j]+bayer[i][j-1]+bayer[i+1][j]+bayer[i][j+1])/4, bayer[i][j]);
				else if(i%2 == 1 && j%2 == 0) 
					scalar = CV_RGB(bayer[i][j], (bayer[i-1][j]+bayer[i][j-1]+bayer[i+1][j]+bayer[i][j+1])/4, (bayer[i-1][j-1]+bayer[i+1][j-1]+bayer[i+1][j+1]+bayer[i-1][j+1])/4);
				else 
					scalar = CV_RGB((bayer[i][j-1]+bayer[i][j+1])/2, bayer[i][j], (bayer[i-1][j]+bayer[i+1][j])/2);
			}
			cvSet2D(BImage, i, j, scalar);
		}
	}

	
	cvNamedWindow("Show origImage", 1); 
	cvShowImage("Show origImage", origImage);
	cvSaveImage("orgImage.jpg", origImage);
	cvNamedWindow("Show bayerImage", 1); 
	cvShowImage("Show bayerImage", bayerImage);
	cvSaveImage("bayerImage.jpg", bayerImage);
	cvNamedWindow("Show BImage", 1); 
	cvShowImage("Show BImage", BImage);
	cvSaveImage("BImage.jpg", BImage);

	cvWaitKey(0);
 
	cvDestroyWindow("Show origImage");
	cvReleaseImage(&origImage);
	cvDestroyWindow("Show bayerImage");
	cvReleaseImage(&bayerImage);
	cvDestroyWindow("Show BImage");
	cvReleaseImage(&BImage);
	
	
}