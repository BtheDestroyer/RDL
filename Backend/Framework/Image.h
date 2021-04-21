#pragma once
#include "Backend/pch.h"
#include "Backend/ImGui/imgui.h"
#include <GL/glew.h>
#include <string>

class DLLMODE Image
{
public:
	using Path = std::string;

	Image();
	Image(const Path& path);
	Image(const Image& other);
	Image(Image&& other) noexcept;
	Image& operator=(const Image& other) = default;
	Image& operator=(Image&& other) noexcept = default;
	~Image();

	[[nodiscard]] GLuint GetGLTexture() const;

	void ImGuiDraw();
	void ImGuiDraw(const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));

protected:

private:
	void GenerateGLTexture();

	unsigned char* m_data;
	int m_width, m_height, m_channels;
	GLuint m_glTexture;
};