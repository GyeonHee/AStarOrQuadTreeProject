#include "Game.h"

Game::Game()
{
}

Game::~Game() 
{
    Release();
}

void Game::Init()
{
    // 게임 초기화 로직
}

void Game::Update(float deltaTime)
{
    // ESC 키로 게임 종료
    if (input.GetKeyDown(VK_ESCAPE))
    {
        Quit();
        PostQuitMessage(0);
    }
    
    // 게임 업데이트 로직
}

void Game::Render()
{
    // 부모 클래스의 기본 렌더링 호출
    Engine::Render();
    
    // 추가 게임 렌더링 로직
}

void Game::Release()
{
    Engine::Release();
}