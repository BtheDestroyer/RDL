// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Backend/pch.h"
#include "Backend/RDL/RDL.h"
#include "Backend/ImGui/imgui.h"

void TextRDLVariable()
{
  static RDL::Object<unsigned> counter("Counter"_hash);
  ImGui::Text("The DLL this function is from only contains it.");
  ImGui::Text("It has no other variables or functions (other than DllMain)");
  if (counter.IsValid())
  {
    ImGui::Text("But it still knows the RDL variable \"Counter\" has a value of %u", *counter);
    ImGui::Text("And it can also change that value with these:");
    if (ImGui::Button("Increase"))
    {
      ++counter;
    }
    if (ImGui::Button("Decrease"))
    {
      --counter;
    }
  }
  else
  {
    ImGui::Text("But it still knows there's no RDL variable named \"Counter\"");
  }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
  static RDL::Manager& rdl = RDL::Manager::Get();
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    // Happens when the DLL is loaded
    rdl.Register(TextRDLVariable, "TextRDLVariable"_hash);
    break;
  case DLL_PROCESS_DETACH:
    // Happens when the DLL is unloaded
    rdl.Unregister("TextRDLVariable"_hash);
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  }
  return TRUE;
}

