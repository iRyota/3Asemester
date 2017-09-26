#include <stdio.h>
#include <ctype.h>
#include "cv.h"
#include "highgui.h"

int bytes_per_pixel(const IplImage* image);
void process(IplImage *source, IplImage *destination);
int calc_otsu_threshold(int histogram[256]);

IplImage* background = 0;

int threshold = 200;

int main(int argc, char** argv) {
  CvCapture* capture = 0;
  IplImage* image = 0;
  IplImage* result = 0;
  int tick = 0;
  int prev_tick = 0;

  double now = 0.0;
  CvFont font;
  char buffer[256];
  int i;

  const char* windowNameBG = "background";
  const char* windowNameBS = "foreground";
  const char* windowNameCI = "current image";

  if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0]))) {
    capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);
  } else if (argc == 2) {
    capture = cvCreateFileCapture(argv[1]);
  }
  if (!capture) {
    fprintf(stderr, "ERROR: capture is NULL \n");
    return -1;
  }

  image = cvQueryFrame(capture);
  if (!image) {
    fprintf(stderr, "Could not query frame...\n");
    return -1;
  }

  result = cvCloneImage(image);
  background = cvCloneImage(image);

  cvNamedWindow(windowNameBG, CV_WINDOW_AUTOSIZE);
  cvNamedWindow(windowNameCI, CV_WINDOW_AUTOSIZE);
  cvNamedWindow(windowNameBS, CV_WINDOW_AUTOSIZE);

  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0.0, 1, 0);

  while (1) {
    image = cvQueryFrame(capture);
    if (!image) {
      fprintf(stderr, "Could not query frame...\n");
      break;
    }

    process(image, result);

    sprintf(buffer, "%3.1lfms", now / 1000);
    cvPutText(result, buffer, cvPoint(50, 150), &font, CV_RGB(255, 0, 0));

    cvShowImage(windowNameBS, result);
    cvShowImage(windowNameBG, background);
    cvShowImage(windowNameCI, image);

    if (cvWaitKey(10) >= 0) {
      break;
    }

    tick = cvGetTickCount();
    now = (tick - prev_tick) / cvGetTickFrequency();
    prev_tick = tick;
  }

  cvReleaseImage(&background);
  cvReleaseImage(&result);

  cvReleaseCapture(&capture);
  cvDestroyWindow(windowNameBS);
  cvDestroyWindow(windowNameBG);
  cvDestroyWindow(windowNameCI);

  return 0;
}

int bytes_per_pixel(const IplImage* image) {
  return ((((image)->depth & 255) / 8) * (image)->nChannels);
}

void process(IplImage *source, IplImage *destination) {
  uchar* pS;
  uchar* pD;
  uchar* pB;
  uchar* dataS;
  uchar* dataD;
  uchar* dataB;
  int bppS;
  int bppD;
  int bppB;

  int step;
  CvSize size;
  int x;
  int y;
  double intensityS;
  int intensityS_d;
  double intensityB;
  int intensityB_d;
  int intensity_diff;
  int histogram[256];
  int i = 0;

  cvCopy(source, destination, NULL);

  bppS = bytes_per_pixel(source);
  bppD = bytes_per_pixel(destination);
  bppB = bytes_per_pixel(background);

  cvGetRawData(source, &dataS, &step, &size);
  cvGetRawData(destination, &dataD, NULL, NULL);
  cvGetRawData(background, &dataB, NULL, NULL);

  for (i = 0; i < 256; i++) {
    histogram[i] = 0;
  }

  pS = dataS;
  pD = dataD;
  pB = dataB;

  for (y = 0; y < size.height; y++) {
    for (x = 0; x < size.width; x++) {
      intensityS = (0.114 * pS[0] + 0.587 * pS[1] + 0.299 * pS[2]) / 255.0;
      intensityS_d = (int) (219.0 * intensityS) + 16;
      intensityB = (0.114 * pB[0] + 0.587 * pB[1] + 0.299 * pB[2]) / 255.0;
      intensityB_d = (int) (219.0 * intensityB) + 16;
      intensity_diff = intensityS_d - intensityB_d; //Fill this value
      intensity_diff = intensity_diff < 0 ? intensity_diff*-1 : intensity_diff; //Fill these
      histogram[intensity_diff]++;
      pS += bppS;
      pD += bppD;
      pB += bppB;
    }
  }

  threshold = calc_otsu_threshold(histogram);
  // threshold = 100;
  printf("%d\n", threshold);

  pS = dataS;
  pD = dataD;
  pB = dataB;

  for (y = 0; y < size.height; y++) {
    for (x = 0; x < size.width; x++) {
      intensityS = (0.114 * pS[0] + 0.587 * pS[1] + 0.299 * pS[2]) / 255.0;
      intensityS_d = (int) (219.0 * intensityS) + 16;
      intensityB = (0.114 * pB[0] + 0.587 * pB[1] + 0.299 * pB[2]) / 255.0;
      intensityB_d = (int) (219.0 * intensityB) + 16;
      intensity_diff = intensityS_d - intensityB_d; //Fill this value
      intensity_diff = intensity_diff < 0 ? intensity_diff*-1 : intensity_diff; // Fill these

      if (intensity_diff > threshold) {
        *pD = *pS;
        *(pD + 1) = *(pS + 1);
        *(pD + 2) = *(pS + 2);
      } else {
        *pD = 0;
        *(pD + 1) = 255;
        *(pD + 2) = 0;
      }

      pS += bppS;
      pD += bppD;
      pB += bppB;
    }
  }

}

int calc_otsu_threshold(int histogram[256]) {
  //
  //Fill this function (Exercise 11)
  //
  int i, t;
  float N=0, myu=0, max_sigma=0, max_t=0, mt=0, qt=0, sigma2;
  for (i=0; i<256; i++) {
    N+=histogram[i];
    myu+=i*histogram[i];
  }
  myu=myu/N;
  for(t=0;t<256;t++){
    mt+=histogram[t];
    qt+=t*histogram[t];
    sigma2 = (qt-mt*myu)*(qt-mt*myu)/(mt*(N-mt));
    if(max_sigma < sigma2) {
      max_sigma = sigma2;
      max_t = t;
    }
  }
  float threshold;
  threshold = max_t;
  return threshold;
}
