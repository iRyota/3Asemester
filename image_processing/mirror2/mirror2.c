#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

void process(IplImage *source, IplImage *destination);

int main(int argc, char** argv) {
  CvCapture* capture = 0;
  IplImage* image = 0;
  IplImage* result = 0;
  const char* windowName = "mirror 2";

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

  result = cvCloneImage(image);

  while (1) {
    image = cvQueryFrame(capture);
    if (!image) {
      fprintf(stderr, "Could not query frame...\n");
      break;
    }

    process(image, result);
    cvShowImage(windowName, result);

    if (cvWaitKey(10) >= 0) {
      break;
    }
  }

  cvReleaseImage(&result);
  cvReleaseCapture(&capture);
  cvDestroyWindow(windowName);

  return 0;
}

void process(IplImage *source, IplImage *destination) {
  CvSize size = cvGetSize(source);
  int x;
  int y;

  for (y = 0; y < size.height; y++) {
    for (x = 0; x < size.width; x++) {
      CvScalar pixelVal = cvGet2D(source, y, size.width - 1 - x);
      cvSet2D(destination, y, x, pixelVal);
    }
  }
}
