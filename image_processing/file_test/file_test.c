#include <stdio.h>
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv) {
  IplImage* image;
  const char* windowName = "file view";

  if (argc == 2 && (image = cvLoadImage(argv[1], 1)) != 0) {
    cvNamedWindow(windowName, 1);
    cvShowImage(windowName, image);

    cvWaitKey(0);
    cvDestroyWindow(windowName);
    cvReleaseImage(&image);

    return 0;
  }
  fprintf(stderr, "invalid arguments or file is not found\n");
  return -1;
}

