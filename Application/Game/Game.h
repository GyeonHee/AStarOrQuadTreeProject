#pragma once
#include "Engine.h"

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

class Game : public Engine
{
public:
    Game();
    ~Game();

	virtual void Init();
	virtual void Render();
	virtual void Update();
	virtual void Release();

private:
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11VertexShader* m_pVertexShader;
    ID3D11PixelShader* m_pPixelShader;
    ID3D11InputLayout* m_pInputLayout;
};
