#include "DXApp.h"



DXApp::DXApp(HINSTANCE hinstance)
{
	//윈도우 객체 생성.
	window = new AppWindow(hinstance);


}


DXApp::~DXApp()
{
	//delete window;
	Memory::SafeDelete(window);

	//DX 리소스 해제
	Memory::SafeRelease(device);
	Memory::SafeRelease(deviceContext);
	Memory::SafeRelease(swapChain);
	Memory::SafeRelease(renderTargetView);
}

bool DXApp::Init()
{

	//창 초기화
	if (window->initializeWindow() == false)
	{
		return false;
	}

	//DX 초기화
	if (InitializeDirect3D() == false)
	{
		return false;
	}

	return true;
}

bool DXApp::InitializeDirect3D()
{
	//스왑체인 생성 -Back/Front 버퍼 교환용
	//서술자(Descriptor)
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;//백버퍼를 하나만 두기, 2이상 설정가능
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = window->GetWindowHandle();
	swapDesc.Windowed = true; //창모드 지원
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//실제로 그림을 그릴 조건 정보 입력
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.Width = window->GetScreenWidth();
	swapDesc.BufferDesc.Height = window->GetScreenHeight();

	//퀄리티를 함부로 쓰겠다고 하면 안됨, 조건마다, 원하는 효과마다 필요한 수치가 있음
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	//첫번째 어답터는 그래픽카드를 의미함
	//스왑체인 & 장치 & 장치문맥 생성
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		NULL, 
		NULL/*D3D_FEATURE_LEVEL_11_0*/, 
		NULL, D3D11_SDK_VERSION, 
		&swapDesc, 
		&swapChain, 
		&device, 
		NULL, 
		&deviceContext);


	//오류 확인.
	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("장치 생성 실패"), TEXT("오류"), MB_OK);
		return false;
	}

	//백버퍼(텍스쳐) 생성.
	ID3D11Texture2D* backBufferTexture;
	result = swapChain->GetBuffer(
		NULL, 
		__uuidof(ID3D11Texture2D), 
		(void**)&backBufferTexture);

	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("백버퍼 생성 실패"), TEXT("오류"), MB_OK);
		return false;
	}

	//렌더 타겟 뷰 생성.
	result = device->CreateRenderTargetView(backBufferTexture, NULL, &renderTargetView);

	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("렌더 타겟 생성 실패"), TEXT("오류"), MB_OK);
		return false;
	}

	//렌더 타겟 연결. 
	deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

	//버퍼 생성을 위해 사용했던 자원 해제
	Memory::SafeRelease(backBufferTexture);


	return true;
}
