#pragma once
#include "Core.h"
#include "Window.h"
#include "Input.h"


class Level;
class Engine_API Engine
{
public:
    Engine(/*const wchar_t* windowName, unsigned int width, unsigned int height*/);
    virtual ~Engine();

    static Engine& Get();

    //HRESULT InitD3D(HWND hWnd, int width, int height);
    virtual void Run();

    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetContext() { return context; }
    ID3D11RenderTargetView* GetRenderTargetView() { return renderTargetView; }
    ID3D11RenderTargetView* const* GetRefRenderTargetView() { return &renderTargetView; }
    HWND GetWindowHandle() { return window ? window->handle : nullptr; }
    ID3D11DepthStencilView* GetDepthStencilView() { return depthStencilView; }
    IDXGISwapChain* GetSwapChain() { return swapChain; }

private:
    void RenderFrame();

protected:
    static Engine* instance;
    Input input;
    Window* window = nullptr;

    // =======================================================

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;

    // DepthStencil (3D������ �ʼ�)
    ID3D11DepthStencilView* depthStencilView = nullptr;
    ID3D11Texture2D* depthStencilBuffer = nullptr;

    // ����Ʈ ����
    D3D11_VIEWPORT            viewport;
    D3D_FEATURE_LEVEL         featureLevel;

    ID3D11RasterizerState* rasterState = nullptr; // �������� ��� / �޸� ���� / ���� Ŭ���� ����
    ID3D11BlendState* blendState = nullptr;       // �ȼ� ���� �ռ�(����, ���� ����) ����

    // =======================================================

    // ���� ����
    Level* mainLevel = nullptr;
};
