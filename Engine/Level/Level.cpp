#include "Level.h"
#include "Actor/Actor.h"
//#include <iostream>

Level::Level()
{

}
Level::~Level()
{
	// null 확인 후 액터 제거
	for (Actor* actor : actors)
	{
		SafeDelete(actor);
	}

	for (Actor* actor : addRequestedActors)
	{
		SafeDelete(actor);
	}

	for (Actor* actor : destroyRequestedActors)
	{
		SafeDelete(actor);
	}

	// std::vector 정리
	actors.clear();
	addRequestedActors.clear();
	destroyRequestedActors.clear();
}

// 레벨에 액터를 추가할 때 사용
void Level::AddActor(Actor* newActor)
{
	// 예외처리(중복 여부 확인) 필요

	// push_back / emplace_back : 배열 맨 뒤에 새로운 항목 추가하는 함수

	// 대기 배열에 추가
	addRequestedActors.emplace_back(newActor);
}

void Level::DestroyActor(Actor* destroyedActor)
{
	// 중복 검사
	if (destroyRequestedActors.size() > 0)
	{
		for (const Actor* const actor : destroyRequestedActors)
		{
			if (actor == destroyedActor)
			{
				return;
			}
		}
	}
	// 대기 배열에 추가
	destroyRequestedActors.emplace_back(destroyedActor);
}

// 이벤트 함수

// 객체 생애주기(Lifetime)에 1번만 호출됨 (초기화 목적)
void Level::OnInit()
{
	for (Actor* const actor : actors)
	{
		if (actor == nullptr)
		{
			continue; // nullptr 체크
		}
		// 액터 처리 여부 확인
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		// 이미 호출된 개체는 건너뛰기
		if (actor->HasBegunPlay())
		{
			continue;
		}

		actor->OnInit();
	}
}

// 프레임 마다 호출 (반복성 작업 / 지속성이 필요한 작업)
void Level::OnUpdate()
{
	for (Actor* const actor : actors)
	{
		if (actor == nullptr)
		{
			continue; // nullptr 체크
		}
		// 액터 처리 여부 확인
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}
	}
}

// 그리기 함수
void Level::OnRender()
{
	// Render Pass
	for (Actor* const actor : actors)
	{
		// 액터 처리 여부 확인
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		// 드로우 콜
		actor->OnRender();
	}
}

void Level::ProcessAddAndDestroyActors()
{
	// actors 배열에서 제외 처리
	for (auto iterator = actors.begin(); iterator != actors.end();)
	{
		// 삭제 요청된 액터인지 확인 후 배열에서 제외
		if ((*iterator)->isExpired)
		{
			// erase 함수를 사용하면 iterator가 무효화되기 때문에
			// 반환되는 값을 저장해야함/
			iterator = actors.erase(iterator);
			continue;
		}

		++iterator;
	}

	// destroyedActors 배열을 순회하면서 액터 delete
	for (auto*& actor : destroyRequestedActors)
	{
		// 리소스 해제
		SafeDelete(actor);
	}

	// 배열 초기화
	destroyRequestedActors.clear();

	// addRequestedActors 배열을 순회하면서 새로운 액터 add
	for (auto* const actor : addRequestedActors)
	{
		actors.emplace_back(actor);
		actor->SetOwner(this);
	}

	addRequestedActors.clear();
}



