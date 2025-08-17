	// BeginPlay 호출 여부 확인
	inline bool HasInit() const { return hasInit; }

	// 오너십 설정 (임시 주석 처리)
	// void SetOwner(Level* newOwner);
	// Level* GetOwner();

protected:
	// 월드 행렬 계산
	DirectX::XMMATRIX CalculateWorldMatrix();

	// 변환 속성
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;

	// 렌더링 관련
	//ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;
    ID3D11Buffer* m_pCB;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11VertexShader* m_pVS;
    ID3D11PixelShader* m_pPS;

    // 색상 속성
    DirectX::XMFLOAT4 m_color;

	// D3D11 디바이스 접근을 위한 포인터들
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	int m_width;
	int m_height;

	// Init 호출이 되었는지 확인
	bool hasInit = false;

	// 액터가 활성 상태인지 알려주는 변수
	//bool isActive = true;
	bool isActive = true;

	// 삭제 요청됐는지 알려주는 변수
	//bool isExpired = false;
	bool isExpired = false;

	// 소유 레벨(오너십) (임시 주석 처리)
	// Level* owner = nullptr;
