/*
*   viewsgi.c
*		---- Program for Viewing SGI image file
*
*       	January 24, 1998 	Programed by TANIKAWA Tomohiro
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "readjpeg.h"

#define SIZEX 500
#define SIZEY 500

int winX, winY;
int winWidth =SIZEX, winHeight =SIZEY;
int pointerX, pointerY;
GLubyte *image =NULL;
GLsizei imageWidth, imageHeight, imageDepth;
GLenum	imageFormat;

void
keyboard(unsigned char c, int x, int y)
{
    switch(c){
      case 32:
	glutPostRedisplay();
	break;
      case 27:
	exit(1);
	break;
    }
}

void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glRasterPos2i((winWidth-imageWidth)/2, (winHeight-imageHeight)/2);
    glDrawPixels(imageWidth, imageHeight, imageFormat, GL_UNSIGNED_BYTE, image);

    glutSwapBuffers();
}

void
reshape(int w, int h)
{
/*    winWidth = SIZEX; winHeight = SIZEY;*/
    glutReshapeWindow(winWidth, winHeight);
    glViewport(0, 0, winWidth, winHeight);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)winWidth, 0.0, (GLdouble)winHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void
init(char *filename)
{
    if((image = readJPEGimage(filename, &imageWidth, &imageHeight, &imageDepth))
       == NULL){
	fprintf(stderr,"JPEG Image file read Error!!\n");
	exit(0);
    }
    if(imageDepth >= 4) imageFormat = GL_RGBA;
    else if(imageDepth == 3) imageFormat = GL_RGB;
    else imageFormat = GL_LUMINANCE;
    printf("%s: %d, %d, %d\n",filename, imageWidth, imageHeight, imageDepth);
    
    winWidth = imageWidth;
    winHeight = imageHeight;
}

int
main(int argc, char **argv)
{
    char	title[128];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    init(argv[1]);

    glutInitWindowSize(winWidth, winHeight);
    sprintf(title, "%s (%dx%d) - JPEG image Viewer", argv[1], imageWidth, imageHeight);
    glutCreateWindow(title);
/*    glutFullScreen();*/

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
