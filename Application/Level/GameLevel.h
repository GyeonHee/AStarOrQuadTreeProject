#pragma once
#include "Level/Level.h"
#include "../Common/Vertex.h"
#include <vector>

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)
public:
	GameLevel();
	~GameLevel();

	// ���� �̺�Ʈ �Լ�
	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
private:
	// DirectX ������ ���ҽ�
	ID3D11Buffer* gridVertexBuffer = nullptr;
	ID3D11VertexShader* gridVertexShader = nullptr;
	ID3D11PixelShader* gridPixelShader = nullptr;
	ID3D11InputLayout* gridInputLayout = nullptr;

	// ���̴� ���� ����
	ID3DBlob* gridVertexShaderBuffer = nullptr;
	ID3DBlob* gridPixelShaderBuffer = nullptr;

	// �������� �ʿ��� DirectX ���ؽ�Ʈ
	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;

	// �׸��� ���� ����
	int gridSize = 10;
	float gridSpacing = 1.0f;
};