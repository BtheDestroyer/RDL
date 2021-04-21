#include "Backend/pch.h"
#include "Backend/Framework/Debug.h"
#include "Backend/Framework/Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image()
	: m_data(nullptr),
  	m_width(0), m_height(0), m_channels(0),
		m_glTexture(0)
{}

Image::Image(const Path& path)
	: m_data(nullptr),
		m_width(0), m_height(0), m_channels(0),
		m_glTexture(0)
{
	m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	GenerateGLTexture();
}

Image::Image(const Image& other)
	: m_data(nullptr),
		m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels),
		m_glTexture(0)
{
	const size_t size = m_width * m_height * m_channels;
	m_data = static_cast<unsigned char*>(stbi__malloc(size));
	std::memcpy(m_data, other.m_data, size);
	GenerateGLTexture();
}

Image::Image(Image&& other) noexcept
	: m_data(other.m_data),
		m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels),
		m_glTexture(0)
{
	GenerateGLTexture();
	other.m_data = nullptr;
}

Image::~Image()
{
	if (m_data)
	{
		stbi_image_free(m_data);
	}
}

void Image::GenerateGLTexture()
{
	glGenTextures(1, &m_glTexture);
	if (m_glTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_glTexture);
		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
		if (m_channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
		}
		else if (m_channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
		}
		else
		{
			DEBUG_BREAK(); // Unknown channels
		}
		stbi_image_free(m_data);
	}
}

GLuint Image::GetGLTexture() const
{
	return m_glTexture;
}

void Image::ImGuiDraw()
{
	ImGuiDraw(ImVec2{float(m_width), float(m_height)});
}

void Image::ImGuiDraw(const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	ImGui::Image((ImTextureID)(uint64_t(GetGLTexture())), size);
}
