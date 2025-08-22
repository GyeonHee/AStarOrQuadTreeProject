#include "Player.h"
#include "Engine.h"
#include "Math/Vector3.h"

Player::Player()
    //: vertexBuffer(nullptr),
    //vertexShader(nullptr),
    //pixelShader(nullptr),
    //inputLayout(nullptr),
    //d3dDevice(nullptr),
    //immediateContext(nullptr)
{
}

Player::~Player()
{

}

void Player::OnInit()
{
    super::OnInit();
    ID3D11Device* device = Engine::Get().GetDevice();
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

    hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(),
        nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);
    hr = device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &inputLayout);

    pVSBlob->Release();

    ID3DBlob* pPSBlob = nullptr;
    hr = D3DCompile(pixelShaderSource, strlen(pixelShaderSource), nullptr, nullptr, nullptr,
        "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob) pErrorBlob->Release();
        return;
    }

    hr = device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(),
        nullptr, &pixelShader);

    pPSBlob->Release();

    Vertex vertices[] =
    {
        { 0.0f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },  // Top - Red
        { 0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },  // Bottom Right - Green
        {-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }   // Bottom Left - Blue
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;

    hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
}

void Player::OnRender()
{
    super::OnRender();
 
    //// 그리기.
    //// 입력 (정점 배열(버퍼), 인덱스 배열(버퍼), 입력 레이아웃).
    //static unsigned int stride = Vector3::Stride() * 2;
    //static unsigned int offset = 0;
    //Engine::Get().GetContext()->IASetVertexBuffers(
    //    0, 1, &vertexBuffer, &stride, &offset
    //);

    //// 정점 구조 넘겨주기.
    //Engine::Get().GetContext()->IASetInputLayout(inputLayout);

    //// 삼각형으로 그리기.
    //Engine::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //// 셰이더 설정.
    //Engine::Get().GetContext()->VSSetShader(vertexShader, nullptr, 0);
    //Engine::Get().GetContext()->PSSetShader(pixelShader, nullptr, 0);

    //// 그리기.
    //Engine::Get().GetContext()->Draw(3, 0);

    ID3D11DeviceContext* context = Engine::Get().GetContext();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetInputLayout(inputLayout);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);

    context->Draw(3, 0);
}


