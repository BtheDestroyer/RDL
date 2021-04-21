#include "Backend/pch.h"
#include "Backend/ProgramManager/ProgramManager.h"
#include <array>
#include <functional>
#include <sstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

void ProgramManager::OpenWindow()
{
  m_window.InitBackend();
  m_window.Open();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  m_window.ImGuiInit();
}

void ProgramManager::CloseWindow()
{
  m_window.Close();
  m_window.ShutdownBackend();
  m_window.ImGuiShutdown();
}

bool ProgramManager::IsRunning()
{
  return m_window.IsOpen();
}

void ProgramManager::Update()
{
  m_window.Update();
  m_window.StartFrame();
  DrawImGuiWindows();
  m_window.EndFrame();
}

void ProgramManager::DrawImGuiWindows()
{
  m_window.ImGuiFrameStart();
  ImGui::PushStyleColor(ImGuiCol_TitleBg, ImGui::GetStyleColorVec4(ImGuiCol_TitleBgActive));
  for (ImGuiWindowDrawFunction& window : m_activeImGuiWindows)
  {
    window();
  }
  ImGui::PopStyleColor();
  m_window.ImGuiFrameEnd();
}

std::size_t ProgramManager::AddImGuiWindow(const ImGuiWindowDrawFunction& windowDrawFunction)
{
  m_activeImGuiWindows.emplace_back(windowDrawFunction);
  return m_activeImGuiWindows.size() - 1;
}

const ProgramManager::ImGuiWindowDrawFunction& ProgramManager::GetImGuiWindow(std::size_t index) const
{
  if (index >= m_activeImGuiWindows.size())
  {
    static ImGuiWindowDrawFunction nullFunc;
    return nullFunc;
  }
  return m_activeImGuiWindows[index];
}

void ProgramManager::RemoveImGuiWindow(std::size_t index)
{
  if (index < m_activeImGuiWindows.size())
  {
    m_activeImGuiWindows.erase(m_activeImGuiWindows.begin() + index);
  }
}

GLWindow& ProgramManager::GetWindow()
{
  return m_window;
}

ProgramManager& ProgramManager::Get()
{
  static ProgramManager singleton;
  return singleton;
}

ProgramManager::ProgramManager()
  : m_window(),
    m_activeImGuiWindows()
{
}
