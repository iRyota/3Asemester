#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv) {
  CvCapture* capture;
  IplImage* frame;
  IplImage* result;
  IplImage* gray;
  IplImage* binary;
  const char* windowName = "binarize 1";
  double threshold = 100.0;

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

  frame = cvQueryFrame(capture);
  if (!frame) {
    fprintf(stderr, "ERROR: frame is null...\n");
    return -1;
  }

  binary = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
  gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

  while (1) {
    frame = cvQueryFrame(capture);
    if (!frame) {
      fprintf(stderr, "ERROR: frame is null...\n");
      break;
    }

    cvCvtColor(frame, gray, CV_BGR2GRAY);

    cvThreshold(gray, binary, threshold, 255.0, CV_THRESH_BINARY);

    cvShowImage(windowName, binary);

    if (cvWaitKey(10) >= 0) {
      break;
    }
  }

  cvReleaseImage(&binary);
  cvReleaseImage(&gray);
  cvReleaseImage(&result);

  cvReleaseCapture(&capture);
  cvDestroyWindow(windowName);

  return 0;
}

