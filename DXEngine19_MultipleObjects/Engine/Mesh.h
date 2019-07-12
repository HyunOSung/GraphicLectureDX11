#pragma once

#include "DXUtil.h"
#include "Vertex.h"
#include "Material.h"
#include <vector>

using namespace DirectX;

class Mesh : public AlignedAllocaton<16>
{
private:
	//struct Vertex
	//{
	//	XMFLOAT3 position;
	//	XMFLOAT4 color;
	//	XMFLOAT2 textureCoord;

	//	Vertex(float x, float y, float z) :
	//		position(x, y, z) { }
	//	Vertex(XMFLOAT3 position) : position(position) { }
	//	Vertex(XMFLOAT3 position, XMFLOAT4 color) 
	//		: position(position), color(color) { }
	//	Vertex(XMFLOAT3 position, XMFLOAT4 color, XMFLOAT2 uv)
	//		: position(position), color(color), textureCoord(uv) { }
	//};

	struct PerObjectBuffer
	{
		PerObjectBuffer() { ZeroMemory(this, sizeof(this)); }

		XMMATRIX world;
	};

public:
	Mesh();
	Mesh(float x, float y, float z);
	Mesh(LPCSTR fbxName, LPCTSTR shaderName);
	~Mesh();

	////16바이트 정렬을 위한 연산자 오버로딩
	//void* operator new(size_t byteSize)
	//{
	//	return _aligned_malloc(byteSize, 16);
	//}

	//void operator delete(void* pointer)
	//{
	//	_aligned_free(pointer);
	//}


	bool InitializeBuffers(ID3D11Device* device);	
	//bool InitializeBuffers(ID3D11Device* device, ID3DBlob * vertexShaderBuffer);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	void Update(ID3D11DeviceContext* deviceContext);

	//머티리얼 함수
	bool CompileShaders(ID3D11Device* device);
	bool CreateShaders(ID3D11Device* device);
	void BindeShaders(ID3D11DeviceContext* deviceContext);

	void AddTexture(LPCTSTR fileName);
	bool LoadTextures(ID3D11Device* device);
	void BindeTextures(ID3D11DeviceContext* deviceContext);

	bool CreateSamplerState(ID3D11Device* device);
	void BindSamplerState(ID3D11DeviceContext* deviceContext);




	// Getter.
	XMFLOAT3 GetPosition() const { return position; }
	XMFLOAT3 GetRotation() const { return rotation; }
	XMFLOAT3 GetScale() const { return scale; }

	XMMATRIX GetWorldMatrix();

	XMMATRIX GetTranslationMatrix();
	XMMATRIX GetRotationMatrix();
	XMMATRIX GetScaleMatrix();

	LPCSTR GetFBXName() const { return fileName; }
	std::vector<Vertex>* GetVertexArray() { return &vertices; }
	std::vector<DWORD>* GetIndexArray() { return &indices; }

	int GetVertexCount() const { return vertices.size(); }
	int GetIndexCount() const { return indices.size(); }

	D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() { return inputLayoutDesc; }

	// Setter.
	void SetPosition(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	void SetRotation(float x, float y, float z) { rotation = XMFLOAT3(x, y, z); }
	void SetScale(float x, float y, float z) { scale = XMFLOAT3(x, y, z); }

private:

	bool InitializeBuffers(ID3D11Device* device, ID3DBlob * vertexShaderBuffer);

private:

	LPCSTR fileName;			// FBX 파일 이름.
	LPCTSTR shaderName;			//셰이더 파일 이름
	std::vector<Vertex> vertices;		// 정점 배열.
	std::vector<DWORD> indices;			// 인덱스 배열.
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[5] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int nVertices;				// 정점 개수.
	int nIndices;				// 인덱스 개수.

	ID3D11Buffer* vertexBuffer;			// 정점 정보 저장할 버퍼.
	ID3D11Buffer* indexBuffer;			// 인덱스 저장할 버퍼.
	ID3D11InputLayout* inputLayout;		// 입력 레이아웃.
	ID3D11Buffer* constantBuffer;		// 상수 버퍼 (월드 행렬).

	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 위치 정보.
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 회전 정보.
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);			// 스케일 정보.

	PerObjectBuffer matrixData;

	//머티리얼
	Material* material;

};