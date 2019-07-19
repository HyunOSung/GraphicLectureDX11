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

	//카메라 이동관련
	void MoveForward(float direction);
	void MoveRight(float direction);
	void MoveUp(float direction);

	//요(Yaw) Y축 회전
	void Yaw(float angle);
	//피치(Pitch) X축 회전
	void Pitch(float angle);
	////롤(Roll) Z축 회전
	//void Roll(float angle);
	void UpdateCamera();


private:
	void UpdateViewMatrix();
	void UpdateProjectMatrix();

private:
	float fovY;					//세로 시야각
	float aspectRatio;			//종횡비
	float nearZ;				//프러스텀 near 거리
	float farZ;					//프러스텀 far 거리


	//초기화 해줘야 하는 값들
	float yaw = 0.0f;					//요 회전값
	float pitch = 0.0f;				//피치 회전값



	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	

	XMVECTOR cameraPosition = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);	//카메라 위치 값
	XMVECTOR cameraLook = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);			//		L-P = F 
	XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);			//카메라 방향	
	XMVECTOR cameraRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);	
	
	XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	


};

