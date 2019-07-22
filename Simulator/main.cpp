#include <iostream>

#include <Windows.h>

#include "Freeglut/include/GL/freeglut.h"

#include "Object/object.hpp"

using namespace std;

Object some_obj;

Axes axes1;
Axes axes2;

static float g_x {};
static float g_y { 0.0F };
static float g_z { -2.0F };

static float g_rot_z {};

static float g_cam_x {};
static float g_cam_y {};
static float g_cam_z { 20.0F };

static bool g_cam_shaken {};

static float g_cam_target_x {};
static float g_cam_target_y {};
static float g_cam_target_z { 20.0F };

static int g_mouse_prev_drag_x {};
static int g_mouse_prev_drag_y {};

const static int g_framebuffer_max_width  { 800 };
const static int g_framebuffer_max_height { 600 };

const static int g_framebuffer_min_width  { 640 };
const static int g_framebuffer_min_height { 480 };

static GLsizei g_framebuffer_width  { 800 };
static GLsizei g_framebuffer_height { 600 };

void Keyboard_callback(unsigned char key, int x, int y)
{
  // x and y are the mouse's location

  switch (key)
  {
    case '\x1B':
    {
      exit(EXIT_SUCCESS);
      break;
    }

    case 'W':
      // fallthrough
    case 'w':
    {
      g_y += 0.1F;
      some_obj.Move(0.0F, 0.1F, 0.0F);

      break;
    }

    case 'A':
      // fallthrough
    case 'a':
    {
      g_x -= 0.1F;
      some_obj.Move(-0.1F, 0.0F, 0.0F);

      break;
    }

    case 'S':
      // fallthrough
    case 's':
    {
      g_y -= 0.1F;
      some_obj.Move(0.0F, -0.1F, 0.0F);

      break;
    }

    case 'D':
      // fallthrough
    case 'd':
    {
      g_x += 0.1F;
      some_obj.Move(0.1F, 0.0F, 0.0F);

      break;
    }

    case 'Q':
    // fallthrough
    case 'q':
    {
      g_rot_z += 0.1F;

      float rot[] = {  cos(g_rot_z), -sin(g_rot_z), 0.0F
                     , sin(g_rot_z),  cos(g_rot_z), 0.0F
                     , 0.0F,          0.0F,         0.0F
                    };

      some_obj.RotateRel(rot);

      break;
    }

    case 'E':
      // fallthrough
    case 'e':
    {
      g_rot_z -= 0.1F;

      float rot[] = {  cos(g_rot_z), -sin(g_rot_z), 0.0F
                     , sin(g_rot_z),  cos(g_rot_z), 0.0F
                     , 0.0F,          0.0F,         0.0F
                    };

      some_obj.RotateRel(rot);

      break;
    }

    case '+':
    {
      g_z += 0.1F;
      break;
    }

    case '-':
    {
      g_z -= 0.1F;
      break;
    }
  }

  //glutGetModifiers

  //some_obj.Place(g_x, g_y, g_z);
}

void Keyboard_special_callback(int key, int x, int y)
{
  // x and y are the mouse's location

  switch (key)
  {
    case GLUT_KEY_F10:
    {
      exit(EXIT_SUCCESS);
      break;
    }

    case GLUT_KEY_UP:
    {
      g_cam_y += 0.1F;
      g_cam_target_y += 0.1F;
      break;
    }

    case GLUT_KEY_LEFT:
    {
      g_cam_x -= 0.1F;
      g_cam_target_x -= 0.1F;
      break;
    }

    case GLUT_KEY_DOWN:
    {
      g_cam_y -= 0.1F;
      g_cam_target_y -= 0.1F;
      break;
    }

    case GLUT_KEY_RIGHT:
    {
      g_cam_x += 0.1F;
      g_cam_target_x += 0.1F;
      break;
    }
  }

  //glutGetModifiers

  //some_obj.Place(g_x, g_y, g_z);
}

void Mouse_callback(int button, int state, int x, int y)
{
  switch (button)
  {
    case GLUT_LEFT_BUTTON: // 0?
    {
      switch (state)
      {
        case GLUT_DOWN:
          g_mouse_prev_drag_x = g_mouse_prev_drag_y = 0;
          break;

        case GLUT_UP:
          break;
      }
      break;
    }

    case GLUT_RIGHT_BUTTON: // 1?
      break;
    case GLUT_MIDDLE_BUTTON:
      break;
  }

  //glutGetModifiers
}

void Mouse_drag_callback(int x, int y)
{
  g_cam_shaken = { true };

  if (0.0F == g_mouse_prev_drag_x)
  {
    g_mouse_prev_drag_x = { x };
  }
  else
  {
    float d { static_cast<float>(x - g_mouse_prev_drag_x) };

    g_cam_target_x -= d / 500.0F;

    g_mouse_prev_drag_x = { x };
  }

  if (0.0F == g_mouse_prev_drag_y)
  {
    g_mouse_prev_drag_y = { y };
  }
  else
  {
    float d{ static_cast<float>(y - g_mouse_prev_drag_y) };

    g_cam_target_y += d / 500.0F;

    g_mouse_prev_drag_y = { y };
  }
}

void Mouse_move_callback(int x, int y)
{

}

void Mouse_wheel_callback(int wheel, int direction, int x, int y)
{
  // wheel - wheel number

  if (direction > 0)
  {
    g_cam_z -= 0.5F;
  }
  else
  {
    g_cam_z += 0.5F;
  }
}

static void Init_renderer()
{
  glClearColor(0.25F, 0.25F, 0.25F, 0.0F);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  //glEnable(GL_NORMALIZE);
}

void Display_callback()
{
  
}

void Window_resize_plug(int width, int height)
{
  glutReshapeWindow(800, 600);
}

void Render(int in_value)
{
  //static float angle {};

  GLsizei main_w { static_cast<GLsizei>(static_cast<float>(g_framebuffer_width) / 1.5F) };
  GLsizei main_h { g_framebuffer_height };

  if (0 == main_h)
  {
    main_h = { 1 };
  }

  glViewport(0, 0, main_w, main_h);

  GLdouble main_aspect_ratio { static_cast<GLdouble>(main_w) / static_cast<GLdouble>(main_h) };

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //if (main_w <= main_h)
  //{
    //glOrtho(-10.0F, 10.0F, -10.0F / main_aspect_ratio, 10.0F / main_aspect_ratio, -1.0F, 200.0F);
  //}
  //else
  //{
    //glOrtho(-10.0F * main_aspect_ratio, 10.0F * main_aspect_ratio, -10.0F, 10.0F, -1.0F, 200.0F);
  //}

  gluPerspective(60.0, main_aspect_ratio, 1.0, 200.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (false == g_cam_shaken)
  {
    gluLookAt(g_cam_x, g_cam_y, g_cam_z, g_cam_x, g_cam_y, 0.0F, 0.0F, 1.0F, 0.0F);
  }
  else
  {
    gluLookAt(g_cam_x, g_cam_y, g_cam_z, g_cam_target_x, g_cam_target_y, 0.0F, 0.0F, 1.0F, 0.0F);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glRotatef(10.0F, 1.0F, 0.0F, 0.0F);
  //glRotatef(-10.0F, 0.0F, 1.0F, 0.0F);
  //glTranslatef(0.0F, 0.0F, 2.0F);

  //glRotatef(angle, 1.0F, 0.0F, 0.0F);
  //angle++;

  some_obj.Render();
  some_obj.Get_front().Render(some_obj.Get_center());
  axes1.Draw();

  GLsizei side_w { static_cast<GLsizei>(static_cast<GLfloat>(g_framebuffer_width) / 2.0F) }; // 3.0F
  GLsizei side_h { g_framebuffer_height };

  glViewport(main_w, 0, side_w, side_h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60.0F, main_aspect_ratio, 1.0F, 200.0F);
  
  glColor3f(0.0F, 0.5F, 0.0F);

  glRectf(0.0F, 0.0F, 1.0F, 0.5F);
  axes2.Draw();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0.0F, 0.0F, 20.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F);

  glutSwapBuffers();

  glutTimerFunc(33, Render, 0); // 30 FPS
}

int main(int argc, char **argv)
{
  some_obj = Object({ g_x, g_y, g_z }, { 2.0F, 2.0F, 2.0F }, { 0.0F, 1.0F, 0.0F });

  some_obj.Set_geometry(/*cube_1_1_1.Get_geometry()*/);

  axes1 = Axes({0.0F, 0.0F, 0.0F}, 10.0F, 1.0F);
  axes2 = Axes({0.0F, 0.0F, 0.0F}, 10.0F, 1.0F);

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(0, 0);

  glutCreateWindow("Simulator");
  
  Init_renderer();

  glutKeyboardFunc(&Keyboard_callback);
  glutSpecialFunc(&Keyboard_special_callback);
  //glutSpecialUpFunc

  glutMouseFunc(&Mouse_callback);
  glutMotionFunc(&Mouse_drag_callback); // Only drag
  glutPassiveMotionFunc(&Mouse_move_callback); // Only NOT drag
  glutMouseWheelFunc(&Mouse_wheel_callback);

  glutDisplayFunc(&Display_callback);

  glutReshapeFunc(&Window_resize_plug);

  glutTimerFunc(33, Render, 0); // 30 FPS

  glutMainLoop();

  //cout << "Hi\n";

  //system("pause");

  return EXIT_SUCCESS;
}
