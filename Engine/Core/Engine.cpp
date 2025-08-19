#include "Engine.h"

// 싱글톤 인스턴스
Engine* Engine::instance = nullptr;

Engine::Engine()
    : m_pd3dDevice(nullptr),
    m_pImmediateContext(nullptr),
    m_pSwapChain(nullptr),
    m_pRenderTargetView(nullptr),
    m_pDepthStencilView(nullptr),
    m_pRasterState(nullptr),
    m_pBlendState(nullptr),
    m_hWnd(nullptr),
    m_width(0),
    m_height(0),
    m_isInitialized(false)
{
    instance = this;
}

Engine::~Engine()
{
    Release();
}

Engine& Engine::Get()
{
    return *instance;
}

HRESULT Engine::InitD3D(HWND hWnd, int width, int height)
{
    m_hWnd = hWnd;
    m_width = width;
    m_height = height;

    // ------------------------------
    // SwapChain 설정
    // ------------------------------
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

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

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pd3dDevice,
        &m_featureLevel,
        &m_pImmediateContext);

    if (FAILED(hr)) return hr;

    // ------------------------------
    // BackBuffer → RenderTargetView 생성
    // ------------------------------
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr)) return hr;

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    // ------------------------------
    // DepthStencil Buffer + View 생성
    // ------------------------------
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthBuffer = nullptr;
    hr = m_pd3dDevice->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
    if (FAILED(hr)) return hr;

    hr = m_pd3dDevice->CreateDepthStencilView(depthBuffer, nullptr, &m_pDepthStencilView);
    depthBuffer->Release();
    if (FAILED(hr)) return hr;

    // RenderTarget + DepthStencil 바인딩
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // ------------------------------
    // Viewport 설정
    // ------------------------------
    m_viewport.Width = static_cast<FLOAT>(width);
    m_viewport.Height = static_cast<FLOAT>(height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;

    m_pImmediateContext->RSSetViewports(1, &m_viewport);

    // ------------------------------
    // Rasterizer State (기본값: CullBack)
    // ------------------------------
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthClipEnable = TRUE;

    hr = m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
    if (FAILED(hr)) return hr;
    m_pImmediateContext->RSSetState(m_pRasterState);

    // ------------------------------
    // Blend State (알파블렌딩 기본값)
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

    hr = m_pd3dDevice->CreateBlendState(&blendDesc, &m_pBlendState);
    if (FAILED(hr)) return hr;

    float blendFactor[4] = { 0.f,0.f,0.f,0.f };
    m_pImmediateContext->OMSetBlendState(m_pBlendState, blendFactor, 0xffffffff);

    // ------------------------------
    // 초기화 성공
    // ------------------------------
    m_isInitialized = true;
    return S_OK;
}

void Engine::Release()
{
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    if (m_pBlendState) { m_pBlendState->Release(); m_pBlendState = nullptr; }
    if (m_pRasterState) { m_pRasterState->Release(); m_pRasterState = nullptr; }
    if (m_pDepthStencilView) { m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr; }
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pImmediateContext) { m_pImmediateContext->Release(); m_pImmediateContext = nullptr; }
    if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = nullptr; }

    m_isInitialized = false;
}
