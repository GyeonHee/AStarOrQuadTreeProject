#include "Game.h"
#include <d3dcompiler.h>

Game::Game()
    : m_pVertexBuffer(nullptr),
    m_pVertexShader(nullptr),
    m_pPixelShader(nullptr),
    m_pInputLayout(nullptr)
{
}

Game::~Game() 
{
    Release();
}

void Game::Init()
{
    const char* vertexShaderSource = R"(
struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}
)";

    const char* pixelShaderSource = R"(
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
)";

    ID3DBlob* pVSBlob = nullptr;
    ID3DBlob* pErrorBlob = nullptr;
    
    HRESULT hr = D3DCompile(vertexShaderSource, strlen(vertexShaderSource), nullptr, nullptr, nullptr,
        "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
    
    if (FAILED(hr))
    {
        if (pErrorBlob) pErrorBlob->Release();
        return;
    }

    hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(),
        nullptr, &m_pVertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    
    UINT numElements = ARRAYSIZE(layout);
    hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &m_pInputLayout);
    
    pVSBlob->Release();

    ID3DBlob* pPSBlob = nullptr;
    hr = D3DCompile(pixelShaderSource, strlen(pixelShaderSource), nullptr, nullptr, nullptr,
        "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);
    
    if (FAILED(hr))
    {
        if (pErrorBlob) pErrorBlob->Release();
        return;
    }

    hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(),
        nullptr, &m_pPixelShader);
    
    pPSBlob->Release();

    Vertex vertices[] =
    {
        { 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },  // Top - Red
        { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },  // Bottom Right - Green
        {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }   // Bottom Left - Blue
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;

    hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
}

void Game::Update()
{
}

void Game::Render()
{
    float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);

    m_pImmediateContext->Draw(3, 0);

    m_pSwapChain->Present(0, 0);
}

void Game::Release()
{
    if (m_pVertexBuffer) { m_pVertexBuffer->Release(); m_pVertexBuffer = nullptr; }
    if (m_pVertexShader) { m_pVertexShader->Release(); m_pVertexShader = nullptr; }
    if (m_pPixelShader) { m_pPixelShader->Release(); m_pPixelShader = nullptr; }
    if (m_pInputLayout) { m_pInputLayout->Release(); m_pInputLayout = nullptr; }
}