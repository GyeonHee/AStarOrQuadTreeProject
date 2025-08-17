#pragma once
#include "Engine.h"

class Game : public Engine
{
public:
    Game();
    ~Game();

    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void Release() override;
};
