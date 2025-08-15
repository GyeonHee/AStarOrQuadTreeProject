// Win32 API�� ����ϱ� ���� include.
#include <Windows.h>
#include "Game/Game.h"

HWND hWnd;

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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // ������ Ŭ���� ���
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassEx(&wc);

    // ������ ����
    HWND hWnd = CreateWindow(L"MyWindowClass", L"DX11 Engine",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);

    Game game;
    if (FAILED(game.InitD3D(hWnd, 1280, 720)))
        return 0;

    // �޽��� ����
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
            game.TickFrame();    // Update ȣ��
            game.RenderFrame();  // Render + Present ȣ��
        }
    }
    game.CleanUp(); // ���ҽ� ����
    return (int)msg.wParam;
}