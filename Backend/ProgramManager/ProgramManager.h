#pragma once
#include "Backend/pch.h"
#include "Backend/Framework/Window.h"
#include "Backend/Framework/Version.h"
#include "Backend/ImGui/imgui.h"

class DLLMODE ProgramManager
{
public:
  using ImGuiWindowDrawFunction = std::function<void()>;
  constexpr static ImGuiWindowFlags k_windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

  void OpenWindow();
  void CloseWindow();

  bool IsRunning();
  void Update();

  void DrawImGuiWindows();
  std::size_t AddImGuiWindow(const ImGuiWindowDrawFunction& windowDrawFunction);
  [[nodiscard]] const ImGuiWindowDrawFunction& GetImGuiWindow(std::size_t index) const;
  void RemoveImGuiWindow(std::size_t index);
  
  [[nodiscard]] GLWindow& GetWindow();

  static ProgramManager& Get();

protected:
  ProgramManager();

private:
  GLWindow m_window;
  std::vector<ImGuiWindowDrawFunction> m_activeImGuiWindows;
};
