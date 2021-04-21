#include "Backend/pch.h"
#include "Backend/Framework/Debug.h"
#include "Backend/Framework/Window.h"
#include "Backend/ImGui/imgui.h"
#include "Backend/ImGui/backends/imgui_impl_glfw.h"
#include "Backend/ImGui/backends/imgui_impl_opengl3.h"
#include "Backend/ProgramManager/ProgramManager.h"
#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>

Window::Window()
  : m_title(k_defaultTitle),
  m_width(k_defaultWidth), m_height(k_defaultHeight),
  m_targetFPS(k_defaultFPS), m_realFPS(0),
  m_open(false),
  m_backendInitialized(false),
  m_midFrame(false)
{
}

Window::~Window()
{
  if (IsBackendInitalized())
    ShutdownBackend();
}

Window& Window::SetWidth(int newWidth)
{
  m_width = newWidth;
  return *this;
}

const int& Window::GetWidth() const
{
  return m_width;
}

Window& Window::SetHeight(int newHeight)
{
  m_height = newHeight;
  return *this;
}

const int& Window::GetHeight() const
{
  return m_height;
}

Window& Window::SetSize(int newWidth, int newHeight)
{
  m_width = newWidth;
  m_height = newHeight;
  return *this;
}

float Window::GetAspect() const
{
  return float(m_width) / float(m_height);
}

Window& Window::SetTitle(const std::string& title)
{
  m_title = title;
  return *this;
}

const std::string& Window::GetTitle() const
{
  return m_title;
}

Window& Window::SetTargetFPS(int fps)
{
  m_targetFPS = fps;
  return *this;
}

const int& Window::GetTargetFPS() const
{
  return m_targetFPS;
}

const float& Window::GetActualFPS() const
{
  return m_realFPS;
}

Window& Window::Open()
{
  DEBUG_REQUIRE_FALSE(IsOpen());
  m_open = OpenInternal();
  return *this;
}

Window& Window::Close()
{
  DEBUG_REQUIRE_TRUE(IsOpen());
  m_open = !CloseInternal();
  return *this;
}

const bool& Window::IsOpen() const
{
  return m_open;
}

Window& Window::InitBackend()
{
  DEBUG_REQUIRE_FALSE(IsBackendInitalized());
  m_backendInitialized = InitBackendInternal();
  return *this;
}

Window& Window::ShutdownBackend()
{
  DEBUG_REQUIRE_TRUE(IsBackendInitalized());
  if (IsOpen())
    Close();
  m_backendInitialized = !ShutdownBackendInternal();
  return *this;
}

const bool& Window::IsBackendInitalized() const
{
  return m_backendInitialized;
}

Window& Window::StartFrame()
{
  DEBUG_REQUIRE_TRUE(IsBackendInitalized());
  DEBUG_REQUIRE_FALSE(IsMidFrame());
  m_midFrame = StartFrameInternal();
  return *this;
}

Window& Window::EndFrame()
{
  DEBUG_REQUIRE_TRUE(IsMidFrame());
  m_midFrame = !EndFrameInternal();
  return *this;
}

const bool& Window::IsMidFrame() const
{
  return m_midFrame;
}

void Window::UpdateSize(int newWidth, int newHeight)
{
  m_width = newWidth;
  m_height = newHeight;
}

////////////////////////////
// OpenGL Implementation //
//////////////////////////

#if defined(_DEBUG) || defined(_PROFILE)
#define GLCall(func) ClearGLError(#func, __FILE__, __LINE__);\
                     func;\
                     DEBUG_REQUIRE_TRUE(CheckGLError(#func, __FILE__, __LINE__))
#else
#define GLCall(func) func;
#endif

#if defined(_DEBUG) || defined(_PROFILE)
static void ClearGLError(const char* function, const char* file, int line)
{
  const int k_countMax = 100;
  int count = 0;
  while (GLenum error = glGetError())
  {
    if (++count >= k_countMax)
    {
      std::cerr
        << "Couldn't clear errors!"
        << "[GL] (0x"
        << std::hex
        << std::setfill('0')
        << std::setw(4)
        << error
        << std::dec
        << std::setw(0)
        << ") "
        << file
        << ":"
        << line
        << std::endl
        << function
        << std::endl;
      DEBUG_BREAK();
    }
  }
}

static bool CheckGLError(const char* function, const char* file, int line)
{
  bool noErrors = true;
  while (GLenum error = glGetError())
  {
    std::cerr
      << "[GL] (0x"
      << std::hex
      << std::setfill('0')
      << std::setw(4)
      << error
      << std::dec
      << std::setw(0)
      << ") "
      << file
      << ":"
      << line
      << std::endl
      << function
      << std::endl;
    noErrors = false;
  }
  return noErrors;
}
#endif

GLWindow::GLWindow()
  : Window(),
    m_glfwWindow(nullptr)
{
}

GLWindow::~GLWindow()
{
  if (IsBackendInitalized())
    ShutdownBackend();
}

bool GLWindow::InitBackendInternal()
{
  if (InitGlfw())
  {
    return true;
  }
  return false;
}

bool GLWindow::ShutdownBackendInternal()
{
  if (!ShutdownGlfw())
    throw std::runtime_error("Failed to shutdown GLFW!");
  return true;
}

bool GLWindow::StartFrameInternal()
{
  GLCall(glClearColor(0.2f, 0.4f, 0.8f, 0.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  return true;
}

bool GLWindow::EndFrameInternal()
{
  glfwSwapBuffers(m_glfwWindow);
  glfwPollEvents();
  // Update window size for next frame
  int display_w, display_h;
  glfwGetFramebufferSize(m_glfwWindow, &display_w, &display_h);
  GLCall(glViewport(0, 0, display_w, display_h));
  if (display_w != GetWidth() || display_h != GetHeight())
  {
    UpdateSize(display_w, display_h);

    // Reconstruct framebuffer
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }
  if (glfwWindowShouldClose(m_glfwWindow))
  {
    ProgramManager::Get().CloseWindow();
  }
  return true;
}

bool GLWindow::OpenInternal()
{
  DEBUG_REQUIRE_FALSE(m_glfwWindow);
  m_glfwWindow = glfwCreateWindow(GetWidth(), GetHeight(), GetTitle().c_str(), nullptr, nullptr);
  if (!m_glfwWindow)
  {
    DEBUG_BREAK(); // Error creating window
    return false;
  }
  glfwMakeContextCurrent(m_glfwWindow);
  if (glewInit() != GLEW_OK)
  {
    DEBUG_BREAK(); // Error initializing glew
    glfwDestroyWindow(m_glfwWindow);
    m_glfwWindow = nullptr;
    return false;
  }

  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glDepthFunc(GL_LESS));
  GLCall(glEnable(GL_CULL_FACE));
  GLCall(glCullFace(GL_BACK));
  GLCall(glEnable(GL_POLYGON_OFFSET_LINE));
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  return true;
}

bool GLWindow::CloseInternal()
{
  DEBUG_REQUIRE_TRUE(m_glfwWindow);
  glfwDestroyWindow(m_glfwWindow);
  m_glfwWindow = nullptr;
  return true;
}

bool GLWindow::InitGlfw()
{
  if (!glfwInit())
  {
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  return true;
}


bool GLWindow::ShutdownGlfw()
{
  glfwTerminate();
  return true;
}

void GLWindow::DrawDemoTri()
{
  GLCall(glBegin(GL_TRIANGLES));
  GLCall(glVertex2f(-0.5f, -0.5f));
  GLCall(glVertex2f(0.0f, 0.5f));
  GLCall(glVertex2f(0.5f, -0.5f));
  GLCall(glEnd());
}

void GLWindow::Update()
{
}

bool GLWindow::ImGuiInit() const
{
  if (!::ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true))
  {
    return false;
  }
  if (!::ImGui_ImplOpenGL3_Init("#version 330"))
  {
    return false;
  }
  return true;
}

void GLWindow::ImGuiFrameStart() const
{
  ::ImGui_ImplOpenGL3_NewFrame();
  ::ImGui_ImplGlfw_NewFrame();
  ::ImGui::NewFrame();
}

void GLWindow::ImGuiFrameEnd() const
{
  ::ImGui::EndFrame();
  ::ImGui::Render();
  ::ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLWindow::ImGuiShutdown() const
{
  ::ImGui_ImplOpenGL3_Shutdown();
  ::ImGui_ImplGlfw_Shutdown();
  ::ImGui::DestroyContext();
}
