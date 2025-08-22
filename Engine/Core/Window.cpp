#include "Window.h"

Window::Window(
	const wchar_t* windowName,
	unsigned int width,
	unsigned int height,
	WNDPROC messageProcedure)
	: width(width), height(height), instance(GetModuleHandleA(nullptr))
{
	size_t length = lstrlen(windowName) + 1;
	name = new wchar_t[length];

	lstrcpy(name, windowName);

	// 창 생성을 위한 속성 구조체.
	WNDCLASS wc = { };
	wc.lpfnWndProc = messageProcedure;
	wc.hInstance = instance;		// 프로그램 포인터.
	//wc.hInstance = hInstance;		// 프로그램 포인터.
	wc.lpszClassName = L"Sample Window Class";
	wc.style = CS_VREDRAW | CS_HREDRAW;

	// 클래스 등록
	if (!RegisterClass(&wc))
	{
		return;
	}

	// 창 크기 조정.
	RECT rect{ 0, 0, (long)width, (long)height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//GetClientRect()

	// 창 크기 구하기.
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom = rect.bottom;

	// 위치 (가운데).
	int xPosition = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
	int yPosition = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

	// 창 생성..
	handle = CreateWindow(
		wc.lpszClassName,
		name,
		WS_OVERLAPPEDWINDOW,
		xPosition, yPosition,
		windowWidth, windowHeight,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr
	);

	// 생성 실패하면 종료.
	if (!handle)
	{
		return;
	}

	// 업데이트 / 보여주기.
	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
}

Window::~Window()
{
	if (name)
	{
		delete[] name;
		name = nullptr;
	}
}