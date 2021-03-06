#include "renderscene.h"
#include "usercamera.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

RenderScene r_scene;
UserCamera r_camera;
bool paused = true;

void error_callback(int error, const char* description)
{
  std::cerr << "Error ("<<error<<"): " << description << "\n";
}

void cursor_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
  if (r_camera.cursorActive()) {r_camera.handleMouseMove(xpos, ypos);}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  r_camera.handleMouseClick(xpos, ypos, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  r_camera.handleScroll(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int mods)
{
  static bool curstate = false;
  // Escape exits the application
  if (action == GLFW_PRESS)
  {
    switch(key)
    {
      case (GLFW_KEY_ESCAPE):
        glfwSetWindowShouldClose(window, true); break;
      case (GLFW_KEY_SPACE):
      {
        r_camera.toggleCursorState();
        curstate = !curstate;
        if (curstate)
        {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        break;
      }
      case (GLFW_KEY_ENTER):
      {
        r_camera.reset();
        r_scene.resetTAA();
        break;
      }
      case (GLFW_KEY_P):
      {
        paused = !paused;
        break;
      }
    }
  }
  r_camera.handleKey(key, action);
}

void resize_callback(GLFWwindow */*window*/, int width, int height)
{
  r_camera.resize(width,height);
  r_scene.resizeGL(width,height);
}

int main(int argc, char *argv[])
{
  std::string title;
  if (argc > 1) {title = std::string(argv[1]);}
  if (!glfwInit())
  {
    // Initialisation failed
    glfwTerminate();
  }

  // Register error callback
  glfwSetErrorCallback(error_callback);

  // Set our OpenGL version
//  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
  //glfwWindowHint(GLFW_SAMPLES, 16);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  int width = 1000; int height = 1000;
  GLFWwindow* window = glfwCreateWindow(width, height, std::string("Crowd simulation for " + title).c_str(), nullptr, nullptr);


  if (window == nullptr)
  {
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  // Disable the cursor for the FPS camera
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Set the mouse move and click callback
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);

  r_scene.initGL(title.c_str());
  std::cout<<"\n\n\nControls:\n"
             "               forward   W\n"
             "            left/right A   S\n"
             "              backward   D \n"
             "               down/up Q   E\n"
             "\n"
             "                  SPACE\n"
             "         toggle cursor window lock\n"
             "\n"
             "                  ENTER\n"
             "          reset camera position\n"
             "\n"
             "\n";

  // Set the window resize callback and call it once
  glfwSetFramebufferSizeCallback(window, resize_callback);
  resize_callback(window, width, height);

  // Main render loop
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    r_camera.update();
    r_scene.setViewMatrix(r_camera.viewMatrix());
    r_scene.setProjMatrix(r_camera.projMatrix());
    r_scene.setCubeMatrix(r_camera.cubeMatrix());
    r_scene.setCameraLocation(r_camera.getLocation());
    r_scene.paintGL(paused);
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
