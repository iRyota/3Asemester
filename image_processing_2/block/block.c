#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

void process(IplImage *source, IplImage *destination);

int main(int argc, char** argv) {
  CvCapture* capture = 0;
  IplImage* input = 0;
  IplImage* output = 0;
  int tick = 0, prev_tick = 0;
  double now = 0.0;
  CvFont font;
  char buffer[256];
  const char* windowName = "block";

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
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0.0, 1, 0);
  input = cvQueryFrame(capture);
  if (!input) {
    fprintf(stderr, "Could not query frame...\n");
    return -1;
  }
  output = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 3);

  while (1) {
    input = cvQueryFrame(capture);
    if (!input) {
      fprintf(stderr, "Could not query frame...\n");
      break;
    }

    process(input, output);

    sprintf(buffer, "%3.1lfms", now / 1000);
    cvPutText(output, buffer, cvPoint(50, 150), &font, CV_RGB(255, 0, 0));

    cvShowImage(windowName, output);

    if (cvWaitKey(10) >= 0) {
      break;
    }

    tick = cvGetTickCount();
    now = (tick - prev_tick) / cvGetTickFrequency();
    prev_tick = tick;
  }

  cvReleaseImage(&output);

  cvReleaseCapture(&capture);
  cvDestroyWindow(windowName);

  return 0;
}

void process(IplImage *source, IplImage *destination) {
  uchar *dataS, *dataD;
  int bpp = 3;
  int step;
  CvSize size;
  int x, y;
  int bsize = 30;
  double weight = 1.0 / bsize / bsize;

  /* for outer region */
  cvCopy(source, destination, NULL);

  cvGetRawData(source, &dataS, &step, &size);
  cvGetRawData(destination, &dataD, NULL, NULL);

  for (y = size.height / 4; y < (size.height * 3 / 4); y += bsize) {
    for (x = size.width / 4; x < (size.width * 3 / 4); x += bsize) {
      double b = 0.0;
      double g = 0.0;
      double r = 0.0;
      int i;
      int j;

      for (j = 0; j < bsize; j++) {
        for (i = 0; i < bsize; i++) {
          uchar* pN = dataS + step * (y + j - 1) + bpp * (x + i - 1);
          b += *pN;
          g += *(pN + 1);
          r += *(pN + 2);
        }
      }

      b *= weight;
      g *= weight;
      r *= weight;

      for (j = 0; j < bsize; j++) {
        for (i = 0; i < bsize; i++) {
          uchar* pD = dataD + step * (y + j - 1) + bpp * (x + i - 1);
          *pD = b;
          *(pD + 1) = g;
          *(pD + 2) = r;
        }
      }
    }
  }
}

