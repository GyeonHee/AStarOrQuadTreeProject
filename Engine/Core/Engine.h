#pragma once
#include "Core.h"
#include "Input.h"

class Engine_API Engine
{
public:
    Engine();
    virtual ~Engine();

    // ���� ���� �Լ�
    void Run();
    // ���� �Լ�
    void Quit();

    // === 반드시 .cpp에서 별도처리 해야함 ===
    HRESULT InitD3D(HWND hWnd, int width, int height);

    // 싱글톤 접근 함수
    static Engine& Get();

    // 초기화 상태 확인
    bool IsInitialized() const { return m_isInitialized; }

protected:
    // 가상(자식 클래스) 함수들
    virtual void Init();
    virtual void Update(float deltaTime = 0.0f);
    virtual void Render();
    virtual void Release();

protected:
    // 기본 DX11 객체들
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pRenderTargetView;

    // 깊이/스텐실
    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11DepthStencilView* m_pDepthStencilView;

    // 상태
    ID3D11RasterizerState* m_pRasterState;
    ID3D11BlendState* m_pBlendState;

    // 뷰포트
    D3D11_VIEWPORT           m_viewport;
    D3D_FEATURE_LEVEL        m_featureLevel;

    HWND                     m_hWnd;
    int                      m_width;
    int                      m_height;

    // 초기화 상태
    bool                     m_isInitialized = false;

protected:
    // 입력 관리자
    Input input;
    // 종료 플래그
    bool isQuit = false;

private:
    static Engine* instance;
};
