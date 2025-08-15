#pragma once
#include "Core.h"
//#include <Windows.h>
//#include <d3d11.h>
//#include <dxgi.h>

class Engine_API Engine
{
public:
    Engine()
        : m_pd3dDevice(nullptr),
        m_pImmediateContext(nullptr),
        m_pSwapChain(nullptr),
        m_pRenderTargetView(nullptr),
        m_pDepthStencilBuffer(nullptr),
        m_pDepthStencilView(nullptr),
        m_pRasterState(nullptr),
        m_pBlendState(nullptr),
        m_hWnd(nullptr),
        m_width(0),
        m_height(0),
        m_featureLevel(D3D_FEATURE_LEVEL_11_0)
    {
    }
    virtual ~Engine() {}

    // === �ݵ�� .cpp�� �ñ״�ó ���� ===
    HRESULT InitD3D(HWND hWnd, int width, int height);
    void    CleanUp();
    void    RenderFrame();
    void    TickFrame();

protected:
    // �����(�Ļ� Ŭ����) ��
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Release() = 0;

protected:
    // �⺻ DX11 ���ҽ�
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pRenderTargetView;

    // ����/���ٽ�
    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11DepthStencilView* m_pDepthStencilView;

    // ����
    ID3D11RasterizerState* m_pRasterState;
    ID3D11BlendState* m_pBlendState;

    // ��Ÿ
    D3D11_VIEWPORT           m_viewport;
    D3D_FEATURE_LEVEL        m_featureLevel;

    HWND                     m_hWnd;
    int                      m_width;
    int                      m_height;
};
