#pragma once
#include "Core.h"
#include "Input.h"


class Engine_API Engine
{
public:
    Engine();
    virtual ~Engine();

    HRESULT InitD3D(HWND hWnd, int width, int height);

    static Engine& Get();

    bool IsInitialized() const { return m_isInitialized; }

protected:
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Release() = 0;

protected:
    // =======================================================
    // DX11 �ʼ� ��ü
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pRenderTargetView;

    // DepthStencil (3D�� �ʼ�)
    ID3D11DepthStencilView* m_pDepthStencilView;

    // ���� ��ü
    ID3D11RasterizerState* m_pRasterState;
    ID3D11BlendState* m_pBlendState;

    // ����Ʈ
    D3D11_VIEWPORT            m_viewport;
    D3D_FEATURE_LEVEL         m_featureLevel;

    // â ����
    HWND                      m_hWnd;
    int                       m_width;
    int                       m_height;

    bool                      m_isInitialized;

protected:
    Input input;

private:
    static Engine* instance;
};
