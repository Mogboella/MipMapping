#include <glad/glad.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_style.h"

#include <string>

#include "callbacks.h"
#include "camera.h"
#include "model.h"
#include "shaders.h"

// TODO, make rotate

using std::cerr;
using std::cout;
using std::endl;

int width = 1920, height = 1080;
const char *project_name = "Lab 4 - MipMapping";

Camera camera(0.0f, 5.0f, 30.0f, 0.0f, 1.0f, 0.0f, -90.0f, -5.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- globals for input ---
Camera *gCamera = nullptr;
float gLastX = 0.0f;
float gLastY = 0.0f;
bool gFirstMouse = true;

bool showUI = true;
bool enableCameraControl = false;

static int mode = 0;
float lod = 0.0f;
static int textureIndex = 0;

int main() {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(width, height, project_name, nullptr, nullptr);

  if (!window) {
    cerr << "Failed to create window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ApplyCustomStyle();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  gCamera = &camera;
  gLastX = width / 2.0f;
  gLastY = height / 2.0f;

  cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

  // Configure OpenGL
  glEnable(GL_DEPTH_TEST);

  // Load shaders
  Shader shader("shaders/main.vert", "shaders/main.frag");

  // Load Texjpge
  unsigned int chessTex =
      Model::loadTexture2D("assets/textures/chess_board.jpeg");
  unsigned int uvGridTex = Model::loadTexture2D("assets/uvtex.jpeg");
  unsigned int lineTex = Model::loadTexture2D("assets/moire.jpg");
  unsigned int textures[] = {chessTex, uvGridTex, lineTex};

  // ---------- Floor plane geometry ----------
  float floorHalf = 100.0f; // 100x100 floor
  float uvRepeat = 50.0f;   // tile texture a lot to see mipmapping effects

  float floorVertices[] = {
      -floorHalf, 0.0f, -floorHalf, 0.0f, 1.0f, 0.0f, 0.0f,     0.0f,
      floorHalf,  0.0f, -floorHalf, 0.0f, 1.0f, 0.0f, uvRepeat, 0.0f,
      floorHalf,  0.0f, floorHalf,  0.0f, 1.0f, 0.0f, uvRepeat, uvRepeat,
      -floorHalf, 0.0f, floorHalf,  0.0f, 1.0f, 0.0f, 0.0f,     uvRepeat};

  unsigned int floorIndices[] = {0, 1, 2, 0, 2, 3};

  unsigned int floorVAO, floorVBO, floorEBO;
  glGenVertexArrays(1, &floorVAO);
  glGenBuffers(1, &floorVBO);
  glGenBuffers(1, &floorEBO);

  glBindVertexArray(floorVAO);

  glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));

  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Clear buffers
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ImGui UI
    if (showUI) {
      ImGui::Begin("Material Controls");
      ImGui::RadioButton("Disable Mipmapping", &mode, 0);

      ImGui::Separator();
      ImGui::Text("Switch MipMap Modes.");

      ImGui::RadioButton("Linear-Linear", &mode, 1);
      ImGui::SameLine();
      ImGui::RadioButton("Nearest-Linear", &mode, 2);

      ImGui::RadioButton("Linear-Nearest", &mode, 3);
      ImGui::SameLine();
      ImGui::RadioButton("Nearest-Nearest", &mode, 4);

      ImGui::Separator();

      ImGui::Text("Toggle this and look at the far end of the floor.");
      ImGui::Separator();
      ImGui::SliderFloat("LOD bias", &lod, -3.0f, 3.0f, "%.2f");

      // ImGui::Separator();
      // ImGui::Text("Switch Textures.");

      // ImGui::RadioButton("Chess Board", &textureIndex, 0);
      // ImGui::RadioButton("UV Grid", &textureIndex, 1);
      // ImGui::RadioButton("Line Grid", &textureIndex, 2);

      ImGui::Separator();

      ImGui::Text("Use WASD + Space/Shift to move, mouse to look around.");
      ImGui::Text("Press LEFT/RIGHT to adjust LOD bias.");
      ImGui::Text("Press TAB to toggle this UI.");
      ImGui::End();
    }

    // Set transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("cameraPos", camera.position);

    switch (mode) {
    case 0:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      break;
    case 1:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      break;
    case 2:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_LINEAR);
      break;
    case 3:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_NEAREST);
      break;
    case 4:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
      break;
    default:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod);

    glActiveTexture(GL_TEXTURE0);

    unsigned int currentTex = textures[textureIndex];
    glBindTexture(GL_TEXTURE_2D, currentTex);

    shader.setInt("tex", 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 10.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);

    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (showUI) {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
}
