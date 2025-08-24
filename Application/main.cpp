#include <Windows.h>
#include "Game/Game.h"
//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow)
{
	//// 엔진 객체 생성.
	//Engine engine(L"DirectX_11", SCREEN_WIDTH, SCREEN_HEIGHT);
	//engine.Run();

	Game game;
	game.Init();
	game.Run();
	/*Engine engine(L"Wanted Demo Renderer", 800, 600);
	engine.Run();*/
	return 0;
}