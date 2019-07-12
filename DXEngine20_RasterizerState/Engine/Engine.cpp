#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{
	//Memory::SafeDelete(mesh);
	//Memory::SafeDelete(mesh2);

	//material->Release();
	//material2->Release();

	//Memory::SafeRelease(material);
	//Memory::SafeRelease(material2);

	Memory::SafeDelete(camera);
	Memory::SafeDelete(input);

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

	// Ÿ�̸� �ʱ�ȭ
	InitializeTimer();

	// �Է� �ʱ�ȭ
	InitializeInput();

	// ��� �ʱ�ȭ.
	if (InitializeScene() == false)
		return false;

	// ��� �ʱ�ȭ.
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update(float deltaTime)
{
	//// ���� ��� ���ε�.
	//mesh->Update(deviceContext);
	//mesh2->Update(deviceContext);

	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
	//// ��/���� ��� ���ε�.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render(float deltaTime)
{
	float color[] = { 0.0f, 0.5f, 0.5f, 1.0f };

	// ���� Ÿ���� ������ �������� ĥ�ϱ�.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// ����/���ٽ� �� �����.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		(*mesh)->Update(deviceContext);
		(*mesh)->BindeShaders(deviceContext);
		(*mesh)->BindeTextures(deviceContext);
		(*mesh)->BindSamplerState(deviceContext);
		(*mesh)->BindRasterizerState(deviceContext);
		(*mesh)->RenderBuffers(deviceContext);
	}

	// ������� ����
	// 1. ���� ��� ���ε�.
	// 2. ��/���� ��� ���ε�.
	// 3. ���̴� ���ε�.
	// 4. �ؽ�ó/���÷� ������Ʈ ���ε�.
	// 5. �޽� ���� �׸���.

	// ���ʴ�� ����� ����� ���ư���. ������ ó���� �ȵǾ��µ� ó���� �Ϸ��� �ϸ� �������� ���� �ʴ´�.
	//mesh->Update(deviceContext);
	//mesh->BindeShaders(deviceContext);
	//mesh->BindeTextures(deviceContext);
	//mesh->BindSamplerState(deviceContext);
	//mesh->RenderBuffers(deviceContext);

	//mesh2->Update(deviceContext);
	//mesh2->BindeShaders(deviceContext);
	//mesh2->BindeTextures(deviceContext);
	//mesh2->BindSamplerState(deviceContext);
	//mesh2->RenderBuffers(deviceContext);



	//�����Ͷ����� ������Ʈ ���ε�, �� ���� �׸��� ���� ���ٰ�


	// ����� <-> ����Ʈ ���� ��ȯ.
	swapChain->Present(1, 0);
}

void Engine::ProcessInput(float deltaTime)
{
	//ESC ����ó��
	if (input->IsKeyDown(Keyboard::Keys::Escape))
	{

		if (MessageBox(NULL,
			TEXT("���� �Ͻðڽ��ϱ�?"),
			TEXT("����"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// �� ��ư ������ â ����.
			DestroyWindow(window->GetWindowHandle());
		}
		/*else
		{
			input->ResetKeyboardState();
		}
		*/
	}

	//ī�޶� �̵� ȸ��
	float moveSpeed = 200.0f;

	if (input->IsKeyDown(Keyboard::Keys::A) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveRight(-1.0f * 200.0f * deltaTime);
	}
	if (input->IsKeyDown(Keyboard::Keys::D) || input->IsKeyDown(Keyboard::Keys::Right))
	{
		camera->MoveRight(1.0f * 200.0f * deltaTime);

	}
	if (input->IsKeyDown(Keyboard::Keys::W) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveForward(1.0f * 200.0f * deltaTime);

	}
	if (input->IsKeyDown(Keyboard::Keys::S) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveForward(-1.0f * 200.0f * deltaTime);

	}
	if (input->IsKeyDown(Keyboard::Keys::Q) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveUp(1.0f * 200.0f * deltaTime);

	}
	if (input->IsKeyDown(Keyboard::Keys::E) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveUp(-1.0f * 200.0f * deltaTime);
	}

	float rotationSpeed = 15.0f;

	//���콺 ���� ��ư ���� ���� �̵�
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{
		camera->Yaw(state.x*rotationSpeed * deltaTime);
		camera->Pitch(state.y*rotationSpeed * deltaTime);
	}

}

bool Engine::InitializeScene()
{

	//FBX �̸� ����
	LPCSTR fbxTPP = "Resources/Models/HeroTPP.FBX";
	LPCSTR fbxBox = "Resources/Models/SK_CharM_Cardboard.FBX";
	LPCSTR fbxCube = "Resources/Models/cube.FBX";
	LPCSTR fbxSphere = "Resources/Models/sphere.FBX";
	LPCSTR fbxBarbarous = "Resources/Models/SK_CharM_Barbarous.FBX";
	LPCSTR fbxWarrior = "Resources/Models/SK_CharM_Warrior.FBX";
	LPCSTR fbxBear = "Resources/Models/Enemy_Bear.FBX";

	//�ؽ��� �̸� ����
	LPCTSTR tppDiffuseTexture = TEXT("Resources/Textures/T_Chr_FPS_D.png");
	LPCTSTR tppNormaTexture = TEXT("Resources/Textures/T_Chr_FPS_N.png");
	LPCTSTR boxDiffuseTexture = TEXT("Resources/Textures/Char_M_Cardboard_D.png");
	LPCTSTR boxNormalTexturel = TEXT("Resources/Textures/Char_M_Cardboard_N.png");
	LPCTSTR lightStepDiffuseTexture = TEXT("Resources/Textures/LightStep.png");
	LPCTSTR lightStepNormalTexture = TEXT("Resources/Textures/LightStep.png");
	LPCTSTR warpDiffuseTexture = TEXT("Resources/Textures/Warp.png");
	LPCTSTR warpNormalTexture = TEXT("Resources/Textures/Warp.png");
	LPCTSTR barbarousDiffuseTexture = TEXT("Resources/Textures/T_Barbarous_D.TGA");
	LPCTSTR barbarousNormalTexture = TEXT("Resources/Textures/T_Barbarous_N.TGA");
	LPCTSTR warriorDiffuseTexture = TEXT("Resources/Textures/T_CharM_Warrior_D.TGA");
	LPCTSTR warriorNormalTexture = TEXT("Resources/Textures/T_CharM_Warrior_N.TGA");
	LPCTSTR bearDiffuseTexture = TEXT("Resources/Textures/T_Enemy_Bear_D.TGA");
	LPCTSTR bearNormalTexture = TEXT("Resources/Textures/T_Enemy_Bear_N.TGA");
	
	//���̴� �̸� ����
	LPCTSTR warpDiffuseShader = TEXT("Shader/WarpDiffuse");
	LPCTSTR normalMappingShader = TEXT("Shader/NormalMapping2");
	LPCTSTR toon2Shader = TEXT("Shader/Toon2");


	// �޽� ����.
	
	Mesh* tppNormal = new Mesh(fbxTPP, normalMappingShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	tppNormal->SetPosition(70.0f, -90.0f, 0.0f);
	tppNormal->SetRotation(-90.0f, 180.0f, 0.0f);
	tppNormal->SetScale(1.0f, 1.0f, 1.0f);

	tppNormal->AddTexture(tppDiffuseTexture);
	tppNormal->AddTexture(tppNormaTexture);

	meshes.push_back(tppNormal);

	Mesh* tppWarp = new Mesh(fbxTPP, warpDiffuseShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	tppWarp->SetPosition(-70.0f, -90.0f, 0.0f);
	tppWarp->SetRotation(-90.0f, 180.0f, 0.0f);
	tppWarp->SetScale(1.0f, 1.0f, 1.0f);

	tppWarp->AddTexture(warpDiffuseTexture);
	tppWarp->AddTexture(tppDiffuseTexture);

	//�迭�� �߰�
	meshes.push_back(tppWarp);

	if (InitializeMeshes() == false)
	{
		return false;
	}

	return true;
}

bool Engine::InitializeTransformation()
{
	//// ī�޶� ���� ����.
	//cameraPosition = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);
	//cameraView = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//camerUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//// �� ���.
	//XMMATRIX view = XMMatrixLookAtLH(cameraPosition, cameraView, camerUpVector);

	// ���� ���.
	// �þ߰� / ��Ⱦ�� ����.
	float fovY = XMConvertToRadians(70.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	/*XMMATRIX projection = XMMatrixPerspectiveFovLH(fovY, aspectRatio, 1.0f, 10000.0f);*/

	//ī�޶� ��ü ����
	camera = new Camera(fovY, aspectRatio, 1.0f, 10000.0f);

	// ���ۿ� ���� ����ü ���� ����.
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectMatrix());
	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	//float x = XMVectorGetX(cameraPosition);
	//float y = XMVectorGetY(cameraPosition);
	//float z = XMVectorGetZ(cameraPosition);
	//matrixData.worldCameraPosition = XMFLOAT3(x, y, z);
	matrixData.worldCameraPosition = camera->GetPosition();

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

void Engine::InitializeInput()
{
	input = new InputClass();
	input->InitializeInput(window->GetWindowHandle());
}

void Engine::InitializeTimer()
{
	gameTimer = new GameTimer();
	gameTimer->StartTimer();
}

bool Engine::InitializeMeshes()
{
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		//��Ƽ���� �ʱ�ȭ
		if ((*mesh)->CompileShaders(device) == false)
		{
			return false;
		}

		//�� ���̴� ��ü ����
		if ((*mesh)->CreateShaders(device) == false)
		{
			return false;
		}

		//�ؽ��� �ε�
		if ((*mesh)->LoadTextures(device) == false)
		{
			return false;
		}
		//���÷�������Ʈ ����
		if ((*mesh)->CreateSamplerState(device) == false)
			return false;

		//���� �ʱ�ȭ
		if ((*mesh)->InitializeBuffers(device)
			== false)
			return false;
	
		//�����Ͷ����� ������Ʈ ����
		if ((*mesh)->CreateRasterizerState(device) == false)
			return false;


	}
	return true;
}
