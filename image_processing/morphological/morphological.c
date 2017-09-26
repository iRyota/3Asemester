#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

int bytes_per_pixel(const IplImage* image);
void dilation(IplImage *source, IplImage *destination);
void erosion(IplImage *source, IplImage *destination);
void opening(IplImage *source, IplImage *destination);
void closing(IplImage *source, IplImage *destination);

int threshold = 100;

int main(int argc, char* argv[]) {
  CvCapture* capture = 0;
  IplImage* image = 0;
  IplImage* grayscale_image = 0;
  IplImage* binary = 0;
  IplImage* opened_image = 0;
  IplImage* closed_image = 0;
  IplImage* result = 0;
  const char* windowName = "morpological";

  if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0]))) {
    capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);
  } else if (argc == 2) {
    capture = cvCreateFileCapture(argv[1]);
  }
  if (!capture) {
    fprintf(stderr, "ERROR: capture is NULL \n");
    return -1;
  }

  cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

  image = cvQueryFrame(capture);
  if (!image) {
    fprintf(stderr, "Could not query frame...\n");
    return -1;
  }
  grayscale_image = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
  binary = cvCloneImage(grayscale_image);
  opened_image = cvCloneImage(grayscale_image);
  closed_image = cvCloneImage(grayscale_image);

  result = cvCloneImage(image);

  while (1) {
    image = cvQueryFrame(capture);
    if (!image) {
      fprintf(stderr, "Could not query frame...\n");
      break;
    }

    cvCvtColor(image, grayscale_image, CV_BGR2GRAY);
    cvThreshold(grayscale_image, binary, threshold, 255.0, CV_THRESH_BINARY);

    opening(binary, opened_image);
    closing(opened_image, closed_image);

    cvShowImage(windowName, closed_image);

    if (cvWaitKey(10) >= 0) {
      break;
    }
  }

  cvReleaseImage(&grayscale_image);
  cvReleaseImage(&binary);
  cvReleaseImage(&opened_image);
  cvReleaseImage(&closed_image);

  cvReleaseCapture(&capture);
  cvDestroyWindow(windowName);

  return 0;
}

int bytes_per_pixel(const IplImage* image) {
  return ((((image)->depth & 255) / 8) * (image)->nChannels);
}

void dilation(IplImage *source, IplImage *destination) {
  uchar *pS;
  uchar *pD;
  uchar *dataS;
  uchar *dataD;
  int bppS;
  int bppD;
  int stepS;
  int stepD;
  CvSize sizeS;
  CvSize sizeD;
  int x;
  int y;
  int i;
  int j;

  int intensityS_d;
  int intensityD_d;
  int intensityS_d_maximum;

  cvCopy(source, destination, NULL);

  bppS = bytes_per_pixel(source);
  bppD = bytes_per_pixel(destination);

  cvGetRawData(source, &dataS, &stepS, &sizeS);
  cvGetRawData(destination, &dataD, &stepD, &sizeD);

  for (y = sizeS.height / 4 + 1; y < (sizeS.height * 3 / 4) - 1; y++) {
    for (x = sizeS.width / 4 + 1; x < (sizeS.width * 3 / 4) - 1; x++) {
      pD = dataD + stepD * y + bppD * x;
      pS = dataS + stepS * y + bppS * x;
      *pD = *pS;
      intensityS_d_maximum = *pS;
      for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
          if ((i != 1) || (j != 1)) {
            pS = dataS + stepS * (y + j - 1) + bppS * (x + i - 1);
            intensityS_d = *pS;
            if (intensityS_d > intensityS_d_maximum) {
              *pD = intensityS_d;
              intensityS_d_maximum = intensityS_d;
            }
          }
        }
      }
    }
  }
}

void erosion(IplImage *source, IplImage *destination) {
  uchar *pS;
  uchar *pD;
  uchar *dataS;
  uchar *dataD;
  int bppS;
  int bppD;
  int stepS;
  int stepD;
  CvSize sizeS;
  CvSize sizeD;
  int x;
  int y;
  int i;
  int j;
  int intensityS_d;
  int intensityD_d;
  int intensityS_d_minimum;

  cvCopy(source, destination, NULL);

  bppS = bytes_per_pixel(source);
  bppD = bytes_per_pixel(destination);

  cvGetRawData(source, &dataS, &stepS, &sizeS);
  cvGetRawData(destination, &dataD, &stepD, &sizeD);

  for (y = sizeS.height / 4 + 1; y < sizeS.height * 3 / 4 - 1; y++) {
    for (x = sizeS.width / 4 + 1; x < sizeS.width * 3 / 4 - 1; x++) {
      pD = dataD + stepD * y + bppD * x;
      pS = dataS + stepS * y + bppS * x;
      *pD = *pS;
      intensityS_d_minimum = *pS;
      for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
          if ((i != 1) || (j != 1)) {
            pS = dataS + stepS * (y + j - 1) + bppS * (x + i - 1);
            intensityS_d = *pS;
            if (intensityS_d < intensityS_d_minimum) {
              *pD = intensityS_d;
              intensityS_d_minimum = intensityS_d;
            }
          }
        }
      }
    }
  }
}

void opening(IplImage *source, IplImage *destination) {
  IplImage* temporary = cvCloneImage(source);

  erosion(source, temporary);
  erosion(temporary, destination);
  dilation(destination, temporary);
  dilation(temporary, destination);

  cvReleaseImage(&temporary);
}

void closing(IplImage *source, IplImage *destination) {
  IplImage * temporary = cvCloneImage(source);

  dilation(source, temporary);
  dilation(temporary, destination);
  erosion(destination, temporary);
  erosion(temporary, destination);

  cvReleaseImage(&temporary);
}

