/*
*   videotexture.c
*		---- OpenGL program example for capturing by using OpenCV
*
*       	December 11, 2009 	programmed by TANIKAWA Tomohiro
*                                       changed by NISHIMURA Kunihiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* OpenGL */
#include <GL/glut.h>
/* OpenCV */
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define SIZE_X 		640
#define SIZE_Y 		480
#define PI 		3.1415926535897932
#define DEG 		(2*PI/360.)
#define FOV_Y 		45 /* �J�����̉�p */
#define NEAR_CLIP	10.0
#define FAR_CLIP 	10000.0
#define DISTANCE_0	579.41
#define DIV_X 	64 /* x�����̃e�N�X�`�������� */
#define DIV_Y 	48 /* y�����̃e�N�X�`�������� */

int 		win_width =SIZE_X, win_height =SIZE_Y;
GLfloat 	distance = DISTANCE_0, pitch = 0.0, yaw = 0.0;
GLint		mouse_button = -1;
GLint		mouse_x = 0, mouse_y =0;

GLuint		tex_index;
GLsizei 	tex_width, tex_height;
GLfloat        	tex_coord_v[DIV_Y+1][DIV_X+1][2]; /* �e�N�X�`�����W�s�� */
GLfloat        	vertex_v[DIV_Y+1][DIV_X+1][3]; /* �|���S�����_�s�� */

IplImage 	*frame;			/* OpenCV�摜�\���� */
CvCapture* 	capture = 0;		/* Capture�\���� */
int 		dev_index = 0;		/* �J�����̃C���f�b�N�X */
int 		cap_width =SIZE_X, cap_height =SIZE_Y;

int time = 0;

/* OpenGL�R�[���o�b�N�֐� */
void keyboard(unsigned char c, int x, int y);
void display(void);
void reshape(int w, int h);
void mouse(int button, int status, int x, int y);
void motion(int x, int y);
void idle(void);
void my_init(int argc, char **argv);
void my_initGL(void);
void my_exit(int e);

float surface(float x, float y, int t);

/* �e�N�X�`���̃T�C�Y��2��n�� */
void define_texturesize(GLsizei *w0, GLsizei *h0, GLsizei *w1, GLsizei *h1)
{
    int i = 0;
    GLsizei prev;

    while(*w0 > (prev = (int)pow(2, i))) i++; *w1 = prev;
    i = 0;
    while(*h0 > (prev = (int)pow(2, i))) i++; *h1 = prev;
}

float surface(float x, float y, int t) {
    float r;
    r = sqrt(x*x+y*y);
    return 10*sin(r/time);
}

/* �|���S���̒��_�ƃe�N�X�`�����W�̎w�� */
void generate_surface(void)
{
    int         i, j;
/* �ۑ�F�����̃e�N�X�`�����W�̎w����C������ */
/* TODO_s �� TODO_e �̊Ԃ�ύX���� */
    for( j=0; j<=DIV_Y; j++ ){
        for( i=0; i<=DIV_X; i++ ){
            vertex_v[j][i][0] = ((float)i/(float)DIV_X -0.5) * (float)win_width;
            vertex_v[j][i][1] = ((float)j/(float)DIV_Y -0.5) * (float)win_height;
            vertex_v[j][i][2] = surface(vertex_v[j][i][0], vertex_v[j][i][1], time);
            tex_coord_v[j][i][0] = (float)/*TODO_s*/i/*TODO_e*//(float)DIV_X * (float)cap_width/(float)tex_width;
            tex_coord_v[j][i][1] = (float)/*TODO_s*/j/*TODO_e*//(float)DIV_Y * (float)cap_height/(float)tex_height;
        }
    }
}

/* �L�[���͂ւ̑Ή� */
void keyboard(unsigned char c, int x, int y)
{
    switch(c){
      case 32: /* Space */
	  glutPostRedisplay();
	break;
      case 27: /* Escape */
	  my_exit(1);
	break;
    }
}

/* �}�E�X�̃{�^�����͂ւ̑Ή� */
void mouse(int button, int state, int x, int y)
{
    mouse_button = button;
    mouse_x = x; mouse_y = y;
    if(state == GLUT_UP) mouse_button = -1;

    glutPostRedisplay();
}

/* �}�E�X�̈ړ��ւ̔��� */
void motion(int x, int y)
{
    switch(mouse_button){
      case GLUT_LEFT_BUTTON:
	if( x==mouse_x && y==mouse_y ) return;
	yaw -= (GLfloat)( x - mouse_x ) /10.0;
	pitch -= (GLfloat)( y - mouse_y ) /10.0;
	break;
      case GLUT_RIGHT_BUTTON:
	if( y==mouse_y ) return;
	if( y < mouse_y ) distance += (GLfloat)(mouse_y - y);
	else distance -= (GLfloat)(y - mouse_y);
	if( distance < NEAR_CLIP ) distance = NEAR_CLIP;
	if( distance > FAR_CLIP ) distance = FAR_CLIP;
	break;
    }
    mouse_x = x;
    mouse_y = y;

    glutPostRedisplay();
}

/* �A�C�h����Ԃł̏��� */
void idle(void)
{
    if (capture) {
	frame = cvQueryFrame(capture); /* �J�����摜�̎擾 */
	if (frame->origin==0) {
	    cvFlip(frame, frame, 0 );
	}
	cvCvtColor(frame, frame, CV_BGR2RGB);
    }

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glBindTexture( GL_TEXTURE_2D, tex_index );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, frame->width, frame->height,
                     GL_RGB, GL_UNSIGNED_BYTE, frame->imageData ); /* �摜�̍X�V */
    glutPostRedisplay();
}

/* �摜�̏����o�� */
void display(void)
{
    int 	i, j;

    if((++time) == 90) {
        time = 0;
    }
    generate_surface();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef( -pitch, 1.0, 0.0, 0.0);
    glRotatef( -yaw, 0.0, 1.0, 0.0);

/* �e�N�X�`���̊��ݒ� */
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, tex_index );

/* �|���S���ւ̃e�N�X�`���\��t�� */
    glPushMatrix();
    for( j=0; j<DIV_Y; j++ ){
        glBegin( GL_TRIANGLE_STRIP );
        for( i=0; i<DIV_X; i++ ){
            glTexCoord2fv( tex_coord_v[j+1][i] );
            glVertex3fv( vertex_v[j+1][i] );
            glTexCoord2fv( tex_coord_v[j][i] );
            glVertex3fv( vertex_v[j][i] );
            glTexCoord2fv( tex_coord_v[j+1][i+1] );
            glVertex3fv( vertex_v[j+1][i+1] );
            glTexCoord2fv( tex_coord_v[j][i+1] );
            glVertex3fv( vertex_v[j][i+1] );
        }
        glEnd();
    }
    glPopMatrix();

    glutSwapBuffers();
}

/* �E�B���h�E�T�C�Y�ύX���̏��� */
void reshape(int w, int h)
{
    win_width =  w; win_height =  h;
    glViewport(0, 0, win_width, win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV_Y, (GLfloat)w/(GLfloat)h, NEAR_CLIP, FAR_CLIP);
    
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

/* �E�B���h�E�̃T�C�Y�w��ƍœK�ȃJ���������̎Z�o */
    win_width = cap_width;
    win_height = cap_height;
    distance = (cap_height/2.0) / tan(FOV_Y/2.0 * DEG);
}

/* OpenGL�̃e�N�X�`���֘A�̏����� */
void my_initGL(void )
{
    define_texturesize( &cap_width, &cap_height, &tex_width, &tex_height );
    generate_surface();

    glGenTextures( 1, &tex_index );
    glBindTexture( GL_TEXTURE_2D, tex_index );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, 0 );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, cap_width, cap_height,
                     GL_RGB, GL_UNSIGNED_BYTE, frame->imageData );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );
}

void my_exit(int e)
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    my_init(argc, argv);

    glutInitWindowSize(win_width, win_height);
    sprintf(title, "OpenCV Video Texture - (%dx%d)", cap_width, cap_height);
    glutCreateWindow(title);
/*    glutFullScreen();*/
    my_initGL();

    glClearColor(0.0, 0.0, 0.0, 0.0);

/* OpenGL�R�[���o�b�N�֐��̎w�� */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
