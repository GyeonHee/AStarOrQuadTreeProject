// Win32 API를 사용하기 위한 include.
#include <Windows.h>
#include "Game/Game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

HWND hWnd;
Game game;

// â���� �޽����� �߻��ϸ� ȣ��� �Լ�.
LRESULT WINAPI WindowProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // â�� �����Ǹ� �߻��ϴ� �޽���.
    case WM_DESTROY:
        // ���α׷� ���� �޽��� �߻���Ű��.
        PostQuitMessage(0);
        break;
    }

    // ������ �޽����� ������ �⺻ �������� ó��.
    return DefWindowProc(hwnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
//{
//    // ������ Ŭ���� ���
//    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc = WindowProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = L"MyWindowClass";
//    RegisterClassEx(&wc);
//
//    // ������ ����
//    HWND hWnd = CreateWindow(L"MyWindowClass", L"DX11 Engine",
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
//        1280, 720, nullptr, nullptr, hInstance, nullptr);
//
//    ShowWindow(hWnd, nCmdShow);
//
//    Game game;
//    if (FAILED(game.InitD3D(hWnd, 1280, 720)))
//        return 0;
//
//    
//    MSG msg = {};
//    while (msg.message != WM_QUIT)
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//        else
//        {
//            game.Run();
//        }
//    }
//    
//    return (int)msg.wParam;
//}

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
    UNREFERENCED_PARAMETER(hInst);

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        L"DirectX11", NULL
    };
    RegisterClassEx(&wc);

    // Create the application's window
    hWnd = CreateWindow(L"DirectX11", L"DX11 Engine",
        WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D    
    if (SUCCEEDED(game.InitD3D(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT)))
    {
        // Initialize game resources
        game.Init();
        
        // Show the window
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // Enter the message loop
        MSG msg;
        while (1)
        {
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                game.Update();
                game.Render();
            }
        }
    }

    UnregisterClass(L"DirectX11", wc.hInstance);
    return 0;
}