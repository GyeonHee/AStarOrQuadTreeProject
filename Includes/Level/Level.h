#pragma once
#include "Core.h"
#include <vector>

class Actor;

class Engine_API Level
{
public:
    Level();
    virtual ~Level();

    virtual void Update(float deltaTime = 0.0f);
    virtual void Render();

    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);

protected:
    std::vector<Actor*> actors;
};
