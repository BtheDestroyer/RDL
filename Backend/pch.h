// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef BACKEND_PCH_H
#define BACKEND_PCH_H

// add headers that you want to pre-compile here
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define WIN32_NO_MIN_MAX
#define NOMINMAX
// Windows Header Files
#include <windows.h>
#pragma warning(disable:4251)
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef BACKEND_PROJECT
#define DLLMODE __declspec(dllexport)
#else
#define DLLMODE __declspec(dllimport)
#endif
#ifndef IMGUI_API
#define IMGUI_API DLLMODE
#else
#define IMGUI_API IMGUI_API DLLMODE
#endif

#endif //PCH_H
