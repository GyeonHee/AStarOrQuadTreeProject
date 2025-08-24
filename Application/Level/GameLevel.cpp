#include "GameLevel.h"
#include "Actor/Player.h"
#include "Engine.h"

GameLevel::GameLevel()
{

}

GameLevel::~GameLevel()
{
	if (gridVertexBuffer) gridVertexBuffer->Release();
	if (gridVertexShader) gridVertexShader->Release();
	if (gridPixelShader) gridPixelShader->Release();
	if (gridInputLayout) gridInputLayout->Release();
	if (gridVertexShaderBuffer) gridVertexShaderBuffer->Release();
	if (gridPixelShaderBuffer) gridPixelShaderBuffer->Release();
}

void GameLevel::OnInit()
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

    ID3DBlob* pErrorBlob = nullptr;

    HRESULT hr = D3DCompile(vertexShaderSource, strlen(vertexShaderSource), nullptr, nullptr, nullptr,
        "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &gridVertexShaderBuffer, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob) pErrorBlob->Release();
        return;
    }

    hr = device->CreateVertexShader(gridVertexShaderBuffer->GetBufferPointer(), gridVertexShaderBuffer->GetBufferSize(),
        nullptr, &gridVertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);
    hr = device->CreateInputLayout(layout, numElements, gridVertexShaderBuffer->GetBufferPointer(),
        gridVertexShaderBuffer->GetBufferSize(), &gridInputLayout);

    hr = D3DCompile(pixelShaderSource, strlen(pixelShaderSource), nullptr, nullptr, nullptr,
        "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &gridPixelShaderBuffer, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob) pErrorBlob->Release();
        return;
    }

    hr = device->CreatePixelShader(gridPixelShaderBuffer->GetBufferPointer(), gridPixelShaderBuffer->GetBufferSize(),
        nullptr, &gridPixelShader);

    // 그리드 라인 정점 생성
    std::vector<Vertex> gridVertices;
    
    // 세로선들 (X축 방향)
    for (int i = -gridSize; i <= gridSize; i++)
    {
        float x = i * gridSpacing;
        Vertex v1 = {x, -gridSize * gridSpacing, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f}; // 시작점
        Vertex v2 = {x, gridSize * gridSpacing, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};  // 끝점
        gridVertices.push_back(v1);
        gridVertices.push_back(v2);
    }
    
    // 가로선들 (Y축 방향)
    for (int i = -gridSize; i <= gridSize; i++)
    {
        float y = i * gridSpacing;
        Vertex v1 = {-gridSize * gridSpacing, y, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f}; // 시작점
        Vertex v2 = {gridSize * gridSpacing, y, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};  // 끝점
        gridVertices.push_back(v1);
        gridVertices.push_back(v2);
    }

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * gridVertices.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = gridVertices.data();

    hr = device->CreateBuffer(&bd, &InitData, &gridVertexBuffer);
}

void GameLevel::OnUpdate()
{
	super::OnUpdate();
}

void GameLevel::OnRender()
{
	super::OnRender();
	
	ID3D11DeviceContext* context = Engine::Get().GetContext();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &gridVertexBuffer, &stride, &offset);
    context->IASetInputLayout(gridInputLayout);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    context->VSSetShader(gridVertexShader, nullptr, 0);
    context->PSSetShader(gridPixelShader, nullptr, 0);

    // 그리드 라인 수 계산 (세로선 + 가로선)
    int lineCount = (gridSize * 2 + 1) * 2;
    context->Draw(lineCount * 2, 0);  // 각 라인당 2개의 정점
}
	