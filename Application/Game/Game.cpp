#include "Game.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

Game::Game()
    : m_pVertexBuffer(nullptr), m_pCB(nullptr),
    m_pInputLayout(nullptr), m_pVS(nullptr), m_pPS(nullptr)
{
}

Game::~Game() {}

void Game::Init()
{
    using namespace DirectX;

    // 2D 삼각형 정점
    Vertex vertices[] =
    {
        {{ 0.0f, 0.5f, 0.0f }, {1,0,0,1}}, // 빨강
        {{ 0.5f,-0.5f, 0.0f }, {0,1,0,1}}, // 초록
        {{-0.5f,-0.5f, 0.0f }, {0,0,1,1}}  // 파랑
    };

    // 정점 버퍼
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;
    m_pd3dDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

    // 상수버퍼 (월드/뷰/프로젝션)
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(XMMATRIX) * 3;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pCB);

    // 셰이더 코드
    const char* vsCode =
        "cbuffer cbPerObject : register(b0) { matrix world; matrix view; matrix proj; };"
        "struct VS_INPUT { float3 pos : POSITION; float4 color : COLOR; };"
        "struct PS_INPUT { float4 pos : SV_POSITION; float4 color : COLOR; };"
        "PS_INPUT VSMain(VS_INPUT input) { PS_INPUT output;"
        "output.pos = float4(input.pos.xy, 0.0f, 1.0f);"
        "output.color = input.color;"
        "return output; }";

    const char* psCode =
        "struct PS_INPUT { float4 pos : SV_POSITION; float4 color : COLOR; };"
        "float4 PSMain(PS_INPUT input) : SV_TARGET { return input.color; }";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    D3DCompile(vsCode, strlen(vsCode), nullptr, nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vsBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_pVS);

    D3DCompile(psCode, strlen(psCode), nullptr, nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &psBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pPS);

    // Input Layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_pInputLayout);
    vsBlob->Release();
    psBlob->Release();
}

void Game::Update()
{
    // 2D 삼각형은 고정
}

void Game::Render()
{
    // Clear
    float clearColor[4] = { 0.1f,0.2f,0.4f,1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

    // 셰이더 설정
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);
    m_pImmediateContext->VSSetShader(m_pVS, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPS, nullptr, 0);

    // 정점 버퍼
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw
    m_pImmediateContext->Draw(3, 0);
    m_pSwapChain->Present(1, 0);
}

void Game::Release()
{
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pInputLayout) m_pInputLayout->Release();
    if (m_pVS) m_pVS->Release();
    if (m_pPS) m_pPS->Release();
    if (m_pCB) m_pCB->Release();
}
