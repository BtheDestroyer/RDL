#include "Frontend/pch.h"
#include "Backend/ProgramManager/ProgramManager.h"
#include "Backend/Framework/Image.h"
#include "Backend/RDL/RDL.h"

static RDL::ObjectName s_activeFunction;

int main(int argc, char** argv)
{
  ProgramManager& programManager = ProgramManager::Get();
  programManager.OpenWindow();
  programManager.AddImGuiWindow([]() {
    ProgramManager& programManager = ProgramManager::Get();
    const ImVec2 size(programManager.GetWindow().GetWidth() * 0.5f, float(programManager.GetWindow().GetHeight()));
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("RDL Manager", nullptr, ProgramManager::k_windowFlags))
    {
      static RDL::Manager& rdl = RDL::Manager::Get();
      ImGui::Text("Loaded Plugins: %d", rdl.GetLoadedPluginCount());
      ImGui::Text("Registered Objects:");
      ImGui::Columns(4);
      for (auto& objectName : rdl.GetObjectNames())
      {
        const auto& type = rdl.GetObjectType(objectName);
        ImGui::Text("%s", objectName.ToString().c_str());
        ImGui::NextColumn();
        ImGui::Text("0x%x", rdl.ForceGetPtr<void*>(objectName));
        ImGui::NextColumn();
        ImGui::Text("%s", type.name());
        ImGui::NextColumn();
#define TRY_PRINT_FOR_TYPE(dType, dFmt) \
        if (type == typeid(dType)) \
        { \
          ImGui::Text(dFmt, rdl.GetRef<dType>(objectName)); \
          ImGui::NextColumn(); \
        } \
        else
        TRY_PRINT_FOR_TYPE(unsigned char, "\t%u")
        TRY_PRINT_FOR_TYPE(unsigned short, "\t%u")
        TRY_PRINT_FOR_TYPE(unsigned int, "\t%u")
        TRY_PRINT_FOR_TYPE(unsigned long, "\t%lu")
        TRY_PRINT_FOR_TYPE(unsigned long long, "\t%llu")
        TRY_PRINT_FOR_TYPE(char, "\t%d")
        TRY_PRINT_FOR_TYPE(short, "\t%d")
        TRY_PRINT_FOR_TYPE(int, "\t%d")
        TRY_PRINT_FOR_TYPE(long, "\t%ld")
        TRY_PRINT_FOR_TYPE(long long, "\t%lld")
        TRY_PRINT_FOR_TYPE(char*, "\t%s")
        TRY_PRINT_FOR_TYPE(void*, "\t%x")
        if (type == typeid(void()))
        {
          // Runnable function
          if (ImGui::Button(("Set as active function##" + objectName.ToString()).c_str()))
          {
            s_activeFunction = objectName;
          }
          ImGui::NextColumn();
        }
        else
        {
          // Failure case
          ImGui::NextColumn();
        }
#undef TRY_PRINT_FOR_TYPE
      }
      ImGui::Columns(1);
      ImGui::End();
    }
    });
  programManager.AddImGuiWindow([]() {
    static ProgramManager& programManager = ProgramManager::Get();
    static RDL::Manager& rdl = RDL::Manager::Get();
    const ImVec2 size(programManager.GetWindow().GetWidth() * 0.5f, float(programManager.GetWindow().GetHeight()));
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(size.x, 0.0f));
    if (ImGui::Begin("Function Output", nullptr, ProgramManager::k_windowFlags))
    {
      const auto function = rdl.TryGetPtr<void()>(s_activeFunction);
      if (function)
      {
        ImGui::Text("Active Function: %s", s_activeFunction.ToString().c_str());
        function();
      }
      else
      {
        ImGui::Text("No Current Active Function. Click a button on the left to select its function.");
      }
      ImGui::End();
    }
    });
  RDL::Manager& rdl = RDL::Manager::Get();
  rdl.LoadPlugins();
  while (programManager.IsRunning())
  {
    programManager.Update();
  }
  return 0;
}
