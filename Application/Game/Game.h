#pragma once
#include "../../Engine/Core/Engine.h"

struct Vertex
{
    DirectX::XMFLOAT3 pos;   // XY만 사용, Z는 0
    DirectX::XMFLOAT4 color; // RGBA
};

class Game : public Engine
{
public:
    Game();
    ~Game();

protected:
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;

private:
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pCB;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11VertexShader* m_pVS;
    ID3D11PixelShader* m_pPS;
};
