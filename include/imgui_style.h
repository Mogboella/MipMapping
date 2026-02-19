#pragma once
#include "imgui.h"

inline void ApplyCustomStyle() {
  ImGuiStyle &s = ImGui::GetStyle();

  // Layout
  s.WindowPadding = ImVec2(14, 14);
  s.FramePadding = ImVec2(12, 8);
  s.ItemSpacing = ImVec2(10, 10);
  s.ItemInnerSpacing = ImVec2(10, 8);
  s.IndentSpacing = 18.0f;
  s.ScrollbarSize = 16.0f;
  s.GrabMinSize = 12.0f;

  // Rounded
  s.WindowRounding = 14.0f;
  s.ChildRounding = 12.0f;
  s.FrameRounding = 12.0f;
  s.PopupRounding = 12.0f;
  s.ScrollbarRounding = 12.0f;
  s.GrabRounding = 10.0f;
  s.TabRounding = 12.0f;

  // Borders
  s.WindowBorderSize = 1.0f;
  s.FrameBorderSize = 1.0f;
  s.PopupBorderSize = 1.0f;
  s.TabBorderSize = 1.0f;

  ImVec4 *c = s.Colors;

  c[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.09f, 0.11f, 0.94f);
  c[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.11f, 0.13f, 0.85f);
  c[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.11f, 0.13f, 0.96f);

  c[ImGuiCol_Border] = ImVec4(0.30f, 0.32f, 0.38f, 0.50f);

  c[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.16f, 0.20f, 0.85f);
  c[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.28f, 0.90f);
  c[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.24f, 0.31f, 0.95f);

  c[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.11f, 0.13f, 0.95f);
  c[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.13f, 0.16f, 0.98f);

  c[ImGuiCol_Button] = ImVec4(0.35f, 0.50f, 0.90f, 0.85f);
  c[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.56f, 0.96f, 0.95f);
  c[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.44f, 0.86f, 1.00f);

  c[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.62f, 0.95f, 0.95f);
  c[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.70f, 1.00f, 1.00f);
  c[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.62f, 0.95f, 1.00f);

  c[ImGuiCol_Header] = ImVec4(0.18f, 0.20f, 0.26f, 0.80f);
  c[ImGuiCol_HeaderHovered] = ImVec4(0.23f, 0.25f, 0.32f, 0.90f);
  c[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.28f, 0.36f, 0.95f);

  c[ImGuiCol_Text] = ImVec4(0.93f, 0.94f, 0.96f, 1.00f);
  c[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.62f, 0.68f, 1.00f);
}
