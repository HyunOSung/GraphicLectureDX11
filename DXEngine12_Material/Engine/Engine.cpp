#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{
	Memory::SafeDelete(mesh);
	Memory::SafeDelete(mesh2);
	
	material->Release();
	material2->Release();

	Memory::SafeRelease(material);	
	Memory::SafeRelease(material2);

	Memory::SafeRelease(constantBuffer);

	//vertexShader->Release();
	//Memory::SafeDelete(vertexShader);

	//pixelShader->Release();
	//Memory::SafeDelete(pixelShader);
}

int Engine::Run()
{
	return window->Run(this);
}

bool Engine::Init()
{
	if (DXApp::Init() == false)
		return false;

	// ��� �ʱ�ȭ.
	if (InitializeScene() == false)
		return false;

	// ��� �ʱ�ȭ.
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update()
{
	//// ���� ��� ���ε�.
	//mesh->Update(deviceContext);
	//mesh2->Update(deviceContext);

	//

	//// ��/���� ��� ���ε�.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render()
{
	float color[] = { 0.0f, 0.5f, 0.5f, 1.0f };

	// ���� Ÿ���� ������ �������� ĥ�ϱ�.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// ����/���ٽ� �� �����.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ���� ��� ���ε�.
	mesh->Update(deviceContext);
	material->BindeShaders(deviceContext);
	material->BindeTextures(deviceContext);
	material->BindSamplerState(deviceContext);
	mesh->RenderBuffers(deviceContext);


	mesh2->Update(deviceContext);
	material2->BindeShaders(deviceContext);
	material2->BindeTextures(deviceContext);
	material2->BindSamplerState(deviceContext);
	mesh2->RenderBuffers(deviceContext);
	// ��/���� ��� ���ε�.
	//deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);


	// ���̴� ���ε�.
	//vertexShader->BindShader(deviceContext);
	//pixelShader->BindShader(deviceContext);
	
	

	// �ؽ�ó/���÷� ������Ʈ ���ε�.
	//pixelShader->BindTexture(deviceContext);
	//pixelShader->BindSamplerState(deviceContext);
	
	
	
	
	


	// �޽� ���� �׸���.
	

	


	// ����� <-> ����Ʈ ���� ��ȯ.
	swapChain->Present(1, 0);
}

bool Engine::InitializeScene()
{
	//// ���� ���̴� ����.
	//vertexShader = new VertexShader(TEXT("Shader//ToonVS.fx"));
	//// ���� ���̴� ������.
	//if (vertexShader->CompileShader(device) == false)
	//	return false;

	//// ���� ���̴� ��ü ����.
	//if (vertexShader->CreateShader(device) == false)
	//	return false;

	//// �ȼ� ���̴� ����.
	//pixelShader = new PixelShader(TEXT("Shader//ToonPS.fx"));

	//// �ȼ� ���̴� ������.
	//if (pixelShader->CompileShader(device) == false)
	//	return false;

	//// �ȼ� ���̴� ��ü ����.
	//if (pixelShader->CreateShader(device) == false)
	//	return false;

	////// �ؽ�ó �ε�.
	//if (pixelShader->LoadTexture(
	//	device,
	//	TEXT("Resources/Textures/earth.jpg"))
	//	== false)
	//{
	//	return false;
	//}

	//// ���÷� ������Ʈ ����.
	//if (pixelShader->CreateSamplerState(device) == false)
	//	return false;

	//��Ƽ���� ��ü ����
	material = new Material(TEXT("Shader//Toon"));
	material2 = new Material(TEXT("Shader//Toon"));

	//��Ƽ���� ������
	if (material->CompileShaders(device) == false)
	{
		return false;
	}	
	
	if (material2->CompileShaders(device) == false)
	{
		return false;
	}

	//�� ���̴� ��ü ����
	if (material->CreateShaders(device) == false)
	{
		return false;
	}	
	
	if (material2->CreateShaders(device) == false)
	{
		return false;
	}

	//�ؽ��� ���� ó��
	//�ؽ��� �߰�
	material->AddTexture(TEXT("Resources/Textures/earth.jpg"));
	material2->AddTexture(TEXT("Resources/Textures/earth.jpg"));

	//�ؽ��� �ε�
	if (material->LoadTextures(device) == false)
	{
		return false;
	}	
	
	if (material2->LoadTextures(device) == false)
	{
		return false;
	}


	// �޽� ����.
	//mesh = new Mesh(0.0f, 0.0f, 0.0f);
	mesh = new Mesh("Resources/Models/Sphere.FBX");
	mesh->SetPosition(-70.0f, 0.0f, 0.0f);
	mesh->SetRotation(-90.0f, 0.0f, 0.0f);
	mesh->SetScale(1.0f, 1.0f, 1.0f);

	mesh2 = new Mesh("Resources/Models/Sphere.FBX");
	mesh2->SetPosition(70.0f, 0.0f, 0.0f);
	mesh2->SetRotation(-90.0f, 0.0f, 0.0f);
	mesh2->SetScale(0.5f, 0.5f, 0.5f);

	//// �ʱ�ȭ.
	//if (mesh->InitializeBuffers(device, vertexShader->GetShaderBuffer())
	//	== false)
	//	return false;	
	
	if (mesh->InitializeBuffers(device, material)
		== false)
		return false;	
	
	if (mesh2->InitializeBuffers(device, material)
		== false)
		return false;

	return true;
}

bool Engine::InitializeTransformation()
{
	// ī�޶� ���� ����.
	cameraPosition = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);
	cameraView = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camerUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �� ���.
	XMMATRIX view = XMMatrixLookAtLH(cameraPosition, cameraView, camerUpVector);

	// ���� ���.
	// �þ߰� / ��Ⱦ�� ����.
	float fovY = XMConvertToRadians(70.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	XMMATRIX projection = XMMatrixPerspectiveFovLH(fovY, aspectRatio, 1.0f, 10000.0f);

	// ���ۿ� ���� ����ü ���� ����.
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(view*projection);
	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	float x = XMVectorGetX(cameraPosition);
	float y = XMVectorGetY(cameraPosition);
	float z = XMVectorGetZ(cameraPosition);
	matrixData.worldCameraPosition = XMFLOAT3(x, y, z);

	// ���� ����.
	// ���� ������.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerSceneBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// �ε��� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// ��� ���� ����.
	HRESULT result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("�� ��� ���� ���� ����")))
	{
		return false;
	}

	return true;
}