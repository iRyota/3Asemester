#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv) {
  CvCapture* capture;
  IplImage* frame;
  IplImage* image;
  const char* windowName = "processed image";
  int tick = 0;
  int prev_tick = 0;

  double now = 0.0;
  CvFont font;
  char buffer[256];

  if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0]))) {
    capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);
  } else if (argc == 2) {
    capture = cvCreateFileCapture(argv[1]);
  }
  if (!capture) {
    fprintf(stderr, "ERROR: capture is NULL \n");
    return -1;
  }

  frame = cvQueryFrame(capture);
  if (!frame) {
    fprintf(stderr, "ERROR: frame is null...\n");
    return -1;
  }

  image = cvCloneImage(frame);

  cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0.0, 1, 0);

  while (1) {
    frame = cvQueryFrame(capture);

    if (!frame) {
      fprintf(stderr, "ERROR: frame is null...\n");
      break;
    }
    cvCopyImage(frame, image);

    tick = cvGetTickCount();
    now = (tick - prev_tick) / cvGetTickFrequency();
    sprintf(buffer, "%3.1lfms", now / 1000);
    cvPutText(image, buffer, cvPoint(50, 150), &font, CV_RGB(255, 0, 0));

    cvRectangle(image, cvPoint(100, 100), cvPoint(120, 120), CV_RGB(0, 0, 255),
        1, 8, 0);

    cvCircle(image, cvPoint(240, 240), 20, CV_RGB(255, 0, 0), 3, 8, 0);

    cvShowImage(windowName, image);

    if ((cvWaitKey(10) & 255) == 27) {
      break;
    }

    prev_tick = tick;
  }

  cvReleaseCapture(&capture);
  cvReleaseImage(&image);
  cvDestroyWindow(windowName);

  return 0;
}

