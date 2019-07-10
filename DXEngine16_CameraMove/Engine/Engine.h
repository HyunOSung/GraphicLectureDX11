#pragma once

#include "DXApp.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "InputClass.h"

//#include "VertexShader.h"
//#include "PixelShader.h"

class Engine : public DXApp
{
private:
	struct PerSceneBuffer
	{
		PerSceneBuffer() { ZeroMemory(this, sizeof(this)); }

		XMMATRIX viewProjection; 
		XMFLOAT3 worldLightPosition; 
		XMFLOAT3 worldCameraPosition; 
	};

public:
	Engine(HINSTANCE hinstance);
	~Engine();

	int Run();
	bool Init() override;
	void Update() override;
	void Render() override;

	//�Է�ó���Լ�
	void ProcessInput();

private:

	// ���(Scene) �ʱ�ȭ.
	bool InitializeScene();
	bool InitializeTransformation();

	// �Է� �ʱ�ȭ
	void InitializeInput();

private:

	Material* material;
	Material* material2;


	//VertexShader* vertexShader;
	//PixelShader* pixelShader;

	ID3D11Buffer* constantBuffer;			// ��, ������� ����.

	// ī�޶� ����.
	//XMVECTOR cameraPosition;		// ī�޶� ��ġ.
	//XMVECTOR cameraView;			// ī�޶� ����.
	//XMVECTOR camerUpVector;			// ī�޶� �� ����.
	Camera* camera;

	Mesh* mesh;
	Mesh* mesh2;

	//�Է� Ŭ����
	InputClass* input;
};