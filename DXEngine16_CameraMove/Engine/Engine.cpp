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

	Memory::SafeDelete(camera);

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

void Engine::Update()
{
	//// 월드 행렬 바인딩.
	//mesh->Update(deviceContext);
	//mesh2->Update(deviceContext);

	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(camera->GetViewMatrix() * camera->GetProjectMatrix());
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);
	//// 뷰/투영 행렬 바인딩.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render()
{
	float color[] = { 0.0f, 0.5f, 0.5f, 1.0f };

	// 렌더 타겟을 설정한 색상으로 칠하기.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// 뎁스/스텐실 뷰 지우기.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	//// 메시 회전
	//if (input->IsKeyDown(Keyboard::Keys::A) || input->IsKeyDown(Keyboard::Keys::Left))
	//{
	//	XMFLOAT3 rot = mesh->GetRotation();
	//	rot.y += 1.5f;

	//	// 변경한 회전 값 설정하기.
	//	mesh->SetRotation(rot.x, rot.y, rot.z);

	//	XMFLOAT3 pos = mesh->GetPosition();
	//	pos.x += 3.0f;
	//	mesh->SetPosition(pos.x, pos.y, pos.z);
	//}

	// 순서대로 정렬
	// 1. 월드 행렬 바인딩.
	// 2. 뷰/투영 행렬 바인딩.
	// 3. 셰이더 바인딩.
	// 4. 텍스처/샘플러 스테이트 바인딩.
	// 5. 메시 버퍼 그리기.

	// 차례대로 해줘야 제대로 돌아간다. 한쪽이 처리가 안되었는데 처리를 하려고 하면 렌더링이 되지 않는다.
	mesh->Update(deviceContext);
	material->BindeShaders(deviceContext);
	material->BindeTexture(deviceContext);
	material->BindSamplerState(deviceContext);
	mesh->RenderBuffers(deviceContext);




	mesh2->Update(deviceContext);
	material2->BindeShaders(deviceContext);
	material2->BindeTexture(deviceContext);
	material2->BindSamplerState(deviceContext);
	mesh2->RenderBuffers(deviceContext);





	// 백버퍼 <-> 프론트 버퍼 교환.
	swapChain->Present(1, 0);
}

void Engine::ProcessInput()
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
	if (input->IsKeyDown(Keyboard::Keys::A) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveRight(-1.0f * 2.0f);
	}
	if (input->IsKeyDown(Keyboard::Keys::D) || input->IsKeyDown(Keyboard::Keys::Right))
	{
		camera->MoveRight(1.0f * 2.0f);

	}
	if (input->IsKeyDown(Keyboard::Keys::W) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveForward(1.0f * 2.0f);

	}
	if (input->IsKeyDown(Keyboard::Keys::S) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveForward(-1.0f * 2.0f);

	}
	if (input->IsKeyDown(Keyboard::Keys::Q) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveUp(1.0f * 2.0f);

	}
	if (input->IsKeyDown(Keyboard::Keys::E) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveUp(-1.0f * 2.0f);

	}

	//마우스 왼쪽 버튼 눌릴 때만 이동
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{
		camera->Yaw(state.x*0.1f);
		camera->Pitch(state.y*0.1f);
	}

}

bool Engine::InitializeScene()
{
	//// 정점 셰이더 생성.
	//vertexShader = new VertexShader(TEXT("Shader//ToonVS.fx"));
	//// 정점 셰이더 컴파일.
	//if (vertexShader->CompileShader(device) == false)
	//	return false;

	//// 정점 셰이더 객체 생성.
	//if (vertexShader->CreateShader(device) == false)
	//	return false;

	//// 픽셀 셰이더 생성.
	//pixelShader = new PixelShader(TEXT("Shader//ToonPS.fx"));

	//// 픽셀 셰이더 컴파일.
	//if (pixelShader->CompileShader(device) == false)
	//	return false;

	//// 픽셀 셰이더 객체 생성.
	//if (pixelShader->CreateShader(device) == false)
	//	return false;

	////// 텍스처 로드.
	//if (pixelShader->LoadTexture(
	//	device,
	//	TEXT("Resources/Textures/earth.jpg"))
	//	== false)
	//{
	//	return false;
	//}

	//// 샘플러 스테이트 생성.
	//if (pixelShader->CreateSamplerState(device) == false)
	//	return false;

	//머티리얼 객체 생성
	material = new Material(TEXT("Shader//Toon"));
	material2 = new Material(TEXT("Shader//Toon"));

	//머티리얼 컴파일
	if (material->CompileShaders(device) == false)
	{
		return false;
	}

	if (material2->CompileShaders(device) == false)
	{
		return false;
	}

	//각 셰이더 객체 생성
	if (material->CreateShaders(device) == false)
	{
		return false;
	}

	if (material2->CreateShaders(device) == false)
	{
		return false;
	}

	//텍스쳐 관련 처리
	//텍스쳐 추가
	material->AddTexture(TEXT("Resources/Textures/earth.jpg"));
	material2->AddTexture(TEXT("Resources/Textures/earth.jpg"));

	//텍스쳐 로드
	if (material->LoadTexture(device) == false)
	{
		return false;
	}

	if (material2->LoadTexture(device) == false)
	{
		return false;
	}


	// 메쉬 생성.
	//mesh = new Mesh(0.0f, 0.0f, 0.0f);
	mesh = new Mesh("Resources/Models/Sphere.FBX");
	mesh->SetPosition(70.0f, 0.0f, 0.0f);
	mesh->SetRotation(-90.0f, 0.0f, 0.0f);
	mesh->SetScale(1.2f, 1.2f, 1.2f);

	mesh2 = new Mesh("Resources/Models/Sphere.FBX");
	mesh2->SetPosition(-70.0f, 0.0f, 0.0f);
	mesh2->SetRotation(-90.0f, 0.0f, 0.0f);
	mesh2->SetScale(0.5f, 0.5f, 0.5f);

	//// 초기화.
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

	return true;
}

void Engine::InitializeInput()
{
	input = new InputClass();
	input->InitializeInput(window->GetWindowHandle());
}
