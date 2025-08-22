#include "Engine.h"

#include "Level/Level.h"
#include "Actor/Actor.h"
#include "../Application/Game/Game.h"
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_NAME L"DirectX_11"

// 싱글톤 인스턴스 초기화
Engine* Engine::instance = nullptr;

// 윈도우 메시지 처리 콜백.
LRESULT CALLBACK MessageProcedure(
    HWND window,
    UINT message,
    WPARAM wparam,
    LPARAM lparam)
{
    // 일부 처리.
    switch (message)
    {
        // 창 삭제 메시지.
    case WM_DESTROY:
        // 프로그램 종료 이벤트 발행.
        PostQuitMessage(0);
        return 0;

    case WM_QUIT:
        DestroyWindow(window);
        return 0;

        // 키다운.
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            // 종료 메시지 박스 띄우기.
            if (MessageBoxA(nullptr, "Quit?", "Quit Program", MB_YESNO) == IDYES)
            {
                // Yes 버튼 누르면 창 제거.
                DestroyWindow(window);
                return 0;
            }
        }

        return 0;

        // 키업.
    }

    // 기본 처리.
    return DefWindowProc(window, message, wparam, lparam);
}

Engine::Engine(/*const wchar_t* windowName, unsigned int width, unsigned int height*/)
    : window(new Window(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, MessageProcedure))
{
    instance = this;

    // DX 리소스 생성.
    // 장치?(그래픽 카드) 생성.
    // 장치 3개 한번에 생성.
    // 장치 2개 생성하고 스왑체인 다음에 생성.

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    // ------------------------------
    // Swap Chain.
	// Back Buffer(화면 뒤에 있는 그림) <-> FrontBuffer(모니터에 보이는 그림).
    // ------------------------------
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;                                 
    sd.BufferDesc.Width = window->width;
    sd.BufferDesc.Height = window->height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window->handle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;          

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        &featureLevel,
        &context);

    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to D3D11CreateDeviceAndSwapChain", "Error", MB_OK);
        return;
    }

    // ------------------------------
    // BackBuffer �� RenderTargetView ����
    // ------------------------------
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to GetBuffer", "Error", MB_OK);
        return;
    }

    hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to CreateRenderTargetView", "Error", MB_OK);
        return;
    }

    // ------------------------------
    // DepthStencil Buffer + View ����
    // ------------------------------
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = SCREEN_WIDTH;
    depthDesc.Height = SCREEN_HEIGHT;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthBuffer = nullptr;
    hr = device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to CreateTexture2D", "Error", MB_OK);
        return;
    }

    hr = device->CreateDepthStencilView(depthBuffer, nullptr, &depthStencilView);
    depthBuffer->Release();
    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to CreateDepthStencilView", "Error", MB_OK);
        return;
    }

    // RenderTarget + DepthStencil ���ε� (�׸� ��ȭ�� ����)
    context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    // ------------------------------
   // Viewport ����
   // ------------------------------
    viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH);
    viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    context->RSSetViewports(1, &viewport);

    // ------------------------------
    // Rasterizer State (�⺻��: CullBack)
    // ------------------------------
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = TRUE;

    hr = device->CreateRasterizerState(&rasterDesc, &rasterState);
    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to CreateRasterizerState", "Error", MB_OK);
        return;
    }
    context->RSSetState(rasterState);

    // ------------------------------
    // Blend State (���ĺ����� �⺻��)
    // ------------------------------
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = device->CreateBlendState(&blendDesc, &blendState);
    if (FAILED(hr))
    {
        __debugbreak();
        MessageBoxA(nullptr, "Failed to CreateBlendState", "Error", MB_OK);
        return;
    }

    float blendFactor[4] = { 0.f,0.f,0.f,0.f };
    context->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

Engine::~Engine()
{
    // 창 삭제.
    SafeDelete(window);

    // 리소스.
    SafeRelease(device);
    SafeRelease(context);
    SafeRelease(swapChain);
    SafeRelease(renderTargetView);
    SafeRelease(depthStencilView);
    SafeRelease(depthStencilBuffer);
    SafeRelease(rasterState);
    SafeRelease(blendState);
}

Engine& Engine::Get()
{
    return *instance;
}

void Engine::Run()
{
    LARGE_INTEGER frequency = {};
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER currentTime = {};
    QueryPerformanceCounter(&currentTime);
    LARGE_INTEGER previousTime = currentTime;

    float targetFrameTime = 120.0f; // 120 FPS 목표.
    float oneFrameTime = 1.0f / targetFrameTime; // 한 프레임 시간 (초 단위).

    // 유지.
    // 메시지 확인 (게임 루프).
    MSG message = {};
    while (message.message != WM_QUIT)
    {
        // 메시지 확인 (창 메시지 확인 로직).
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            // 변환.
            TranslateMessage(&message);

            // 보내기.
            DispatchMessage(&message);
        }
        else
        {
            QueryPerformanceCounter(&currentTime);

            // Delta Time 계산.
            float deltaTime =
                static_cast<float>(currentTime.QuadPart - previousTime.QuadPart)
                / static_cast<float>(frequency.QuadPart);

            if (deltaTime >= oneFrameTime)
            {
                // FPS 타이틀 바에 표시.
                char buffer[256] = {};
                sprintf_s(buffer, 256, "DirectX_11 | FPS: %d", static_cast<int>(ceil(1.0f / deltaTime)));
                SetWindowTextA(window->handle, buffer);

                //// 렌더링.
                //// 레벨 그리기
                //if (mainLevel)
                //{
                //    mainLevel->OnRender();
                //}
                    RenderFrame();

                    // 이전 프레임에 추가 및 삭제 요청된 액터 처리
                    if (mainLevel)
                    {
                        mainLevel->ProcessAddAndDestroyActors();
                    }

                // 이전 시간 업데이트.
                previousTime = currentTime;
            }
        }
    }
}

void Engine::RenderFrame()
{
    // 1. 화면 지우기
    float clearColor[4] = { 0.8f, 0.6f, 0.8f, 1.0f };
    context->ClearRenderTargetView(renderTargetView, clearColor);
    context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 2. 레벨 그리기 
    Game* game = dynamic_cast<Game*>(this);
    if (game && game->GetMainLevel())
    {
        game->GetMainLevel()->OnRender();
    }

    // 3. 최종 출력
    swapChain->Present(0, 0);
}
