/*
 * intCalib.c
 * written by m. shimosaka, y. sagawa, t. mori
 *
 */

#include <stdio.h>
#include <ctype.h> 
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#define ESCAPE 27 
#define RETURN 10
#define CHESS_ROW_NUM 10
#define CHESS_COL_NUM 7
#define CHESS_ROW_DX 20
#define CHESS_COL_DY 20
#define IMAGE_NUM 10

typedef struct{
  double dx;
  double dy;
  CvSize patternSize;
} ChessBoard;


void store2DCoordinates( CvMat* cornerPoints, CvPoint2D32f* points, 
                        ChessBoard chess, int imageID );
void store3DCoordinates( CvMat* objectPoints, ChessBoard chess, int imageID );

void calibrateCameraIntrinsic ( const char* intrinsicFileName, 
                               CvMat* pointCounts, ChessBoard chess, 
                               CvMat* cornerPoints, CvMat* objectPoints );


int main( int argc, char** argv ){
  CvCapture* capture = NULL;
  IplImage* src = NULL;
  IplImage* smallSrc = NULL;
  IplImage* gray = NULL; 

  CvMat* cornerPoints;
  CvMat* objectPoints;
  CvMat pointsNumMat;
  CvPoint2D32f* points;
  int pointsNum[IMAGE_NUM];
  int currImgNum = 0;

  ChessBoard chess;
  int pointsPerScene;
  int detectedPointsNum;
  int allPointsFound;
  int i, j;
  char key;
  int camID;
  char* windowName = "intrinsic calibration";

  capture = cvCreateCameraCapture(0);

  if( !capture ) {
    fprintf(stderr, "ERROR: capture is NULL \n");
    return(-1);
  }

  chess.dx = CHESS_ROW_DX;
  chess.dy = CHESS_COL_DY;
  chess.patternSize.width = CHESS_ROW_NUM;
  chess.patternSize.height = CHESS_COL_NUM;

  pointsPerScene = chess.patternSize.width * chess.patternSize.height;

  cornerPoints = cvCreateMat(pointsPerScene * IMAGE_NUM, 2, CV_32F);
  objectPoints = cvCreateMat(pointsPerScene * IMAGE_NUM, 3, CV_32F);

  for( i = 0; i < IMAGE_NUM; i++ ) {
    pointsNum[i] = pointsPerScene;
  }

  pointsNumMat = cvMat(IMAGE_NUM, 1, CV_32S, pointsNum);

  points = (CvPoint2D32f*)malloc( sizeof(CvPoint2D32f) * pointsPerScene ) ;

  src = cvQueryFrame(capture);
  if(src == NULL){
    fprintf(stderr, "Could not grab and retrieve frame...\n");
    return(-1);
  }

  smallSrc = cvCloneImage( src );
  
  gray = cvCreateImage(cvSize(smallSrc->width, smallSrc->height), 
		       smallSrc->depth, 1);

  cvNamedWindow( windowName, CV_WINDOW_AUTOSIZE );

  while( 1 ){
    src = cvQueryFrame(capture);
    if( !src ) {
      fprintf(stderr, "Could not grab and retrieve frame...\n");
      return(-1);
    }

    cvCopy( src, smallSrc, NULL );  

    cvCvtColor(smallSrc, gray, CV_BGR2GRAY);
    
    if( cvFindChessboardCorners( gray, chess.patternSize, points, 
        &detectedPointsNum, CV_CALIB_CB_ADAPTIVE_THRESH ) ){
      cvFindCornerSubPix(gray, points, detectedPointsNum, 
        cvSize(5, 5), cvSize(-1, -1), 
        cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.1));
      allPointsFound = 1;
    } else {
      allPointsFound = 0;
    }
    
    cvDrawChessboardCorners( smallSrc, chess.patternSize, points, 
      detectedPointsNum, allPointsFound);

    cvShowImage(windowName, smallSrc);

    key = cvWaitKey(20);
    if(key == RETURN && allPointsFound ){
      store2DCoordinates( cornerPoints, points, chess, currImgNum );
      store3DCoordinates( objectPoints, chess, currImgNum );
      
      currImgNum++;
      fprintf(stderr, "scene %d is saved\n", currImgNum );

      if(currImgNum == IMAGE_NUM){
        calibrateCameraIntrinsic("intrinsic_param.txt", 
          &pointsNumMat, chess, cornerPoints, objectPoints );
        break;
      }
    } else if(key == ESCAPE) {
      break;
    }
  }

  cvDestroyWindow( windowName );

  cvReleaseCapture(&capture);

  free(points);
  cvReleaseMat( &cornerPoints );
  cvReleaseMat( &objectPoints );

  cvReleaseImage( &smallSrc );
  cvReleaseImage( &gray );

  return( 0 );
}

void store2DCoordinates( CvMat* cornerPoints, CvPoint2D32f* points, 
                        ChessBoard chess, int imageID ) {
  int i = 0;
  int pointsPerScene = chess.patternSize.width * chess.patternSize.height;
  int offset = imageID * pointsPerScene;

  for ( i = 0; i < pointsPerScene; i++ ) {
    cvmSet(cornerPoints, offset + i, 0, points[i].x);
    cvmSet(cornerPoints, offset + i, 1, points[i].y);
  }
}

void store3DCoordinates( CvMat* objectPoints,ChessBoard chess, int imageID ) {
  int i;
  int pointsPerScene = chess.patternSize.width * chess.patternSize.height;

  for( i = 0; i < pointsPerScene; i++){
    double dx = chess.dx;
    double dy = chess.dy;

    int rowNum = chess.patternSize.width;
    int k = i % rowNum;
    int l = i / rowNum;
    int offset = pointsPerScene * imageID;
    cvmSet(objectPoints, offset + i, 0, k*dx );
    cvmSet(objectPoints, offset + i, 1, l*dy );
    cvmSet(objectPoints, offset + i, 2, 0.0 );
  }
}

void saveIntrinsicParams(const char* fileName, 
                         CvMat* intrinsic, CvMat* distortion ){
  //
  FILE* file;
  if((file = fopen(fileName, "w")) == NULL){
    fprintf(stderr, "file open error\n");
    return;
  }

  fprintf(file, "%lf %lf %lf\n",
    cvmGet(intrinsic, 0, 0),
    cvmGet(intrinsic, 0, 1), 
    cvmGet(intrinsic, 0, 2));

  fprintf(file, "%lf %lf %lf\n",
    cvmGet(intrinsic, 1, 0), 
    cvmGet(intrinsic, 1, 1), 
    cvmGet(intrinsic, 1, 2));

  fprintf(file, "%lf %lf %lf\n",
    cvmGet(intrinsic, 2, 0), 
    cvmGet(intrinsic, 2, 1), 
    cvmGet(intrinsic, 2, 2));

  fprintf(file, "%lf %lf %lf %lf\n", 
    cvmGet(distortion, 0, 0), 
    cvmGet(distortion, 1, 0), 
    cvmGet(distortion, 2, 0), 
    cvmGet(distortion, 3, 0));

  fclose(file);

}

void calibrateCameraIntrinsic ( const char* intrinsicFileName, 
                               CvMat* pointCounts, ChessBoard chess, 
                               CvMat* cornerPoints, CvMat* objectPoints ){

  CvMat* intrinsic = cvCreateMat(3, 3, CV_64F);
  CvMat* distortion = cvCreateMat(4, 1, CV_64F);

  cvCalibrateCamera2( objectPoints, cornerPoints, pointCounts, 
    chess.patternSize, intrinsic, distortion, NULL, NULL, 0 );

  saveIntrinsicParams( intrinsicFileName, intrinsic, distortion );
  
 
  cvReleaseMat( &intrinsic );
  cvReleaseMat( &distortion );
}
