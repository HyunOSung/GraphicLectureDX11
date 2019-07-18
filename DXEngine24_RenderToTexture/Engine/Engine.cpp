#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{

	Memory::SafeDelete(camera);
	Memory::SafeDelete(input);

	Memory::SafeRelease(constantBuffer);

	renderTexture->Release();
	Memory::SafeDelete(renderTexture);

	rtRenderer->Release();
	Memory::SafeDelete(rtRenderer);

	rtMaterial->Release();
	Memory::SafeDelete(rtMaterial);




}

int Engine::Run()
{
	return window->Run(this);
}

bool Engine::Init()
{
	if (DXApp::Init() == false)
		return false;

	// 타이머 초기화
	InitializeTimer();

	// 입력 초기화
	InitializeInput();

	// 장면 초기화.
	if (InitializeScene() == false)
		return false;

	// 행렬 초기화.
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update(float deltaTime)
{
	//// 월드 행렬 바인딩.
	//mesh->Update(deviceContext);
	//mesh2->Update(deviceContext);

	//camera->UpdateCamera();

	//PerSceneBuffer matrixData;
	//matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectMatrix());
	//matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	//matrixData.worldCameraPosition = camera->GetPosition();

	//deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
	//// 뷰/투영 행렬 바인딩.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render(float deltaTime)
{
	//렌더 투 텍스쳐
	UpdatePerspectiveCamera();
	RenderToTexture();

	float color[] = { 0.0f, 0.5f, 0.5f, 1.0f };
	BeginScene(color);

	//백버퍼에 그리기
	RenderScene();

	//직교모드 카메라 설정
	UpdateOrthographicCamera();

	rtMaterial->BindeShaders(deviceContext);
	rtMaterial->BindTexture(deviceContext, renderTexture->GetShaderResourceView());
	rtMaterial->BindSamplerState(deviceContext);
	rtRenderer->UpdateBuffers(deviceContext, 100, 100);
	rtRenderer->RenderBuffers(deviceContext);


	rtRenderer->UpdateBuffers(deviceContext, 1000, 300);
	rtRenderer->RenderBuffers(deviceContext);

	// 순서대로 정렬
	// 1. 월드 행렬 바인딩.
	// 2. 뷰/투영 행렬 바인딩.
	// 3. 셰이더 바인딩.
	// 4. 텍스처/샘플러 스테이트 바인딩.
	// 5. 메시 버퍼 그리기.

	// 차례대로 해줘야 제대로 돌아간다. 한쪽이 처리가 안되었는데 처리를 하려고 하면 렌더링이 되지 않는다.
	// 래스터라이저 스테이트 바인딩, 꼭 버퍼 그리기 전에 해줄것


	// 백버퍼 <-> 프론트 버퍼 교환.
	swapChain->Present(1, 0);
}

void Engine::ProcessInput(float deltaTime)
{
	//ESC 종료처리
	if (input->IsKeyDown(Keyboard::Keys::Escape))
	{

		if (MessageBox(NULL,
			TEXT("종료 하시겠습니까?"),
			TEXT("종료"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// 예 버튼 눌리면 창 삭제.
			DestroyWindow(window->GetWindowHandle());
		}
		/*else
		{
			input->ResetKeyboardState();
		}
		*/
	}

	//카메라 이동 회전
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

	//마우스 왼쪽 버튼 눌릴 때만 이동
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{
		camera->Yaw(state.x*rotationSpeed * deltaTime);
		camera->Pitch(state.y*rotationSpeed * deltaTime);
	}

}

bool Engine::InitializeScene()
{

	//FBX 이름 변수
	LPCSTR fbxTPP = "Resources/Models/HeroTPP.FBX";
	LPCSTR fbxBox = "Resources/Models/SK_CharM_Cardboard.FBX";
	LPCSTR fbxCube = "Resources/Models/cube.FBX";
	LPCSTR fbxSphere = "Resources/Models/sphere.FBX";
	LPCSTR fbxBarbarous = "Resources/Models/SK_CharM_Barbarous.FBX";
	LPCSTR fbxWarrior = "Resources/Models/SK_CharM_Warrior.FBX";
	LPCSTR fbxBear = "Resources/Models/Enemy_Bear.FBX";

	//텍스쳐 이름 변수
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


	LPCTSTR fortpointTexture = TEXT("Resources/Textures/FortPoint.dds");
	LPCTSTR goldengatebridgeTexture = TEXT("Resources/Textures/GoldenGateBridge.dds");





	//셰이더 이름 변수
	LPCTSTR warpDiffuseShader = TEXT("Shader/WarpDiffuse");
	LPCTSTR normalMappingShader = TEXT("Shader/NormalMapping2");
	LPCTSTR toon2Shader = TEXT("Shader/Toon2");
	LPCTSTR cubeMappingShader = TEXT("Shader/CubeMapping");
	LPCTSTR rimShader = TEXT("Shader/Rim");
	LPCTSTR orenNayerDiffuseShader = TEXT("Shader/OrenNayer");
	LPCTSTR cookTorranceShader = TEXT("Shader/CookTorrance");


	Mesh* tppRim = new Mesh(
		fbxTPP, cookTorranceShader,
		D3D11_FILL_SOLID, D3D11_CULL_NONE
	);
	tppRim->SetPosition(0.0f, -90.0f, 0.0f);
	tppRim->SetRotation(-90.0f, 0.0f, 180.0f);
	tppRim->SetScale(1.0f, 1.0f, 1.0f);

	tppRim->AddTexture(tppDiffuseTexture);
	meshes.push_back(tppRim);


	// 메쉬 생성.

	//Mesh* tppNormal = new Mesh(fbxTPP, normalMappingShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	//tppNormal->SetPosition(70.0f, -90.0f, 0.0f);
	//tppNormal->SetRotation(-90.0f, 180.0f, 0.0f);
	//tppNormal->SetScale(1.0f, 1.0f, 1.0f);

	//tppNormal->AddTexture(tppDiffuseTexture);
	//tppNormal->AddTexture(tppNormaTexture);

	//meshes.push_back(tppNormal);

	//Mesh* tppWarp = new Mesh(fbxTPP, warpDiffuseShader, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	//tppWarp->SetPosition(-70.0f, -90.0f, 0.0f);
	//tppWarp->SetRotation(-90.0f, 180.0f, 0.0f);
	//tppWarp->SetScale(1.0f, 1.0f, 1.0f);

	//tppWarp->AddTexture(warpDiffuseTexture);
	//tppWarp->AddTexture(tppDiffuseTexture);

	////배열에 추가
	//meshes.push_back(tppWarp);

	if (InitializeMeshes() == false)
	{
		return false;
	}

	//RT Material 초기화
	rtMaterial = new Material(TEXT("Shader/"));

	//컴파일
	if (rtMaterial->CompileShaders(device) == false)
	{
		return false;
	}

	//셰이더 객체 생성
	if (rtMaterial->CreateShaders(device) == false)
	{
		return false;
	}

	//샘플러 스테이트 생성
	if (rtMaterial->CreateSamplerState(device) == false)
	{
		return false;
	}


	return true;
}

bool Engine::InitializeTransformation()
{
	//// 카메라 정보 설정.
	//cameraPosition = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);
	//cameraView = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//camerUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//// 뷰 행렬.
	//XMMATRIX view = XMMatrixLookAtLH(cameraPosition, cameraView, camerUpVector);

	// 투영 행렬.
	// 시야각 / 종횡비 설정.
	float fovY = XMConvertToRadians(70.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	/*XMMATRIX projection = XMMatrixPerspectiveFovLH(fovY, aspectRatio, 1.0f, 10000.0f);*/

	//카메라 객체 생성
	camera = new Camera(fovY, aspectRatio, 1.0f, 10000.0f);

	// 버퍼에 담을 구조체 변수 설정.
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectMatrix());
	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	//float x = XMVectorGetX(cameraPosition);
	//float y = XMVectorGetY(cameraPosition);
	//float z = XMVectorGetZ(cameraPosition);
	//matrixData.worldCameraPosition = XMFLOAT3(x, y, z);
	matrixData.worldCameraPosition = camera->GetPosition();

	// 버퍼 생성.
	// 버퍼 서술자.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerSceneBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 배열 정보 넣어줄 구조체.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// 상수 버퍼 생성.
	HRESULT result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("씬 상수 버퍼 생성 실패")))
	{
		return false;
	}

	// Render Texture, RT Renderer 초기화
	renderTexture = new RenderTexture();
	if (renderTexture->Initialize(
		device, 
		camera, 
		window->GetScreenWidth(), 
		window->GetScreenHeight()) == false)
	{
		return false;
	}

	rtRenderer = new RTRenderer();
	if (rtRenderer->Initialize(
		device,
		rtMaterial->GetVertexShader()->GetShaderBuffer(),
		window->GetScreenWidth(),
		window->GetScreenHeight(),
		400, 400) == false)
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
		//머티리얼 초기화
		if ((*mesh)->CompileShaders(device) == false)
		{
			return false;
		}

		//각 셰이더 객체 생성
		if ((*mesh)->CreateShaders(device) == false)
		{
			return false;
		}

		//텍스쳐 로드
		if ((*mesh)->LoadTextures(device) == false)
		{
			return false;
		}
		//샘플러스테이트 생성
		if ((*mesh)->CreateSamplerState(device) == false)
			return false;

		//버퍼 초기화
		if ((*mesh)->InitializeBuffers(device)
			== false)
			return false;

		//래스터라이저 스테이트 생성
		if ((*mesh)->CreateRasterizerState(device) == false)
			return false;


	}
	return true;
}

void Engine::RenderToTexture()
{
	//렌더 타겟 바꾸기
	renderTexture->SetRenderTarget(deviceContext, depthStencilView);

	float color[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	renderTexture->ClearRenderTarget(
		deviceContext,
		depthStencilView,
		color
	);


	//씬 그리기
	RenderScene();

	//렌더 타겟 돌려놓기
	deviceContext->OMSetRenderTargets(
		1,
		&renderTargetView,
		depthStencilView
	);
}

void Engine::RenderScene()
{
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		(*mesh)->Update(deviceContext);
		(*mesh)->BindeShaders(deviceContext);
		(*mesh)->BindeTextures(deviceContext);
		(*mesh)->BindSamplerState(deviceContext);
		(*mesh)->BindRasterizerState(deviceContext);
		(*mesh)->RenderBuffers(deviceContext);
	}
}

void Engine::BeginScene(float color[])
{
	// 렌더 타겟을 설정한 색상으로 칠하기.
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	// 뎁스/스텐실 뷰 지우기.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



}

void Engine::UpdatePerspectiveCamera()
{
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
}

void Engine::UpdateOrthographicCamera()
{
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * renderTexture->GetProjectionMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
}
