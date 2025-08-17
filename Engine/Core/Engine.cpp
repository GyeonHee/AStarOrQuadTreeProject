#include "Engine.h"
#include "Input.h"
#include "Core.h"


// ���� ���� �ʱ�ȭ
Engine* Engine::instance = nullptr;

Engine& Engine::Get()
{
    return *instance;
}

Engine::Engine()
{
    instance = this;

    m_pd3dDevice = nullptr;
    m_pImmediateContext = nullptr;
    m_pSwapChain = nullptr;
    m_pRenderTargetView = nullptr;
    m_pDepthStencilBuffer = nullptr;
    m_pDepthStencilView = nullptr; 
    m_pRasterState = nullptr;
    m_pBlendState = nullptr;
    m_hWnd = nullptr;
    m_width = 0;
    m_height = 0;
    m_featureLevel = D3D_FEATURE_LEVEL_11_0;
    
    // InitD3D(m_hWnd, m_width, m_height); // 제거 - 생성자에서 호출하지 않음
}

Engine::~Engine()
{
    Release();
}

HRESULT Engine::InitD3D(HWND hWnd, int width, int height)
{
    m_hWnd = hWnd;
    m_width = width;
    m_height = height;

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    UINT createDeviceFlags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 기본 어댑터
        D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 가속
        nullptr,                    // 소프트웨어 렌더러 사용 안함
        createDeviceFlags,
        nullptr, 0,                  // 기능 레벨 배열 없음
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pd3dDevice,
        &m_featureLevel,
        &m_pImmediateContext);

    if (FAILED(hr))
        return hr;

    // 백버퍼 가져오기
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    // 렌더타겟 뷰 생성
    if (pBackBuffer != nullptr)
    {
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
        pBackBuffer->Release();
    }

    // 깊이 스텐실 버퍼 생성
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    m_pd3dDevice->CreateTexture2D(&depthDesc, nullptr, &m_pDepthStencilBuffer);
    if (m_pDepthStencilBuffer != nullptr)
    {
        m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
    }

    // 렌더타겟 & 깊이 스텐실 설정
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // 뷰포트 설정
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = (FLOAT)width;
    m_viewport.Height = (FLOAT)height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &m_viewport);

    // 초기화 완료 표시
    m_isInitialized = true;

    Init(); // Game에서 오버라이드할 Init 호출

    return S_OK;
}

void Engine::Run()
{
    // DirectX가 초기화되지 않았으면 게임 루프를 실행하지 않음
    if (!m_isInitialized)
    {
        return;
    }

    // 정적 변수로 타이머 정보 유지
    static LARGE_INTEGER previousTime = {0};
    static LARGE_INTEGER frequency = {0};
    static bool timerInitialized = false;

    // 타이머 초기화 (한 번만)
    if (!timerInitialized)
    {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&previousTime);
        timerInitialized = true;
    }

    // 현재 시간 측정
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    // 델타 타임 계산
    float deltaTime = (currentTime.QuadPart - previousTime.QuadPart) / (float)frequency.QuadPart;

    // 타겟 프레임레이트 설정 (60FPS)
    float targetFrameRate = 60.0f;
    float oneFrameTime = 1.0f / targetFrameRate;

    // 프레임 레이트 제한
    if (deltaTime >= oneFrameTime)
    {
        // 입력 처리
        input.ProcessInput();

        // 게임 업데이트 및 렌더링
        Update(deltaTime);
        Render();

        // 시간 업데이트
        previousTime = currentTime;

        // 이전 키 상태 저장
        input.SavePreviousKeyStates();
    }
}

void Engine::Release()
{
    if (m_pBlendState) m_pBlendState->Release();
    if (m_pRasterState) m_pRasterState->Release();
    if (m_pDepthStencilView) m_pDepthStencilView->Release();
    if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();

    // 포인터들을 nullptr로 리셋
    m_pBlendState = nullptr;
    m_pRasterState = nullptr;
    m_pDepthStencilView = nullptr;
    m_pDepthStencilBuffer = nullptr;
    m_pRenderTargetView = nullptr;
    m_pSwapChain = nullptr;
    m_pImmediateContext = nullptr;
    m_pd3dDevice = nullptr;

    // 초기화 상태 리셋
    m_isInitialized = false;
}

void Engine::Render()
{
    // DirectX가 초기화되지 않았으면 렌더링하지 않음
    if (!m_isInitialized || !m_pImmediateContext || !m_pRenderTargetView || !m_pSwapChain)
    {
        return;
    }

    FLOAT clearColor[4] = { 0.1f, 0.2f, 0.4f, 1.0f }; // RGBA
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
    
    if (m_pDepthStencilView)
    {
        m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    m_pSwapChain->Present(1, 0);
}   

void Engine::Update(float deltaTime)
{

}

void Engine::Quit()
{
    // ���� �÷��� ����
    isQuit = true;
}

void Engine::Init()
{
    
}
