#pragma once
#include "Core.h"
#include "RTTI.h"

class Level;
class Engine_API Actor : public RTTI
{
	friend class Level;
	RTTI_DECLARATIONS(Actor, RTTI)
public:
	Actor();
	virtual ~Actor();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender()=0;
	void Destroy();

	// BeginPlay ȣ�� ���� Ȯ��
	inline bool HasBegunPlay() const { return hasBegunPlay; }

	// ���ʽ� ����
	void SetOwner(Level* newOwner) { owner = newOwner; }
	Level* GetOwner() { return owner; }

protected:
	// ���Ͱ� Ȱ�� �������� �˷��ִ� ����
	bool isActive = true;

	// ���� ��û�ƴ��� �˷��ִ� ����
	bool isExpired = false;

	// ���� ����(���ʽ�)
	Level* owner = nullptr;

	// BeginPlay ȣ���� �Ǿ����� Ȯ��
	bool hasBegunPlay = false;
};