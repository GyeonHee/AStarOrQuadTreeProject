#pragma once

#include "Core.h"
#include <vector>	// ũ�Ⱑ �˾Ƽ� �����Ǵ� ���� �迭
#include "RTTI.h"
#include "Math/Vector3.h"

class Actor;
class Engine_API Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)
public:
	Level();
	virtual ~Level();

	// ������ ���͸� �߰��� �� ���
	void AddActor(Actor* newActor);

	// ���� ���� �Լ�
	void DestroyActor(Actor* destroyedActor);

	// ���� �̺�Ʈ �Լ�
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();

	// �߰� �� ���� ��û�� ���͸� ó���ϴ� �Լ�
	void ProcessAddAndDestroyActors();

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;

	// �߰� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> addRequestedActors;

	// ���� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> destroyRequestedActors;
};