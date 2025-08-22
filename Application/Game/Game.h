#pragma once
#include "Engine.h"
#include "Level/Level.h"


class Game : public Engine
{
public:
    Game();
    ~Game();

    Level* GetMainLevel() const { return mainLevel; }

    void Init();
    virtual void Run() override;
    void GameRenderFrame();

private:
    Level* mainLevel;
};
