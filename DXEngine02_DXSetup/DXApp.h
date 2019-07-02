#pragma once


#include"AppWindow.h"
#include"DXUtil.h"

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	virtual int Run() = 0;
	virtual bool Init();
	virtual void Update() = 0;
	virtual void Render() = 0;


protected:
	AppWindow* window;
	ID3D11Device* device;//생성시에는 장치에서
	ID3D11DeviceContext* deviceContext; //연결시에는 컨텍스트에서
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;

protected:
	bool InitializeDirect3D();
};

