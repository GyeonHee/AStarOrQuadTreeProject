// Win32 API를 사용하기 위해 include.
#include <Windows.h>
#include "Game/Game.h"

HWND hWnd;

// 창에서 메시지가 발생하면 호출될 함수.
LRESULT WINAPI WindowProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // 창이 삭제되면 발생하는 메시지.
    case WM_DESTROY:
        // 프로그램 종료 메시지 발생시키기.
        PostQuitMessage(0);
        break;
    }

    // 나머지 메시지는 윈도우 기본 설정으로 처리.
    return DefWindowProc(hwnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // 윈도우 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassEx(&wc);

    // 윈도우 생성
    HWND hWnd = CreateWindow(L"MyWindowClass", L"DX11 Engine",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);

    Game game;
    if (FAILED(game.InitD3D(hWnd, 1280, 720)))
        return 0;

    // 메시지 루프
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            game.TickFrame();    // Update 호출
            game.RenderFrame();  // Render + Present 호출
        }
    }
    game.CleanUp(); // 리소스 해제
    return (int)msg.wParam;
}