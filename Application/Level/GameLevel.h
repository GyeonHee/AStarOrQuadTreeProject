#pragma once
#include "Level/Level.h"

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)
public:
	GameLevel();
	~GameLevel();

	// ���� �̺�Ʈ �Լ�
	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
private:
};