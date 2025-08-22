#pragma once

#include <Windows.h>

class Window
{
	friend class Engine;

public:
	Window(
		const wchar_t* windowName,
		unsigned int width,
		unsigned int height,
		WNDPROC messageProcedure
	);

	~Window();

public:
	wchar_t* name = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;
	HWND handle = nullptr;
	HINSTANCE instance = nullptr;
};