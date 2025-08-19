#pragma once

#include "Core.h"
#include "RTTI.h"
#include <vector>

class Actor;
class Engine_API Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)
public:
	Level();
	virtual ~Level();

	void AddActor(Actor* actor);

	void DestroyActor(Actor* actor);

	void Init();
	void Update();
	void Render();

	// �߰� �� ���� ��û�� ���͸� ó���ϴ� �Լ�
	void ProcessAddAndDestroyActors();

private:

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;

	// �߰� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> addRequestedActors;

	// ���� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> destroyRequestedActors;

};
