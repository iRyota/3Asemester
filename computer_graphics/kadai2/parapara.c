/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*  texbind.c
 *  This program demonstrates using glBindTexture() by
 *  creating and managing two textures.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "readjpeg.h"

#ifdef GL_VERSION_1_1


static double distance = 5.0, pitch = 0.0, yaw = 0.0;

GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

GLubyte **image;
GLsizei *imageWidth, *imageHeight, *imageDepth;
GLenum  *imageFormat;
static GLuint *texName;

int status = 0;
int count = 0;
int frame = 0;

void init(int argc,char **filename)
{
  count = argc-1;
  if(count == 0){
    exit(0);
  }
  if((imageWidth = (GLsizei*)malloc(count * sizeof(GLsizei)))==NULL) {
    printf("error: could not allocate memory");
    exit(0);
  }
  if((imageHeight = (GLsizei*)malloc(count * sizeof(GLsizei)))==NULL) {
    printf("error: could not allocate memory");
    exit(0);
  }
  if((imageDepth = (GLsizei*)malloc(count * sizeof(GLsizei)))==NULL) {
    printf("error: could not allocate memory");
    exit(0);
  }
  if((imageFormat = (GLenum*)malloc(count * sizeof(GLenum)))==NULL) {
    printf("error: could not allocate memory");
    exit(0);
  }
  if((texName = (GLuint*)malloc(count * sizeof(GLuint)))==NULL) {
    printf("error: could not allocate memory");
    exit(0);
  }
  if((image = (GLubyte**)malloc(count * sizeof(GLubyte*)))==NULL) {
    printf("error: could not allocate memory");
    exit(0);
  }
  for (int i; i<count; i++){
    image[i] = NULL;
  }
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  int i;
  for (i=0;i<count;i++){
    if((image[i] = readJPEGimage(filename[i+1], &imageWidth[i], &imageHeight[i], &imageDepth[i]))== NULL){
      fprintf(stderr,"JPEG Image file read Error!!\n");
      exit(0);
    }
    if(imageDepth[i] >= 4) imageFormat[i] = GL_RGBA;
    else if(imageDepth[i] == 3) imageFormat[i] = GL_RGB;
    else imageFormat[i] = GL_LUMINANCE;
    printf("%s: %d, %d, %d\n",filename[i+1], imageWidth[i], imageHeight[i], imageDepth[i]);
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(count, texName);
  for(i=0;i<count;i++){
    glBindTexture(GL_TEXTURE_2D, texName[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                     GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat[i], imageWidth[i],
                  imageHeight[i], 0, imageFormat[i], GL_UNSIGNED_BYTE,
                  image[i]);
  }
  glEnable(GL_TEXTURE_2D);

  free(imageHeight);free(imageWidth);free(imageDepth);free(imageFormat);free(image);
}

void display(void)
{
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glRotatef( -pitch, 1.0, 0.0, 0.0 );
  glRotatef( -yaw, 0.0, 1.0, 0.0 );

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, texName[status]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, -2.0, 0.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 2.0, 0.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(3.0, 2.0, 0.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(3.0, -2.0, 0.0);
  glEnd();
  glFlush();
  if(++frame == 10){
    if (++status == count) {
      status = 0;
    }
    frame = 0;
  }
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -3.6);
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(0);
      break;
  }
}

void idle()
{
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  mouse_button = button;
  mouse_x = x;  mouse_y = y;

  if(state == GLUT_UP){
    mouse_button = -1;
  }

  glutPostRedisplay();
}

void motion(int x, int y)
{
  switch(mouse_button){
  case GLUT_LEFT_BUTTON:

    if( x == mouse_x && y == mouse_y )
      return;

    yaw -= (GLfloat) (x - mouse_x) / 10.0;
    pitch -= (GLfloat) (y - mouse_y) / 10.0;

    break;

  case GLUT_RIGHT_BUTTON:

    if( y == mouse_y )
      return;

    if( y < mouse_y )
      distance += (GLfloat) (mouse_y - y)/50.0;
    else
      distance -= (GLfloat) (y-mouse_y)/50.0;

    if( distance < 1.0 ) distance = 1.0;
    if( distance > 10.0 ) distance = 10.0;

    break;
  }

  mouse_x = x;
  mouse_y = y;

  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init(argc, argv);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc (keyboard);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}
#else
int main(int argc, char** argv)
{
    fprintf (stderr, "This program demonstrates a feature which is not in OpenGL Version 1.0.\n");
    fprintf (stderr, "If your implementation of OpenGL Version 1.0 has the right extensions,\n");
    fprintf (stderr, "you may be able to modify this program to make it run.\n");
    return 0;
}
#endif

