#include "DXApp.h"



DXApp::DXApp(HINSTANCE hinstance)
{
	//윈도우 객체 생성.
	window = new AppWindow(hinstance);


}


DXApp::~DXApp()
{
	delete window;
}

bool DXApp::Init()
{
	if (window->initializeWindow() == false)
	{
		return false;
	}

	return true;
}
