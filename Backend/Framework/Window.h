#pragma once
#include "Backend/pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

constexpr const int k_defaultWidth = 1600;
constexpr const int k_defaultHeight = 720;
constexpr const char* k_defaultTitle = "Runtime Deferred Linking";
constexpr const int k_defaultFPS = 60;

class DLLMODE Window {
public:
  Window();
  virtual ~Window();

  Window& SetWidth(int newWidth);
  const int& GetWidth() const;
  Window& SetHeight(int newHeight);
  const int& GetHeight() const;
  Window& SetSize(int newWidth, int newHeight);
  float GetAspect() const;

  Window& SetTitle(const std::string& title);
  const std::string& GetTitle() const;

  Window& SetTargetFPS(int fps);
  const int& GetTargetFPS() const;
  const float& GetActualFPS() const;

  Window& Open();
  Window& Close();
  const bool& IsOpen() const;

  Window& InitBackend();
  Window& ShutdownBackend();
  const bool& IsBackendInitalized() const;
        
  Window& StartFrame();
  Window& EndFrame();
  const bool& IsMidFrame() const;

  // Implementation specific update
  virtual void Update() = 0;

  // Implementation specific ImGui calls
  virtual bool ImGuiInit() const = 0;
  virtual void ImGuiFrameStart() const = 0;
  virtual void ImGuiFrameEnd() const = 0;
  virtual void ImGuiShutdown() const = 0;

protected:
  // Initializes the backend graphics implementation
  // Returns true if successful, false otherwise
  // Note: If initialization fails, it's up to the implementation to clean itself up
  virtual bool InitBackendInternal() = 0;
  // Shuts down the backend graphics implementation
  // Returns true if shutdown is permitted and successful
  // Returns false if shutdown request is denied
  // Note: If false is returned, IsBackendInitalized() will continue to return true
  //   False should not be returned on error, instead an exception should be thrown
  //   unless the error can be handled cleanly by the implementation
  virtual bool ShutdownBackendInternal() = 0;

  // Sets up a new frame to be drawn to
  // Returns true if successful, false otherwise
  virtual bool StartFrameInternal() = 0;
  // Finishes drawing to and swaps framebuffers
  // Returns true if successful, false otherwise
  virtual bool EndFrameInternal() = 0;

  // Opens the window
  // Returns true if successful, false otherwise
  virtual bool OpenInternal() = 0;
  // Closes the window
  // Returns true if close is permitted and successful
  // Returns false if close request is denied
  // Note: If false is returned, IsOpen() will continue to return true
  //   False should not be returned on error, instead an exception should be thrown
  //   unless the error can be handled cleanly by the implementation
  virtual bool CloseInternal() = 0;

  void UpdateSize(int newWidth, int newHeight);

private:
  std::string m_title;

  int m_width;
  int m_height;
  int m_targetFPS;
  float m_realFPS;

  bool m_open;
  bool m_backendInitialized;
  bool m_midFrame;
};

class DLLMODE GLWindow : public Window {
public:
  GLWindow();
  ~GLWindow();

  static void DrawDemoTri();

  void Update() override;

  bool ImGuiInit() const override;
  void ImGuiFrameStart() const override;
  void ImGuiFrameEnd() const override;
  void ImGuiShutdown() const override;

protected:
  bool InitBackendInternal() override;
  bool ShutdownBackendInternal() override;
  bool StartFrameInternal() override;
  bool EndFrameInternal() override;
  bool OpenInternal() override;
  bool CloseInternal() override;

private:
  bool InitGlfw();
  bool ShutdownGlfw();

  GLFWwindow* m_glfwWindow;
};
