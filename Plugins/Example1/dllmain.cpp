// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Backend/pch.h"
#include "Backend/RDL/RDL.h"
#include "Backend/ImGui/imgui.h"

void TestFunction()
{
  ImGui::Text("This is an unexposed function from a loaded DLL! :D");
}

int TestReturningFunction()
{
  ImGui::Text("This is an unexposed function from a loaded DLL! :D");
  ImGui::Text("It returns an int, so you shouldn't try to run it like a void() function");
  return 42;
}

static unsigned s_counter = 5;

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
    rdl.Register(TestFunction, "TestFunction"_hash);
    rdl.Register(TestReturningFunction, "TestReturningFunction"_hash);
    rdl.Register(s_counter, "Counter"_hash);
    break;
  case DLL_PROCESS_DETACH:
    // Happens when the DLL is unloaded
    rdl.Unregister("TestFunction"_hash);
    rdl.Unregister("TestReturningFunction"_hash);
    rdl.Unregister("Counter"_hash);
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  }
  return TRUE;
}

