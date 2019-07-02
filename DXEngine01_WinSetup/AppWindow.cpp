#include "AppWindow.h"
#include"Engine.h"



AppWindow* app = NULL;

//윈도우 메시지 프로시져
LRESULT  CALLBACK WindowProcessor(
	HWND hwnd,
	UINT msg,
	WPARAM wParam, // 입력장치의 입력여부를 메세지로 전환할 수 있는 패러미터
	LPARAM lParam  // 여기도 가능
)
{
	if (app != NULL)
	{
		return app->MessageProcessor(hwnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


}

AppWindow::AppWindow(HINSTANCE hinstance)
{
	hwnd = NULL;
	this->hinstance = hinstance; //우리가 가지고 있는 값을 기록
	screenWidth = 1600;
	screenHeight = 900;
	applicationName = TEXT("Engine01 - Window Setup");
	windowStyle = WS_OVERLAPPEDWINDOW;
	app = this;

}


AppWindow::~AppWindow()
{
}

int AppWindow::Run(Engine * engine) //창이 실행중일 때 구동할 함수
{

	MSG msg;  //메시지 변수
	ZeroMemory(&msg, sizeof(msg)); //메모리 초기화

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//실제로 그림을 그리는 부분
			engine->Update();
			engine->Render();
		}
	}
	return 0;
}

bool AppWindow::initializeWindow()
{
	//창 시작
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// 창 구조체 값 설정
	wc.cbSize = sizeof(WNDCLASSEX); //창 만들때 메모리 크기를 전달해줘야 함
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;						//배경
	wc.lpszClassName = TEXT("DXEngine");			//창 클래스 이름, 다시 쓰임
	wc.lpfnWndProc = WindowProcessor;				//메시지 처리 함수


	//클래스 등록
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	//창 핸들 만들기.
	hwnd = CreateWindow(wc.lpszClassName,
		applicationName,
		windowStyle,
		0, 0,
		screenWidth, screenHeight,
		NULL, NULL, hinstance, NULL);

	//창 핸들 값이 NULL인 경우 실패 리턴
	if (hwnd == NULL)
	{
		return false;
	}

	//창 보여주기
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

LRESULT AppWindow::MessageProcessor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				if (MessageBox(NULL, TEXT("종료하시겠습니까?"), TEXT("종료"), MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					//예 버튼 눌리면 창 삭제
					DestroyWindow(this->hwnd);
				}
			}

		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;
	}



	return DefWindowProc(hwnd, msg, wParam, lParam);
}
