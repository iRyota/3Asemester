#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

typedef struct __runlength__ {
  int minID;
  int start;
  int end;
  int position;
  int area;
  int childMinID;
  int childMaxID;
  int childSize;
} RunLength;

typedef struct __rlwithsize__ {
  struct __runlength__* content;
  int numOfRuns;
} RunLengthWithSize;

typedef struct __rlidatpos__ {
  int position;
  int size;
  int min;
  int max;
} RunsPrevIDs;

typedef struct __areaid__ {
  int area;
  int originalID;
} Area;

RunLengthWithSize*
encodeRunLength(unsigned char* input, int width, int height, int n8,
    int xOffset, int yOffset, int regionWidth, int regionHeight,
    unsigned char* rendered);

void setMinID(RunLength* runlength, int minID, int rootID);

void assignOutput(int numOfRuns, RunLength* patches, int width, int height,
    unsigned char* output);

int compareArea(const void* a, const void* b);

void validateConnectivity(int numOfRuns, RunLength* patches);

int intmin(int a, int b);

int isConnect(RunLength* prev, RunLength* now, int n8);

void appendChild(RunLength* patches, int parentID, int childID);

void releaseObject(RunLengthWithSize* rl);

void initRunLength(RunLength* runlengthes, int id, int x, int y);

RunLengthWithSize*
encodeRunLength(unsigned char* input, int width, int height, int n8,
    int xOffset, int yOffset, int regionWidth, int regionHeight,
    unsigned char* rendered) {

  const int BUF_SIZE = height * 5;
  int counter = 0;
  int patchesLength = BUF_SIZE;
  int y = 0;
  RunsPrevIDs* prevIds = NULL;
  RunLength* patches = NULL;
  RunLengthWithSize* rl = NULL;
  int regionWidthMax;
  int regionHeightMax;

  prevIds = (RunsPrevIDs*) malloc(sizeof(RunsPrevIDs) * height);
  patches = (RunLength*) malloc(sizeof(RunLength) * patchesLength);
  rl = (RunLengthWithSize*) malloc(sizeof(RunLengthWithSize) * 1);
  regionWidthMax =
      regionWidth + xOffset > width ? width : regionWidth + xOffset;
  regionHeightMax =
      regionHeight + yOffset > height ? height : regionHeight + yOffset;

  for (y = yOffset; y < regionHeightMax; y++) {
    int offset = y * width;
    unsigned char* p = input + offset;
    int x = xOffset;
    int minIDtoSearch = 0;

    if (y > yOffset) {
      minIDtoSearch = prevIds[y - 1].min;
    }
    prevIds[y].position = y;
    prevIds[y].min = counter;
    prevIds[y].max = counter;
    prevIds[y].size = 0;

    while (x < regionWidthMax) {

      while ((x < regionWidthMax) && (!*(p + x))) {
        rendered[offset + x] = 0;
        x++;
      }

      if ((x < regionWidthMax) && *(p + x)) {
        rendered[offset + x] = 0;
        initRunLength(patches, counter, x, y);
        prevIds[y].size++;
        prevIds[y].max = counter;

        while ((x < regionWidthMax) && (*(p + x))) {
          rendered[offset + x] = 0;
          x++;
        }
        patches[counter].end = x - 1;
        patches[counter].area = patches[counter].end - patches[counter].start
            + 1;

        if (y > yOffset && prevIds[y - 1].size > 0) {
          int onceConnected = 0;
          int checkID = 0;
          int maxID = prevIds[y - 1].max;

          for (checkID = minIDtoSearch; checkID <= maxID; checkID++) {
            int connect = isConnect(&patches[checkID], &patches[counter], n8);

            if (connect == 0) {

              if (onceConnected != 0) {
                minIDtoSearch = checkID - 1;
                break;
              } else {
                int n4or8 = n8 ? 1 : 0;

                if (patches[counter].end + n4or8 < patches[checkID].start) {
                  break;
                }
                continue;
              }
            } else {
              onceConnected = 1;
              appendChild(patches, counter, checkID);
              minIDtoSearch = checkID;
            }
          } // scan patches at previous line.
        } else {
        } //end for connectivity check
        counter++;

        if (counter >= patchesLength) {
          patchesLength += BUF_SIZE;
          patches = (RunLength*) realloc(patches,
              sizeof(RunLength) * patchesLength);
        }
      }
    }
  }
  validateConnectivity(counter, patches);
  free(prevIds);
  rl->content = patches;
  rl->numOfRuns = counter;
  return rl;
}

void setMinID(RunLength* runlengthes, int minID, int rootID) {

  if (runlengthes[rootID].minID <= minID) {
    return;
  } else {
    int id = 0;
    runlengthes[rootID].minID = minID;

    if (runlengthes[rootID].childSize > 0) {

      for (id = runlengthes[rootID].childMinID;
          id <= runlengthes[rootID].childMaxID; id++) {
        setMinID(runlengthes, minID, id);
      }
    }
  }
}

void assignOutput(int numOfRuns, RunLength* patches, int width, int height,
    unsigned char* output) {
  int c = 0;
  int label = 0;
  int* map = (int*) malloc(sizeof(int) * numOfRuns);
  int* map2; //contains index info arter sorting by area size
  Area* area;

  for (c = 0; c < numOfRuns; c++) {
    map[c] = -1;
  }

  for (c = 0; c < numOfRuns; c++) {
    if (map[patches[c].minID] < 0) {
      map[patches[c].minID] = label++;
    }
  }

  area = (Area*) malloc(sizeof(Area) * label);

  for (c = 0; c < label; c++) {
    area[c].area = 0;
    area[c].originalID = 0;
  }

  for (c = 0; c < numOfRuns; c++) {
    int id = map[patches[c].minID];
    area[id].area += patches[c].area;
    area[id].originalID = id;
  }
  qsort(area, label, sizeof(area[0]), compareArea);
  map2 = (int*) malloc(sizeof(int) * label);

  for (c = 0; c < label; c++) {
    map2[area[c].originalID] = c;
  }

  for (c = 0; c < numOfRuns; c++) {
    int y = patches[c].position;
    int offsetstart = y * width + patches[c].start;
    int offsetend = y * width + patches[c].end;
    int offset = offsetstart;

    for (offset = offsetstart; offset < offsetend; offset++) {
      int density = map2[map[patches[c].minID]] + 1;

      if (density < 256) {
        *(output + offset) = (unsigned char) density;
      }
    }
  }
  free(map);
  free(area);
  free(map2);
}

int compareArea(const void* a, const void* b) {
  Area* aa = (Area*) a;
  Area* bb = (Area*) b;
  return bb->area - aa->area;
}

int intmin(int a, int b) {
  return a > b ? b : a;
}

void validateConnectivity(int numOfLength, RunLength* patches) {
  int ccc = 0;
  int isChanged = 1;

  while (isChanged) {
    isChanged = 0;

    for (ccc = 0; ccc < numOfLength; ccc++) {

      if (patches[ccc].childSize > 0) {
        int diff = 0;
        int minimum = patches[ccc].minID;
        int id = 0;

        for (id = patches[ccc].childMinID; id <= patches[ccc].childMaxID;
            id++) {

          if (minimum > patches[id].minID) {
            minimum = patches[id].minID;
            diff = 1;
            isChanged = 1;
          }
        }

        if (diff) {
          int id = 0;
          patches[ccc].minID = minimum;

          for (id = patches[ccc].childMinID; id <= patches[ccc].childMaxID;
              id++) {
            setMinID(patches, minimum, id);
          }
        }
      }
    }
  }
}

int isConnect(RunLength* prev, RunLength* now, int n8) {
  int n4or8 = n8 ? 1 : 0;

  if ((prev->start >= (now->start - n4or8))
      && (prev->start <= (now->end + n4or8))) {
    return 1;
  } else if ((prev->start < (now->start - n4or8))
      && (prev->end >= (now->start - n4or8))) {
    return 1;
  } else {
    return 0;
  }
}

void appendChild(RunLength* patches, int parentID, int childID) {

  if (patches[parentID].childSize == 0) {
    patches[parentID].childMinID = childID;
    patches[parentID].childMaxID = childID;
    patches[parentID].childSize++;
    patches[parentID].minID = patches[childID].minID;
  } else {
    int minID = intmin(patches[childID].minID, patches[parentID].minID);
    setMinID(patches, minID, parentID);
    setMinID(patches, minID, childID);
    patches[parentID].childSize++;
    patches[parentID].childMaxID = childID;
  }
}

/*
 row base
 pixel info.: 
 horizontal x, vertical axis y;
 input[width * y + x];
 output[width* y + x];
 */
void labeling(unsigned char* input, int width, int height, int n8, int xOffset,
    int yOffset, int regionWidth, int regionHeight, unsigned char* output) {
  RunLengthWithSize* rl = NULL;
  rl = encodeRunLength(input, width, height, n8, xOffset, yOffset, regionWidth,
      regionHeight, output);
  assignOutput(rl->numOfRuns, rl->content, width, height, output);
  releaseObject(rl);
}

void releaseObject(RunLengthWithSize* rl) {
  free(rl->content);
  free(rl);
}

void initRunLength(RunLength* runlengthes, int id, int x, int y) {
  runlengthes[id].position = y;
  runlengthes[id].childMaxID = 0;
  runlengthes[id].childMinID = 0;
  runlengthes[id].childSize = 0;
  runlengthes[id].minID = id;
  runlengthes[id].start = x;
}

#define ESCAPE 27 
#define RETURN 13

void process(IplImage* input, IplImage* output);

int main(int argc, char* argv[]) {
  CvCapture* capture = NULL;
  IplImage* src = NULL;
  IplImage* gray = NULL;
  IplImage* bin = NULL;
  IplImage* labelImg = NULL;
  IplImage* rendered = NULL;
  char* windowName = "labeling";

  if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0]))) {
    capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);
  } else if (argc == 2) {
    capture = cvCreateFileCapture(argv[1]);
  }

  if (!capture) {
    fprintf(stderr, "ERROR: capture is NULL \n");
    return (-1);
  }

  src = cvQueryFrame(capture);
  if (src == NULL) {
    fprintf(stderr, "Could not grab and retrieve frame...\n");
    return (-1);
  }

  rendered = cvCloneImage(src);

  gray = cvCreateImage(cvSize(src->width, src->height), src->depth, 1);

  gray->origin = src->origin;

  bin = cvCloneImage(gray);
  labelImg = cvCloneImage(bin);

  cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

  while (1) {
    src = cvQueryFrame(capture);
    if (!src) {
      fprintf(stderr, "Could not grab and retrieve frame...\n");
      return -1;
    }

    cvCvtColor(src, gray, CV_BGR2GRAY);
    cvThreshold(gray, bin, 130, 255, 0);

    cvCopyImage(src, rendered);

    process(bin, rendered);
    cvShowImage(windowName, rendered);

    if (cvWaitKey(2) == ESCAPE) {
      break;
    }
  }

  cvDestroyWindow(windowName);

  cvReleaseCapture(&capture);

  cvReleaseImage(&gray);
  cvReleaseImage(&bin);
  cvReleaseImage(&rendered);

  return 0;
}

void process(IplImage* input, IplImage* colored) {
  CvSize size;
  int step;
  uchar* data;
  uchar* labeldata;
  uchar* colordata;
  int id = 0;
  int bpp = 0;

  IplImage* label = cvCloneImage(input);

  cvGetRawData(input, &data, &step, &size);
  cvGetRawData(label, &labeldata, NULL, NULL);
  cvGetRawData(colored, &colordata, NULL, NULL);

  labeling(data, input->width, input->height, 1, 0, 0, input->width,
      input->height, labeldata);

  bpp = 3;

  for (id = 0; id < size.width * size.height; id++) {
    if (labeldata[id] == 1) {
      // int mod6 = labeldata[id] % 6;
      // int r = 255;
      // int g = 255;
      // int b = 255;
      // switch (mod6) {
      // case 0:
      //   b = 0;
      //   break;
      // case 1:
      //   r = 0;
      //   break;
      // case 2:
      //   g = 0;
      //   break;
      // case 3:
      //   b = r = 0;
      //   break;
      // case 4:
      //   r = g = 0;
      //   break;
      // case 5:
      //   g = b = 0;
      //   break;
      // }
      // colordata[id * bpp] = b;
      // colordata[id * bpp + 1] = g;
      // colordata[id * bpp + 2] = r;
    } else {
      colordata[id * bpp] = 0;
      colordata[id * bpp + 1] = 0;
      colordata[id * bpp + 2] = 0;
    }
  }

  cvReleaseImage(&label);
}
