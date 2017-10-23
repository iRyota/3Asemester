#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//-----------------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------------
// 視点情報
static double distance = 5.0, pitch = 0.0, yaw = 0.0;

// マウス入力情報
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

//ライティングの情報
static const GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
static const GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

//-----------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------
void init(void)
{
  // クリアの値の設定
  glClearColor (0.0, 0.0, 0.0, 0.0);
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

//-----------------------------------------------------------------------------------
// レンダリング
//-----------------------------------------------------------------------------------
void display(void)
{
  // フレームバッファのクリア
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 視点の設定
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glRotatef( -pitch, 1.0, 0.0, 0.0 );
  glRotatef( -yaw, 0.0, 1.0, 0.0 );

  // ティーポットを描く
  glPushMatrix();
  {
    glutSolidCube( 1.5 );
  }
  glPopMatrix();

  glutSwapBuffers();
}


//-----------------------------------------------------------------------------------
// ウィンドウリサイズのコールバック関数
//-----------------------------------------------------------------------------------
void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);

}

//-----------------------------------------------------------------------------------
// キーボード入力のコールバック関数
//-----------------------------------------------------------------------------------
void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 27://ESCで終了
    exit(0);
    break;
  }
}

//-----------------------------------------------------------------------------------
// マウスクリックのコールバック関数
//-----------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
  mouse_button = button;
  mouse_x = x;	mouse_y = y;

  if(state == GLUT_UP){
    mouse_button = -1;
  }

  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// マウス移動のコールバック関数
//-----------------------------------------------------------------------------------
void motion(int x, int y)
{
  switch(mouse_button){
  case GLUT_LEFT_BUTTON://マウス左ボタン

    if( x == mouse_x && y == mouse_y )
      return;

    yaw -= (GLfloat) (x - mouse_x) / 10.0;
    pitch -= (GLfloat) (y - mouse_y) / 10.0;

    break;

  case GLUT_RIGHT_BUTTON://マウス右ボタン

    if( y == mouse_y )
      return;

    if( y < mouse_y )
      distance += (GLfloat) (mouse_y - y)/50.0;
    else
      distance -= (GLfloat) (y - mouse_y)/50.0;

    if( distance < 1.0 ) distance = 1.0;
    if( distance > 10.0 ) distance = 10.0;

    break;
  }

  mouse_x = x;
  mouse_y = y;

  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// アイドル時のコールバック関数
//-----------------------------------------------------------------------------------
void idle()
{
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// メイン関数
//-----------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize (640, 480);
  glutInitWindowPosition (50, 50);
  glutCreateWindow ("planet");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  glutMainLoop();

  return 0;
}
