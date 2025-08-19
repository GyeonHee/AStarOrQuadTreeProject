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

	// BeginPlay 호출 여부 확인
	inline bool HasBegunPlay() const { return hasBegunPlay; }

protected:
    // 오너십 설정
	Level* m_level = nullptr;

	// 액터가 활성 상태인지 알려주는 변수
	bool isActive = true;

	// 삭제 요청됐는지 알려주는 변수
	bool isExpired = false;

	// BeginPlay 호출이 되었는지 확인
	bool hasBegunPlay = false;
};