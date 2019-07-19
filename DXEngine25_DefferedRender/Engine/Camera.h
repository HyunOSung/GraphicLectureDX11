#pragma once

#include"DXUtil.h"

using namespace DirectX;

class Camera : public AlignedAllocaton<16>
{
public:
	Camera();
	Camera(float fovY, float aspectRatio, float nearZ, float farZ);
	~Camera();

	//Getter
	float GetAspectRatio() const { return aspectRatio; }

	XMMATRIX GetViewMatrix() const { return viewMatrix; }
	XMMATRIX GetProjectMatrix() const { return projectionMatrix; }

	XMVECTOR GetPositionVector() const { return cameraPosition; }
	XMFLOAT3 GetPosition() const;

	const XMVECTOR GetLookVector() const { return cameraLook; }
	const XMVECTOR GetUpVector() const { return cameraUp; }

	const float GetNearZ() const { return nearZ; }
	const float GetFarZ() const { return farZ; }



	//Setter
	void SetAspectRatio(UINT width, UINT height);

	void SetPosition(float x, float y, float z);

	//ī�޶� �̵�����
	void MoveForward(float direction);
	void MoveRight(float direction);
	void MoveUp(float direction);

	//��(Yaw) Y�� ȸ��
	void Yaw(float angle);
	//��ġ(Pitch) X�� ȸ��
	void Pitch(float angle);
	////��(Roll) Z�� ȸ��
	//void Roll(float angle);
	void UpdateCamera();


private:
	void UpdateViewMatrix();
	void UpdateProjectMatrix();

private:
	float fovY;					//���� �þ߰�
	float aspectRatio;			//��Ⱦ��
	float nearZ;				//�������� near �Ÿ�
	float farZ;					//�������� far �Ÿ�


	//�ʱ�ȭ ����� �ϴ� ����
	float yaw = 0.0f;					//�� ȸ����
	float pitch = 0.0f;				//��ġ ȸ����



	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	

	XMVECTOR cameraPosition = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);	//ī�޶� ��ġ ��
	XMVECTOR cameraLook = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);			//		L-P = F 
	XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);			//ī�޶� ����	
	XMVECTOR cameraRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);	
	
	XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	


};

