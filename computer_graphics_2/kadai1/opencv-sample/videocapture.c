/*
*   videocapture.c
*		---- OpenGL program example for capturing by using OpenCV
*
*       	December 10, 2009 	programmed by TANIKAWA Tomohiro
*                                       changed by NISHIMURA Kunihiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* OpenGL */
#include <GL/glut.h>
/* OpenCV */
#include <cv.h>
#include <highgui.h>

#define SIZE_X 	640
#define SIZE_Y 	480

int 		win_width =SIZE_X, win_height =SIZE_Y;
int 		cap_width =SIZE_X, cap_height =SIZE_Y;
IplImage 	*frame;         /* OpenCV�摜�\���� */
CvCapture* 	capture = 0;	/* Capture�\���� */
int 		dev_index = 0;	/* �J�����̃C���f�b�N�X */

void my_init(int argc, char **argv);
int my_exit(int e);

/* �L�[���͂ւ̑Ή� */
void keyboard(unsigned char c, int x, int y)
{
    switch(c){
      case 32:
	  glutPostRedisplay();
	break;
      case 27:
	  my_exit(1);
	break;
    }
}

/* �A�C�h����Ԃł̏��� */
void idle(void)
{
    if (capture) {
	frame = cvQueryFrame(capture); /* �J�����摜�̎擾 */
	if (frame->origin==0) {
	    cvFlip(frame, frame, 0 ); /* OpenCV��OpenGL�ł͍��W���قȂ邽�ߕϊ� */
	}
	cvCvtColor(frame, frame, CV_BGR2RGB); /* �F�f�[�^�̓���ւ� */
    }
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glutPostRedisplay();
}

/* �摜�̏����o�� */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* �e�N�X�`���f�[�^�̊i�[���� */
/*    glPixelZoom(1.0, 1.0);
      glRasterPos2i(-cap_width/2, -cap_height/2); */
    glPixelZoom((GLdouble)(win_width-2)/(GLdouble)cap_width,
		(GLdouble)(win_height-2)/(GLdouble)cap_height);
    glRasterPos2i(-win_width/2+1, -win_height/2+1); /* ���_�w�� */
    glDrawPixels(cap_width, cap_height, GL_RGB, GL_UNSIGNED_BYTE,
		 frame->imageData);

    glFlush();
    glutSwapBuffers();
}

/* �E�B���h�E�T�C�Y�ύX���̏��� */
void reshape(int w, int h)
{
/*    win_width = SIZE_X; win_height = SIZE_Y;*/
    win_width =  w; win_height =  h;
    glViewport(0, 0, win_width, win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-(GLdouble)win_width/2.0, (GLdouble)win_width/2.0,
	       -(GLdouble)win_height/2.0, (GLdouble)win_height/2.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

/* OpenCV�֘A�̏����� */
void my_init(int argc, char **argv)
{
    /* �J��������̃L���v�`������ */
    capture = cvCreateCameraCapture(dev_index);
    if (capture) {
	/* �J�����摜�̎擾 */
	frame = cvQueryFrame(capture);
	/* �J������ʃT�C�Y�̎擾 */
	cap_width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	cap_height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    } else {
	fprintf(stderr, "Found No Camera\n");
	exit(-1);
    }
    /* �摜�f�[�^�̏㉺���] */
    if (frame->origin==0) {
	cvFlip(frame, frame, 0);
    }
    /* BGR�z�񂩂�RGB�z��ւ̕ϊ� */
    cvCvtColor(frame, frame, CV_BGR2RGB);

    win_width = cap_width;
    win_height = cap_height;
}

int my_exit(int e)
{
    /* �L���v�`���̏I������ */
    if (capture) cvReleaseCapture(&capture);

    exit(e);
}

/* ���C���֐� */
int main(int argc, char **argv)
{
    char	title[128];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    my_init(argc, argv);

    glutInitWindowSize(win_width, win_height);
    sprintf(title, "OpenCV Video Texture - (%dx%d)", cap_width, cap_height);
    glutCreateWindow(title);
/*    glutFullScreen();*/

    glClearColor(0.0, 0.0, 0.0, 0.0);

/* OpenGL�R�[���o�b�N�֐��̎w�� */
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
