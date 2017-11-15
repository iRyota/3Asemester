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
int angle = 0;

//-------------------------------------------

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  if (btn_flag==4){
    angle++;
    if (angle >= 360) {
      angle = 0;
    }
  }
  // draing wire cube
  glColor3d(0.0, 0.0, 0.0);
  glPushMatrix();
  glutWireCube(1.0);
  glPopMatrix();


  glPushMatrix();
  glRotatef(angle, 0.0,1.0,0.0);
  glTranslatef(pos_wii.x, pos_wii.y, pos_wii.z);
  //printf("x: %f, y: %f, z: %f\n", pos_x, pos_y, pos_z);
  
  //change color and start/stop rotate
  if(btn_flag == 0 && btn_flag == 4){
    glColor3d(1.0,0.0,0.0);
  }else if(btn_flag == 1){
    glColor3d(0.0,1.0,0.0);
  }else if(btn_flag == 2){
    glColor3d(0.0, 0.0, 1.0);
  }
  glutSolidSphere(0.2, 10, 10);
  glPopMatrix();

  glutSwapBuffers();
  //glFlush();
}

//-------------------------------------------

void resize(int w, int h)
{
  glViewport(0, 0, w, h);

  glLoadIdentity();
  gluPerspective(60.0, (double)w/(double)h, 1.0, 20.0);
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


//-------------------------------------------

void keyboard(unsigned char key, int x, int y){
  switch(key){
  case 27: // ESC key
    if(cwiid_close(wiimote)){
      fprintf(stderr,"Error on wiimote disconnect\n");
      exit(-1);
    }
    exit (0);
    break;
  case 'r':
    //vibration ON
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 1);
    break;
  case 'R':
    //vibration OFF
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 0);
    break;
  case '1':
    //LED 1 ON
    cwiid_command(wiimote, CWIID_CMD_LED, 1);
    break;
  case '!':
    //LED 1 OFF
    cwiid_command(wiimote, CWIID_CMD_LED, 0);
    break;
  case '2':
    //LED 2 ON
    cwiid_command(wiimote, CWIID_CMD_LED, 2);
    break;
  case '\"':
    //LED 2 OFF
    cwiid_command(wiimote, CWIID_CMD_LED, 0);
    break;
  default:
    break;
  }
}


//-------------------------------------------

void idle(void){
  glutPostRedisplay();
}

//-------------------------------------------

void init(void)
{
  glClearColor(1.0,1.0,1.0,1.0);
  pos_wii.x = 0.0;
  pos_wii.y = 0.0;
  pos_wii.z = 0.0;
  glEnable(GL_DEPTH_TEST);
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

  /* BDADDR Bluetooth address*/
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
  double roll, pitch;

  double px, py, pz;

  //printf("x: %d, y: %d, z: %d\t", (mesg->acc)[0], (mesg->acc)[1], (mesg->acc)[2]);
  //printf("x: %d, y: %d, z: %d\t", acc_one.x, acc_one.y, acc_one.z);
  //printf("x: %d, y: %d, z: %d\n", acc_zero.x, acc_zero.y, acc_zero.z);

  a_x = ((double)mesg->acc[CWIID_X] - acc_zero.x) /
    (acc_one.x - acc_zero.x);
  a_y = ((double)mesg->acc[CWIID_Y] - acc_zero.y) /
    (acc_one.y - acc_zero.y);
  a_z = ((double)mesg->acc[CWIID_Z] - acc_zero.z) /
    (acc_one.z - acc_zero.z);

  //printf("ax: %f, ay: %f, az: %f\t", a_x, a_y, a_z);
  roll = atan(a_x/a_z);
  if (a_z <= 0.0) {
    roll += PI * ((a_x > 0.0) ? 1 : -1);
  }
  roll *= -1;
  pitch = atan(a_y/a_z*cos(roll));

  px = -0.02;
  if(roll > 0.5){
    pos_wii.x +=px*(roll-0.5);
  }else if(roll < -0.5){
    pos_wii.x -= px*(-0.5-roll);
  }

  pz = -0.03;
  if(pitch > 0.5){
    pos_wii.y +=pz*(pitch-0.5);
  }else if(pitch < -0.5){
    pos_wii.y -= pz*(-0.5-pitch);
  }

  py = 0.02;
  if(a_x > 0.2){
    pos_wii.z +=py*(a_x-0.2);
  }else if(a_x < -0.2){
    pos_wii.z -= py*(-0.2-a_x);
  }


  //printf("roll: %f, pitch: %f\n", roll, pitch);


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
  }
  if(mesg->buttons & CWIID_BTN_HOME){
    btn_flag = 0;
  }
  if(mesg->buttons & CWIID_BTN_1){
    btn_flag = 4;
  }
  if(mesg->buttons & CWIID_BTN_2){
    btn_flag = 0;
  }

}

//-------------------------------------------

