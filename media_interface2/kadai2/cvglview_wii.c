/* 2008.12.16 知能機械情報学演習 Media Interface */
/* Updated 2010                               */
/* Masamichi Shimosaka  simosaka@ics.t.u-tokyo.ac.jp */
/* Kunihiro Nishimura   kuni@cyber.t.u-tokyo.ac.jp */

#include "cv.h"
#include "highgui.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <pthread.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include "cwiid.h"

#define KEY_ESC 27

#define CHESS_ROW_NUM 10
#define CHESS_COL_NUM 7
#define CHESS_ROW_DX 20
#define CHESS_COL_DY 20

typedef struct{
 double dx;
 double dy;
 CvSize patternSize;
} ChessBoard;

int graphicsWinWidth = 640;
int graphicsWinHeight = 480;

double distance = 3.0;
double theta = 0.0;
double twist = 0.0;
double elevation = 0.0;
double azimuth = 0.0;

double xAngle = 0;
double yAngle = 0;

int xStart = 0;
int yStart = 0;

int mouseFlag = GL_FALSE;

double transGL[3];

double rotGL[4];

int isCVinitialized = 0;

const char* captureWinName = "camera view";

void store2DCoordinates( CvMat* cornerPoints, CvPoint2D32f* points,
			 ChessBoard chess, int imageID );

void store3DCoordinates( CvMat* objectPoints, ChessBoard chess, int imageID );


void loadIntrinsicParams(const char* fileName, CvMat* intrinsic,
			 CvMat* distortion);

void getCameraPosition(CvMat* rotation, CvMat* translation, double* cameraPosition);

void getCameraOriVec(CvMat* rotation, double* cameraOriVec);

void convertCv2Gl(double* cvPosInMM, double* glPosInM);


void calibrate ( CvMat* cornerPoints, CvMat* objectPoints,
		 CvMat* intrinsic, CvMat* distortion,
		 CvMat* rotation, CvMat* translation );

void updateTexture(IplImage *image);

void initTexture();



struct acc {
	uint8_t x;
	uint8_t y;
	uint8_t z;
};
struct acc acc_zero, acc_one;

struct pos{
  double x;
  double y;
  double z;
};

struct pos pos_global,pos_wii;
double pos_x, pos_y, pos_z;

cwiid_wiimote_t *wiimote = NULL;
bdaddr_t bdaddr;
cwiid_mesg_callback_t wiimote_callback;

void wiimote_acc(struct cwiid_acc_mesg *);
void wiimote_btn(struct cwiid_btn_mesg *mesg);
void set_report_mode(void);

/* GetOpt */
#define OPTSTRING	"h"
extern char *optarg;
extern int optind, opterr, optopt;
#define USAGE "usage:%s [-h] [BDADDR]\n"

int btn_flag = 0;
static double wroll = 0.0, wpitch = 0.0;

int spin_flag = -1;
int angle = 0;

int grab_flag = -1;


/* 3軸の描画 */
void drawXYZAxes()
{
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3d(0, 1, 0);//x軸 緑
    glVertex2d(0, 0);
    glVertex2d(0.5, 0);

    glColor3d(1, 0, 0);//y軸 赤
    glVertex2d(0, 0);
    glVertex2d(0, 0.5);

    glColor3d(0, 0, 1);//z軸 青
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 0.5);
    glEnd();
}


/* チェッカーボードの描画 */
void drawBoard(double unit, int xsize, int ysize)
{
    int i,j;
    xsize++;
    ysize++;

    glColor3d(1, 1, 1);
    glLineWidth(0.5);
    glBegin(GL_POLYGON);
    glVertex3d(-unit, 0, -unit);
    glVertex3d(xsize * unit + unit, 0, -unit);
    glVertex3d(xsize * unit + unit, 0, ysize * unit + unit);
    glVertex3d(-unit, 0, ysize * unit + unit);
    glEnd();

    glColor3d(0, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3d(-unit, 0, -unit);
    glVertex3d(xsize * unit + unit, 0, -unit);
    glVertex3d(xsize * unit + unit, 0, ysize * unit + unit);
    glVertex3d(-unit, 0, ysize * unit + unit);
    glEnd();

    glLineWidth(0.5);
    for( i = 0; i < xsize; i++){
	for( j = 0; j < ysize; j++){
	    double offsetx = unit * i;
	    double offsety = unit * j;
	    double xend = unit * (i + 1);
	    double yend = unit * (j + 1);

	    if((i + j) % 2){
		glColor3d(1, 1, 1);
	    } else {
		glColor3d(0, 0, 0);
	    }
	    glBegin(GL_POLYGON);
	    glVertex3d(offsetx, 0, offsety);
	    glVertex3d(xend, 0, offsety);
	    glVertex3d(xend, 0, yend);
	    glVertex3d(offsetx, 0, yend);
	    glEnd();
	}
    }
}

/* Teapotの描画 */
void drawTeapot(){
    if (spin_flag == 1) {
      if(++angle > 360){
        angle = 0;
      }
    }
    glPushMatrix();
    /* 課題 */
    /* Wiiリモコンの傾きを色にマッピングするように変更する */
    glColor3d(pos_wii.x, pos_wii.y, pos_wii.z);
    /* 課題 */
    /* Wiiリモコンにより、さまざまな操作を可能にすること */
    glRotatef(angle,0.0,1.0,0.0);
    glTranslated(pos_wii.x, pos_wii.y, pos_wii.z);
    glutWireTeapot(0.1);
    glPopMatrix();
}

/* OpenGLの描画部分 */
void display(void)
{
  float aspect = 1.0f * (float)graphicsWinWidth / (float)graphicsWinHeight;
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.01f, 100.f);
    //
    /* 課題 */
    /* ヒント transGLを用いて、視点位置を変更するようにする */
    gluLookAt(transGL[0], transGL[1], transGL[2],  0, 0, 0,  0, 1, 0);


    glRotated(xAngle,1,0,0);
    glRotated(yAngle,0,1,0);

    /* 3軸の描画 */
    drawXYZAxes();
    /* チェッカーボードの描画 */
    drawBoard(CHESS_ROW_DX/1000.0f, CHESS_ROW_NUM, CHESS_COL_NUM);
    /* Teapotの描画 */
    drawTeapot();
    glutSwapBuffers();
}

void idle(void)
{
    glutPostRedisplay();
}

void keyboardFunction( unsigned char key, int x, int y )
{
    if(key == KEY_ESC){
      if(cwiid_close(wiimote)){
	fprintf(stderr,"Error on wiimote disconnect\n");
	exit(-1);
      }
      exit(0);
    }
    glutPostRedisplay();
}

void mouseFunction( int button, int state, int x, int y )
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	xStart = x;
	yStart = y;
	mouseFlag = GL_TRUE;
    } else{
	mouseFlag = GL_FALSE;
    }
}

void motionFunction( int x, int y )
{
    int xdis;
    int ydis;
    double a = 0.25;

    if(mouseFlag ==GL_FALSE){
	return;
    }

    xdis = x - xStart;
    ydis = y - yStart;
    xAngle += a * ydis;
    yAngle += a * xdis;
    xStart = x;
    yStart = y;

    glutPostRedisplay();
}

void initialize()
{

  float aspect = 1.0f * (float)graphicsWinWidth / (float)graphicsWinHeight;

    glutInitWindowPosition(0, 0);
    glutInitWindowSize( graphicsWinWidth, graphicsWinHeight);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("graphics window");

    glClearColor (0.0, 0.0, 0.0, 1.0);

    glutKeyboardFunc(keyboardFunction);
    glutMouseFunc(mouseFunction);
    glutMotionFunc(motionFunction);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display);
    glutIdleFunc(idle);

}

void* captureThread(void* args)
{
  /*IpBasedCapture* capture = NULL;*/
  CvCapture* capture = NULL;
    IplImage* frame;
    IplImage* frame_copy;

    IplImage* gray;
    ChessBoard chess;
    int pointsPerScene;
    CvPoint2D32f* points;
    int pointsNum[1];
    CvMat* cornerPoints;
    CvMat* objectPoints;
    CvMat pointsNumMat;

    CvMat* intrinsic = cvCreateMat( 3, 3, CV_64F );
    CvMat* distortion = cvCreateMat( 4, 1, CV_64F );
    CvMat* rotation = cvCreateMat( 3, 3, CV_64F );
    CvMat* translation = cvCreateMat( 3, 1, CV_64F );

    loadIntrinsicParams("intrinsic_param.txt", intrinsic, distortion );

    /*課題*/
    /* カメラをキャプチャする部分。以前のプログラムを参照すれば分かるはず */
    capture = cvCreateCameraCapture( 0 );

    if(capture == NULL){
	fprintf(stderr, "ERROR: Could not open Camera Device\n");
	exit(1);
    }

    /*frame = queryFrameOverIP(capture);*/
    frame = cvQueryFrame(capture);

    if(frame == NULL){
	fprintf(stderr, "ERROR: Could not query frame\n");
	exit(1);
    }

    frame_copy = cvCreateImage(cvGetSize(frame),
			       frame->depth, 3);

    gray = cvCreateImage(cvGetSize(frame_copy), frame_copy->depth, 1);

//    cvNamedWindow(captureWinName, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(captureWinName, graphicsWinWidth + 10, 0);

    chess.dx = CHESS_ROW_DX;
    chess.dy = CHESS_COL_DY;
    chess.patternSize.width = CHESS_ROW_NUM;
    chess.patternSize.height = CHESS_COL_NUM;

    pointsPerScene = chess.patternSize.width * chess.patternSize.height;
    cornerPoints = cvCreateMat(pointsPerScene, 2, CV_32F);
    objectPoints = cvCreateMat(pointsPerScene, 3, CV_32F);

    pointsNum[0] = pointsPerScene;
    pointsNumMat = cvMat(1, 1, CV_32S, pointsNum);

    points = (CvPoint2D32f*)malloc( sizeof(CvPoint2D32f) * pointsPerScene );

    //flag of initialization
    isCVinitialized = 1;

    while(1){
	int allPointsFound = 0;
	int detectedPointsNum;
        /* 課題 */
        /* カメラでキャプチャしたフレームをframeに格納する部分 */
        /* 以前のプログラムを参照すればわかる */
	frame = cvQueryFrame(capture); 

	if( !frame ) {
	    fprintf(stderr, "could not query frame\n");
	    exit(1);
	}

	cvResize(frame, frame_copy, CV_INTER_NN);
	cvCvtColor(frame_copy, gray, CV_BGR2GRAY);
	if( cvFindChessboardCorners( gray, chess.patternSize, points,
				     &detectedPointsNum,
				     CV_CALIB_CB_ADAPTIVE_THRESH ) ){
	    cvFindCornerSubPix(gray, points, detectedPointsNum,
			       cvSize(5, 5), cvSize(-1, -1),
			       cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.1));
	    allPointsFound = 1;
	} else {
	    allPointsFound = 0;
	}

	cvDrawChessboardCorners( frame_copy, chess.patternSize, points,
				 detectedPointsNum, allPointsFound );

	if( allPointsFound ){
	    double cameraPosition[3];
	    double cameraOriVec[4];
	    store2DCoordinates( cornerPoints, points, chess, 0 );
	    store3DCoordinates( objectPoints, chess, 0 );
	    calibrate( cornerPoints, objectPoints,
		       intrinsic, distortion, rotation, translation );

	    getCameraPosition(rotation, translation, cameraPosition);
	    printf("cam pos relative to chess board: %.1f, %.1f, %.1f\n",
		   cameraPosition[0],
		   cameraPosition[1],
		   cameraPosition[2]);
	    convertCv2Gl(cameraPosition, transGL);
	    getCameraOriVec(rotation, rotGL);
	}

	cvShowImage( captureWinName, frame_copy);
	if(cvWaitKey(10) == KEY_ESC){
	    exit(1);
	}
    }

    free(points);
    cvReleaseMat(&intrinsic);
    cvReleaseMat(&distortion);
    cvReleaseMat(&rotation);
    cvReleaseMat(&translation);

    cvReleaseMat(&cornerPoints);
    cvReleaseMat(&objectPoints);

    cvDestroyWindow(captureWinName);
    cvReleaseImage(&frame_copy);
    cvReleaseImage(&gray);
    cvReleaseCapture(&capture);
}

int main(int argc, char** argv)
{
  int cnt = 0;
    pthread_t thread;

  int c;
  char *str_addr;

  /* Parse Options */
  while ((c = getopt(argc, argv, OPTSTRING)) != -1) {
    switch (c) {
    case 'h':
      printf(USAGE, argv[0]);
      return 0;
      break;
    case '?':
      return -1;
      break;
    default:
      printf("unknown command-line option: -%c\n", c);
      break;
    }
  }

  /* BDADDR Bluetoothアドレスの読み込み */
  bdaddr_t bdaddr_any = {{0, 0, 0, 0, 0, 0}};

  if (optind < argc) {
    if (str2ba(argv[optind], &bdaddr)) {
      printf("invalid bdaddr\n");
      bacpy(&bdaddr, &bdaddr_any);
    }
    optind++;
    if (optind < argc) {
      printf("invalid command-line\n");
      printf(USAGE, argv[0]);
      return -1;
    }
  }
  else if ((str_addr = getenv(WIIMOTE_BDADDR)) != NULL) {
    if (str2ba(str_addr, &bdaddr)) {
      printf("invalid address in %s\n", WIIMOTE_BDADDR);
      bacpy(&bdaddr, &bdaddr_any);
    }
  }
  else {
    bacpy(&bdaddr, &bdaddr_any);
  }

  char reset_bdaddr = 0;
  unsigned char buf[7];
  int paired;

  if (bacmp(&bdaddr, &bdaddr_any) == 0) {
    reset_bdaddr = 1;
  }

  printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
  paired=0;
  do {
    if (!(wiimote = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC))) {
      fprintf(stderr, "Unable to connect to wiimote - retrying\n");
    }else{
      paired=1;
    }
  }while(!paired);
  printf("Success!\n");

  if (cwiid_set_mesg_callback(wiimote, &wiimote_callback)) {
    printf("Unable to set message callback\n");
  }
  else {
    if (cwiid_read(wiimote, CWIID_RW_EEPROM, 0x16, 7, buf)) {
      printf("Unable to retrieve accelerometer\n");
    }
    else {
      acc_zero.x = buf[0];//zero point for X axis
      acc_zero.y = buf[1];
      acc_zero.z = buf[2];
      acc_one.x  = buf[4];//+1G point for X axis
      acc_one.y  = buf[5];
      acc_one.z  = buf[6];
    }
    set_report_mode();
    cwiid_command(wiimote, CWIID_CMD_STATUS, 0);
  }

  if (reset_bdaddr) {
    bacpy(&bdaddr, &bdaddr_any);
  }

    glutInit(&argc, argv);
    initialize();

    /* カメラキャプチャのスレッドを起動する */
    pthread_create(&thread, NULL, captureThread, (void*)NULL);

    //flag of initalization
    while(!isCVinitialized){
      cvWaitKey(10);
      cnt++;
      if(cnt>500){
        printf("Initialization Error\n");
        exit(-1);
      }
    }

    glutMainLoop();
    pthread_join(thread, NULL);

    return 0;
}



void store2DCoordinates( CvMat* cornerPoints, CvPoint2D32f* points,
                       ChessBoard chess, int imageID )
{
    int i = 0;
    int pointsPerScene = chess.patternSize.width * chess.patternSize.height;
    int offset = imageID * pointsPerScene;

    for ( i = 0; i < pointsPerScene; i++ ) {
	cvmSet(cornerPoints, offset + i, 0, points[i].x);
	cvmSet(cornerPoints, offset + i, 1, points[i].y);
    }
}

void store3DCoordinates( CvMat* objectPoints,ChessBoard chess, int imageID ) 
{
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


void loadIntrinsicParams(const char* fileName, CvMat* intrinsic,
			 CvMat* distortion)
{
    FILE* file;
    int i, j;
    int ret;

    if((file = fopen(fileName, "r")) == NULL){
	fprintf(stderr, "file open error\n");
	return;
    }

    for( i = 0; i < 3; i++){
	for( j = 0; j < 3; j++){
	    double value = 0;
	    ret = fscanf( file, "%lf", &value );
	    cvmSet( intrinsic, i, j, value );
	}
    }

    for( i = 0; i < 4; i++){
	cvmSet( distortion, i, 0,  0.0 );
    }

    fclose(file);
}

void getCameraPosition(CvMat* rotation, CvMat* translation,
		       double* cameraPosition)
{
    int rr;
    int cc;

    for(rr = 0; rr < 3; rr++){
	cameraPosition[rr] = 0.0;
	for(cc = 0; cc < 3; cc++){
	    cameraPosition[rr] -=
		cvmGet(rotation, cc, rr) * cvmGet(translation, cc, 0);
	}
    }

    //inverting z axis value
    cameraPosition[2] *= -1;
}

void getCameraOriVec(CvMat* rotation, double* cameraOriVec)
{
  int rr;
  for(rr=0; rr<3; rr++){
    cameraOriVec[rr] = cvmGet(rotation, 0, rr);
  }
}

void convertCv2Gl(double* cvPosInMM, double* glPosInM)
{
    glPosInM[0] = cvPosInMM[0] / 1000.;
    glPosInM[1] = cvPosInMM[2] / 1000.;
    glPosInM[2] = cvPosInMM[1] / 1000.;
}

void calibrate ( CvMat* cornerPoints, CvMat* objectPoints,
		 CvMat* intrinsic, CvMat* distortion,
		 CvMat* rotation, CvMat* translation )
{

    CvMat* rotVector = cvCreateMat( 3, 1, CV_64F );
    cvFindExtrinsicCameraParams2( objectPoints, cornerPoints,
				  intrinsic, distortion,
				  rotVector, translation, 0 );

    cvRodrigues2( rotVector, rotation, 0 );

    cvReleaseMat( &rotVector );
}


//-------------------------------------------

void set_report_mode(void)
{
  uint8_t rpt_mode;

  rpt_mode = CWIID_RPT_STATUS | CWIID_RPT_BTN;

  rpt_mode |= CWIID_RPT_ACC;
  cwiid_command(wiimote, CWIID_CMD_RPT_MODE, rpt_mode);
}

//-------------------------------------------

void wiimote_callback(cwiid_wiimote_t *wiimote, int mesg_count,
		      union cwiid_mesg mesg_array[], struct timespec *timestamp)
{
  int i;

  /* 課題 */
  /* Wiiリモコンのイベントのコールバックを実装せよ。*/
  for (i=0; i < mesg_count; i++) {
    switch (mesg_array[i].type) {
    case CWIID_MESG_ACC:
      wiimote_acc(&mesg_array[i].acc_mesg);
      break;
    case CWIID_MESG_BTN:
      wiimote_btn(&mesg_array[i].btn_mesg);
      break;
    default:
      break;
    }
  }

}

//-------------------------------------------

void wiimote_acc(struct cwiid_acc_mesg *mesg)
{
  //static gchar str[LBLVAL_LEN];
  double a_x, a_y, a_z;

  a_x = ((double)mesg->acc[CWIID_X] - acc_zero.x) /
    (acc_one.x - acc_zero.x);
  a_y = ((double)mesg->acc[CWIID_Y] - acc_zero.y) /
    (acc_one.y - acc_zero.y);
  a_z = ((double)mesg->acc[CWIID_Z] - acc_zero.z) /
    (acc_one.z - acc_zero.z);

  //printf("ax: %f, ay: %f, az: %f\t", a_x, a_y, a_z);
  wroll = atan(a_x/a_z);
  if (a_z <= 0.0) {
    wroll += M_PI * ((a_x > 0.0) ? 1 : -1);
  }
  //wroll *= -1;
  wpitch = atan(a_y/a_z*cos(wroll));

  if (grab_flag == 1) {
    pos_wii.x -= a_x * 0.001;
    pos_wii.y += a_z * 0.001;
    pos_wii.z += a_y * 0.001;
  }

  //printf("roll: %f, pitch: %f\n", wroll, wpitch);


}

//-------------------------------------------

void wiimote_btn(struct cwiid_btn_mesg *mesg){

  //printf("%d\n", mesg->buttons & CWIID_BTN_UP);
  if(mesg->buttons & CWIID_BTN_UP){
    pos_wii.y += 0.2;
  }
  if(mesg->buttons & CWIID_BTN_DOWN){
    pos_wii.y -= 0.2;
  }
  if(mesg->buttons & CWIID_BTN_RIGHT){
    pos_wii.x += 0.2;
  }
  if(mesg->buttons & CWIID_BTN_LEFT){
    pos_wii.x -= 0.2;
  }
  if(mesg->buttons & CWIID_BTN_PLUS){
    pos_wii.z += 0.2;
  }
  if(mesg->buttons & CWIID_BTN_MINUS){
    pos_wii.z -= 0.2;
  }
  if(mesg->buttons & CWIID_BTN_A){
    btn_flag = 1;
    spin_flag *= -1;
  }
  if(mesg->buttons & CWIID_BTN_B){
    btn_flag = 2;
    grab_flag *= -1;
  }
  if(mesg->buttons & CWIID_BTN_HOME){
    btn_flag = 0;
  }
  if(mesg->buttons & CWIID_BTN_1){
    btn_flag = 3;
  }
  if(mesg->buttons & CWIID_BTN_2){
    btn_flag = 4;
  }


}

//-------------------------------------------

