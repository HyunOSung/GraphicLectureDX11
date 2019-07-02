#include "DXApp.h"



DXApp::DXApp(HINSTANCE hinstance)
{
	//������ ��ü ����.
	window = new AppWindow(hinstance);


}


DXApp::~DXApp()
{
	//delete window;
	Memory::SafeDelete(window);

	//DX ���ҽ� ����
	Memory::SafeRelease(device);
	Memory::SafeRelease(deviceContext);
	Memory::SafeRelease(swapChain);
	Memory::SafeRelease(renderTargetView);
}

bool DXApp::Init()
{

	//â �ʱ�ȭ
	if (window->initializeWindow() == false)
	{
		return false;
	}

	//DX �ʱ�ȭ
	if (InitializeDirect3D() == false)
	{
		return false;
	}

	return true;
}

bool DXApp::InitializeDirect3D()
{
	//����ü�� ���� -Back/Front ���� ��ȯ��
	//������(Descriptor)
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;//����۸� �ϳ��� �α�, 2�̻� ��������
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = window->GetWindowHandle();
	swapDesc.Windowed = true; //â��� ����
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//������ �׸��� �׸� ���� ���� �Է�
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.Width = window->GetScreenWidth();
	swapDesc.BufferDesc.Height = window->GetScreenHeight();

	//����Ƽ�� �Ժη� ���ڴٰ� �ϸ� �ȵ�, ���Ǹ���, ���ϴ� ȿ������ �ʿ��� ��ġ�� ����
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	//ù��° ����ʹ� �׷���ī�带 �ǹ���
	//����ü�� & ��ġ & ��ġ���� ����
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


	//���� Ȯ��.
	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("��ġ ���� ����"), TEXT("����"), MB_OK);
		return false;
	}

	//�����(�ؽ���) ����.
	ID3D11Texture2D* backBufferTexture;
	result = swapChain->GetBuffer(
		NULL, 
		__uuidof(ID3D11Texture2D), 
		(void**)&backBufferTexture);

	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("����� ���� ����"), TEXT("����"), MB_OK);
		return false;
	}

	//���� Ÿ�� �� ����.
	result = device->CreateRenderTargetView(backBufferTexture, NULL, &renderTargetView);

	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("���� Ÿ�� ���� ����"), TEXT("����"), MB_OK);
		return false;
	}

	//���� Ÿ�� ����. 
	deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

	//���� ������ ���� ����ߴ� �ڿ� ����
	Memory::SafeRelease(backBufferTexture);


	return true;
}
