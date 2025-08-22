#include "Actor.h"
#include "Level/Level.h"

Actor::Actor()
{

}
Actor::~Actor()
{

}

void Actor::OnInit()
{
	hasBegunPlay = true;
}

void Actor::OnUpdate()
{

}

void Actor::OnRender()
{

}

void Actor::Destroy()
{

	// 삭제 요청 되었다고 설정
	isExpired = true;

	owner->DestroyActor(this);
}