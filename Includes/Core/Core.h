#pragma once
// dll �ܺη� ���ø� ������ �� �߻��ϴ� ��� ��Ȱ��ȭ
#pragma warning(disable: 4251)
// ���� ������ �ּҸ� ��ȯ�� �� �߻��ϴ� ��� ��Ȱ��ȭ
#pragma warning(disable: 4172)

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if BuildEngineDLL
#define Engine_API __declspec(dllexport)
#else
#define Engine_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define new new
#endif

// �޸� ���� �Լ�
template<typename T>
void SafeDelete(T*& target)
{
	if (target != nullptr)
	{
		delete target;
		target = nullptr;
	}
}

// �޸� ���� �Լ�
template<typename T>
void SafeDeleteArray(T*& target)
{
	if (target != nullptr)
	{
		delete[] target;
		target = nullptr;
	}
}