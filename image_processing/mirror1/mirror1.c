#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

int bytes_per_pixel(const IplImage* image);
void process(IplImage *source, IplImage *destination);

int main(int argc, char** argv) {
  CvCapture* capture = 0;
  IplImage* image = 0;
  IplImage* result = 0;
  const char* windowName = "mirror 1";

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

int bytes_per_pixel(const IplImage* image) {
  return (((image->depth & 255) / 8) * (image->nChannels));
}

void process(IplImage *source, IplImage *destination) {
  uchar *pS, *pD, *pS2;
  uchar *dataS, *dataD, *dataS2;
  int bpp;
  int step;
  CvSize size;
  int x, y;

  bpp = bytes_per_pixel(source);

  cvGetRawData(source, &dataS, &step, &size);
  cvGetRawData(destination, &dataD, NULL, NULL);

  for (y = 0; y < size.height; y++) {
    pS = dataS + step * y;
    pD = dataD + step * y + bpp * size.width;
    pS2 = dataS + step * y + bpp * size.width;
    for (x = 0; x < size.width; x++) {
      if (y > size.height/4 && y < size.height*3/4 && x > size.width/4 && x < size.width*3/4) {
        *pD = *pS;
        *(pD + 1) = *(pS + 1);
        *(pD + 2) = *(pS + 2);
      } else {
        *pD = *pS2;
        *(pD + 1) = *(pS2 + 1);
        *(pD + 2) = *(pS2 + 2);
      }
      pS += bpp;
      pD -= bpp;
      pS2 -= bpp;
    }
  }
}

