#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <GL/glut.h>
#include "cwiid.h"

#define PI 3.14159265358979323
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

// 視点情報
static double distance = 5.0;//pitch = 0.0, yaw = 0.0; 

// マウス入力情報
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

static const GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
static const GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

// インタラクション
static int disp_flag = 0;
static double disp_p = 0.03;

GLfloat mat_color_default[] = {1.0,1.0,1.0,1.0};

int time_count = 0;
int start_vib = 0;
float col = 1.0;

#define win_x 500
#define win_y 500

//-------------------------------------------

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);

  // 視点の設定
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


  //glRotatef( -wpitch*180.0/PI, 1.0, 0.0, 0.0 );
  //glRotatef( -wroll*180.0/PI, 0.0, 0.0, 1.0 );

  glPushMatrix();
  glTranslatef(pos_wii.x, pos_wii.y, pos_wii.z);
  //printf("x: %f, y: %f, z: %f\n", pos_x, pos_y, pos_z);
  if(btn_flag == 0){

  }else if(btn_flag == 1){

  }else if(btn_flag == 2){

  }
  glutSolidSphere(0.1, 20, 20);
  glPopMatrix();

  glPushMatrix();
  {
    glRotatef(75.0,0.0,1.0,0.0);
    glTranslatef(0.0,0.0,2.0);
    glRectf(-1.0,1.0,1.0,-1.0);
  }
  glPopMatrix();
  GLfloat mat_color[] = {1.0,col,col,1};
  glPushMatrix();
  {
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color);
    glRotatef(-75.0,0.0,1.0,0.0);
    glTranslatef(0.0,0.0,2.0);
    glRectf(-1.0,1.0,1.0,-1.0);
  }
  glPopMatrix();

  glutSwapBuffers();
  //glFlush();
}

//-------------------------------------------

void resize(int w, int h)
{
  glViewport(0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)w/(double)h, 1.0, 20.0);

}


//-------------------------------------------

void keyboard(unsigned char key, int x, int y){
  switch(key){
  case 27: // ESC
    if(cwiid_close(wiimote)){
      fprintf(stderr,"Error on wiimote disconnect\n");
      exit (-1);
    }
    exit (0);
    break;
  case 32: // SPACE 
    pos_wii.x = 0.0;
    pos_wii.y = 0.0;
    pos_wii.z = 0.0;
    glutPostRedisplay();
    break;
  case 'r':
    //振動ON
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 1);
    break;
  case 'R':
    //振動OFF
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 0);
    break;
  case '1':
    cwiid_command(wiimote, CWIID_CMD_LED, 1);
    break;
  case '!':
    cwiid_command(wiimote, CWIID_CMD_LED, 0);
    break;
  case '2':
    cwiid_command(wiimote, CWIID_CMD_LED, 2);
    break;
  case '\"':
    cwiid_command(wiimote, CWIID_CMD_LED, 0);
    break;
  default:
    break;
  }
}


//-------------------------------------------

void idle(void){
  if(pos_wii.x >= 2.0){
    disp_flag = 1;
    start_vib = 1;
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 1);
  } else if (pos_wii.x <= -2.0) {
    col -= 0.05;
    if (col <= 0) {
      col = 0;
    }
    disp_flag = -1;
    start_vib = 1;
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 1);
  }
  time_count += start_vib;
  if (time_count >= 10) {
    time_count = 0;
    start_vib = 0;
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 0);
  }
  if(disp_flag == -1){
    pos_wii.x += disp_p;
  }else if(disp_flag == 1){
    pos_wii.x -= disp_p;
  }
  glutPostRedisplay();
}

//-------------------------------------------

void init(void)
{
  glClearColor(0.0,0.0,0.0,0.0);
  pos_wii.x = 0.0;
  pos_wii.y = 0.0;
  pos_wii.z = 0.0;

  glClearDepth( 1.0 );
  
  // デプステストを行う
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  
  glShadeModel (GL_SMOOTH);
  
  // デフォルトライト
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_ambient);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  // デフォルトマテリアル
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_default_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_default_shininess);

}

//-------------------------------------------

int main(int argc, char *argv[])
{
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
  glutInitDisplayMode(GLUT_RGBA| GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(win_x, win_y);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  
  
  
  return 0;
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

  printf("ax: %f, ay: %f, az: %f\t", a_x, a_y, a_z);
  wroll = atan(a_x/a_z);
  if (a_z <= 0.0) {
    wroll += PI * ((a_x > 0.0) ? 1 : -1);
  }
  //wroll *= -1;
  wpitch = atan(a_y/a_z*cos(wroll));
  
  if(fabs(a_x) > 3 || fabs(a_y) > 3 ){
    if(disp_flag == 0){
      disp_flag = -1;
      disp_p += 0.001;
    }else if(disp_flag == -1){
      disp_flag = 1;
      disp_p += 0.001;
    }
  }
  printf("disp_flag: %d\n", disp_flag);
  // printf("roll: %f, pitch: %f\n", wroll, wpitch);


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
  }
  if(mesg->buttons & CWIID_BTN_B){
    btn_flag = 2;
    disp_flag = 0;
    disp_p = 0.03;
  }
  if(mesg->buttons & CWIID_BTN_HOME){
    pos_wii.x = 0.0;
    pos_wii.y = 0.0;
    pos_wii.z = 0.0;
  }
  if(mesg->buttons & CWIID_BTN_1){
    btn_flag = 0;
  }
  if(mesg->buttons & CWIID_BTN_2){
    btn_flag = 0;
  }


}

//-------------------------------------------

