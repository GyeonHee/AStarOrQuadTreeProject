#pragma once

#include "Actor/Actor.h"
#include "../Common/Vertex.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)
public:
	Player();
	~Player();

    virtual void OnInit();
    virtual void OnRender();

private:
    // DirectX ������ ���ҽ�
    //ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* vertexBuffer;
    ID3D11VertexShader* vertexShader;    // ���� ���̴� ��ü.
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;      // ������ ���� ����.

    // ���� ���̴� ������ �� ���� (c++�� obj�� ����).
    ID3DBlob* vertexShaderBuffer = nullptr;

    //ID3D11PixelShader* pixelShader = nullptr;
    ID3DBlob* pixelShaderBuffer = nullptr;
    
    // �������� �ʿ��� DirectX ���ؽ�Ʈ
    ID3D11Device* d3dDevice;
    ID3D11DeviceContext* immediateContext;
};