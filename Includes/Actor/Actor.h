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

private:
	virtual void Init();
	virtual void Update();
	virtual void Render();

	void SetLevel(Level* level);
	Level* GetLevel() const;

	void Destroy();

	// BeginPlay ȣ�� ���� Ȯ��
	inline bool HasBegunPlay() const { return hasBegunPlay; }

protected:
    // ���ʽ� ����
	Level* m_level = nullptr;

	// ���Ͱ� Ȱ�� �������� �˷��ִ� ����
	bool isActive = true;

	// ���� ��û�ƴ��� �˷��ִ� ����
	bool isExpired = false;

	// BeginPlay ȣ���� �Ǿ����� Ȯ��
	bool hasBegunPlay = false;
};