#pragma once
#include "Engine.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	//wWinMain���� w�� ���� LPSTR�� �ٲ���� ��, �ƴϸ� ���縦 �״�� ���ְų�
	int nCmdShow)
{
	Engine engine(hInstance);

	if (engine.Init() == false)
	{
		return 0;
	}

	return  engine.Run();
}
