#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "camera.h"
#include <GLFW/glfw3.h>

extern Camera *gCamera;
extern bool gFirstMouse;
extern float gLastX;
extern float gLastY;
extern float deltaTime;
extern bool enableCameraControl;
extern float lod;

inline void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
  (void)window;
  glViewport(0, 0, w, h);
}

inline void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  (void)window;

  if (!enableCameraControl)
    return;

  if (!gCamera)
    return;

  if (gFirstMouse) {
    gLastX = (float)xpos;
    gLastY = (float)ypos;
    gFirstMouse = false;
  }

  float xoffset = (float)xpos - gLastX;
  float yoffset = gLastY - (float)ypos; // reversed: y ranges bottom->top
  gLastX = (float)xpos;
  gLastY = (float)ypos;

  gCamera->ProcessMouseMovement(xoffset, yoffset);
}

inline void scroll_callback(GLFWwindow *window, double xoffset,
                            double yoffset) {
  (void)window;
  (void)xoffset;

  if (!gCamera)
    return;
  gCamera->ProcessMouseScroll((float)yoffset);
}

inline void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  (void)scancode;
  (void)mods;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
    enableCameraControl = !enableCameraControl;
    glfwSetInputMode(window, GLFW_CURSOR,
                     enableCameraControl ? GLFW_CURSOR_DISABLED
                                         : GLFW_CURSOR_NORMAL);
    gFirstMouse = true;
  }

  if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) &&
      (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    const float lodStep = 0.1f;
    lod += (key == GLFW_KEY_RIGHT) ? lodStep : -lodStep;
    if (lod < -3.0f)
      lod = -3.0f;
    if (lod > 3.0f)
      lod = 3.0f;
  }
}

inline void processInput(GLFWwindow *window) {
  if (!enableCameraControl)
    return;

  if (!gCamera)
    return;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    gCamera->ProcessKeyboard(FRONT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    gCamera->ProcessKeyboard(BACK, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    gCamera->ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    gCamera->ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    gCamera->ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    gCamera->ProcessKeyboard(DOWN, deltaTime);
}

#endif
