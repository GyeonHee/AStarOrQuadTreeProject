#include "Game.h"
#include "Actor/Player.h"

Game::Game()
{
    mainLevel = new Level();
    Player* player = new Player();
    mainLevel->AddActor(player);
}

Game::~Game()
{
    delete mainLevel;
}

void Game::Init()
{
    if (mainLevel)
    {
        mainLevel->ProcessAddAndDestroyActors();
        mainLevel->OnInit();
    }
}

void Game::Run()
{
    LARGE_INTEGER frequency = {};
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER currentTime = {};
    QueryPerformanceCounter(&currentTime);
    LARGE_INTEGER previousTime = currentTime;

    float targetFrameTime = 120.0f;
    float oneFrameTime = 1.0f / targetFrameTime;

    MSG message = {};
    while (message.message != WM_QUIT)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            QueryPerformanceCounter(&currentTime);

            float deltaTime =
                static_cast<float>(currentTime.QuadPart - previousTime.QuadPart)
                / static_cast<float>(frequency.QuadPart);

            if (deltaTime >= oneFrameTime)
            {
                // FPS 표시
                char buffer[256] = {};
                sprintf_s(buffer, 256, "DirectX_11 | FPS: %d", static_cast<int>(ceil(1.0f / deltaTime)));
                SetWindowTextA(window->handle, buffer);  // window 접근을 위해 protected로 변경 필요

                // Game만의 렌더링 로직
                GameRenderFrame();

                // Level 업데이트
                if (mainLevel)
                {
                    mainLevel->ProcessAddAndDestroyActors();
                }

                previousTime = currentTime;
            }
        }
    }
}

void Game::GameRenderFrame()
{
    // 1. 화면 지우기
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GetContext()->ClearRenderTargetView(GetRenderTargetView(), clearColor);
    GetContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  // 추가
    // DepthStencil 접근을 위해 Engine의 멤버를 protected로 변경 필요

    // 2. Level 렌더링
    if (mainLevel)
    {
        mainLevel->OnRender();
    }

    // 3. 최종 출력 - swapChain 접근을 위해 protected 필요
    swapChain->Present(0, 0);
}