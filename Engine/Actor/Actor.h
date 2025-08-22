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

	// BeginPlay 호출 여부 확인
	inline bool HasBegunPlay() const { return hasBegunPlay; }

	// 오너십 설정
	void SetOwner(Level* newOwner) { owner = newOwner; }
	Level* GetOwner() { return owner; }

protected:
	// 액터가 활성 상태인지 알려주는 변수
	bool isActive = true;

	// 삭제 요청됐는지 알려주는 변수
	bool isExpired = false;

	// 소유 레벨(오너십)
	Level* owner = nullptr;

	// BeginPlay 호출이 되었는지 확인
	bool hasBegunPlay = false;
};