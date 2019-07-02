#pragma once

#include<Windows.h>

class Engine; //이 곳에서 창을 띄움 
			  // 중복방지를 위해 전방선언


class AppWindow
{
public:
	AppWindow(HINSTANCE hinstance);
	~AppWindow();

	int Run(Engine* engine);
	bool initializeWindow();
	virtual LRESULT MessageProcessor(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	);

	//Getter, 받아오기 위한 접근처
	HWND GetWindowHandle() const { return hwnd; }
	UINT GetScreenWidth() const { return screenWidth; }
	UINT GetScreenHeight() const { return screenHeight; }
	LPCTSTR GetApplicationName() const { return applicationName; }


protected:
	//창 만들 때 필요한 값들
	HWND hwnd;					//윈도우 핸들 값
	HINSTANCE hinstance;		//창 인스턴스
	UINT screenWidth;			//화면 가로크기
	UINT screenHeight;			// 화면 세로크기
	LPCTSTR applicationName;	// 창 이름
	DWORD windowStyle;			// 창 모양

};

