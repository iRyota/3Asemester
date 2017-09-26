#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv) {
  CvCapture* capture = 0;
  IplImage* color = 0;
  IplImage* planes[3] = { 0, 0, 0 };
  IplImage* frame = 0;
  const char* windowName = "color test";
  int i;
  int tmp;
  double dtmp;

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

  fprintf(stderr, "CV_CAP_PROP_POS_MSEC = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC));
  fprintf(stderr, "CV_CAP_PROP_POS_FRAMES = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES));
  fprintf(stderr, "CV_CAP_PROP_POS_AVI_RATIO = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO));
  fprintf(stderr, "CV_CAP_PROP_FRAME_WIDTH = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
  fprintf(stderr, "CV_CAP_PROP_FRAME_HEIGHT = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
  fprintf(stderr, "CV_CAP_PROP_FPS = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_FPS));
  fprintf(stderr, "CV_CAP_PROP_FOURCC = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_FOURCC));
  fprintf(stderr, "CV_CAP_PROP_FRAME_COUNT = %lf\n",
      cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT));

  while (1) {
    tmp = cvGrabFrame(capture);
    frame = cvRetrieveFrame(capture, 1);
    if (!frame) {
      fprintf(stderr, "ERROR: frame is null...\n");
      break;
    }

    if (!color) {
      for (i = 0; i < 3; i++) {
        planes[i] = cvCreateImage(cvSize(frame->width, frame->height), 8, 1);
      }
      color = cvCreateImage(cvSize(frame->width, frame->height), 8, 3);
    }

    cvSplit(frame, planes[0], planes[1], planes[2], 0);
    cvMerge(planes[1], planes[2], planes[0], 0, color);
    color->origin = frame->origin;

    cvShowImage(windowName, color);

    if (cvWaitKey(10) >= 0) {
      break;
    }
  }

  cvReleaseImage(&color);
  for (i = 0; i < 3; i++) {
    cvReleaseImage(&planes[i]);
  }
  cvReleaseCapture(&capture);
  cvDestroyWindow(windowName);

  return 0;
}

