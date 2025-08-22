#pragma once

#include "Actor/Actor.h"

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)
public:
	Player();
	~Player();

    virtual void OnInit();
    virtual void OnRender();

private:
    // DirectX 렌더링 리소스
    //ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* vertexBuffer;
    ID3D11VertexShader* vertexShader;    // 정점 셰이더 객체.
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;      // 포인터 정보 전달.

    // 정점 셰이더 컴파일 후 정보 (c++의 obj와 같음).
    ID3DBlob* vertexShaderBuffer = nullptr;

    //ID3D11PixelShader* pixelShader = nullptr;
    ID3DBlob* pixelShaderBuffer = nullptr;
    
    // 렌더링에 필요한 DirectX 컨텍스트
    ID3D11Device* d3dDevice;
    ID3D11DeviceContext* immediateContext;
};