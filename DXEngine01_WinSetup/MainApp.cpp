#pragma once
#include "Engine.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	//wWinMain에서 w를 빼면 LPSTR로 바꿔줘야 함, 아니면 복사를 그대로 해주거나
	int nCmdShow)
{
	Engine engine(hInstance);

	if (engine.Init() == false)
	{
		return 0;
	}

	return  engine.Run();
}
