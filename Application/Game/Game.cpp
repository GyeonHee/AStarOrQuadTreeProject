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
                // FPS ǥ��
                char buffer[256] = {};
                sprintf_s(buffer, 256, "DirectX_11 | FPS: %d", static_cast<int>(ceil(1.0f / deltaTime)));
                SetWindowTextA(window->handle, buffer);  // window ������ ���� protected�� ���� �ʿ�

                // Game���� ������ ����
                GameRenderFrame();

                // Level ������Ʈ
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
    // 1. ȭ�� �����
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GetContext()->ClearRenderTargetView(GetRenderTargetView(), clearColor);
    GetContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  // �߰�
    // DepthStencil ������ ���� Engine�� ����� protected�� ���� �ʿ�

    // 2. Level ������
    if (mainLevel)
    {
        mainLevel->OnRender();
    }

    // 3. ���� ��� - swapChain ������ ���� protected �ʿ�
    swapChain->Present(0, 0);
}