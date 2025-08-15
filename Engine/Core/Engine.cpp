#include "Engine.h"

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
        nullptr,                    // �⺻ �����
        D3D_DRIVER_TYPE_HARDWARE,   // �ϵ���� ������
        nullptr,                    // ����Ʈ���� ������ �Ⱦ�
        createDeviceFlags,
        nullptr, 0,                  // ��ó ���� ���� ����
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pd3dDevice,
        &m_featureLevel,
        &m_pImmediateContext);

    if (FAILED(hr))
        return hr;

    // ����� ��������
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    // ����Ÿ�� �� ����
    m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();

    // ���� ���ٽ� ���� ����
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
    m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);

    // ����Ÿ�� & ���� ���ٽ� ���ε�
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // ����Ʈ ����
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = (FLOAT)width;
    m_viewport.Height = (FLOAT)height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &m_viewport);

    Init(); // Game���� �������̵��� Init ȣ��

    return S_OK;
}

VOID Engine::CleanUp()
{
    Release(); // Game���� �������̵��� Release ȣ��

    if (m_pBlendState) m_pBlendState->Release();
    if (m_pRasterState) m_pRasterState->Release();
    if (m_pDepthStencilView) m_pDepthStencilView->Release();
    if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();
}

VOID Engine::RenderFrame()
{
    FLOAT clearColor[4] = { 0.1f, 0.2f, 0.4f, 1.0f }; // RGBA
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    Render(); // Game���� �������̵��� Render ȣ��

    m_pSwapChain->Present(1, 0);
}

void Engine::TickFrame()
{
    Update(); // Game���� �������̵��� Update ȣ��
}
