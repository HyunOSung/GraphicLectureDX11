#pragma once

#include<Windows.h>

class Engine; //�� ������ â�� ��� 
			  // �ߺ������� ���� ���漱��


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

	//Getter, �޾ƿ��� ���� ����ó
	HWND GetWindowHandle() const { return hwnd; }
	UINT GetScreenWidth() const { return screenWidth; }
	UINT GetScreenHeight() const { return screenHeight; }
	LPCTSTR GetApplicationName() const { return applicationName; }


protected:
	//â ���� �� �ʿ��� ����
	HWND hwnd;					//������ �ڵ� ��
	HINSTANCE hinstance;		//â �ν��Ͻ�
	UINT screenWidth;			//ȭ�� ����ũ��
	UINT screenHeight;			// ȭ�� ����ũ��
	LPCTSTR applicationName;	// â �̸�
	DWORD windowStyle;			// â ���

};

