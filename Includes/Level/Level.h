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

	// 추가 및 삭제 요청된 액터를 처리하는 함수
	void ProcessAddAndDestroyActors();

private:

protected:
	// 레벨에 배치된 모든 액터를 관리하는 배열
	std::vector<Actor*> actors;

	// 추가 요청된 액터를 관리하는 배열
	std::vector<Actor*> addRequestedActors;

	// 삭제 요청된 액터를 관리하는 배열
	std::vector<Actor*> destroyRequestedActors;

};
