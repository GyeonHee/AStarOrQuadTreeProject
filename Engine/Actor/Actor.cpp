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

	// ���� ��û �Ǿ��ٰ� ����
	isExpired = true;

	owner->DestroyActor(this);
}