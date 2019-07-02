#pragma once

#include"DXUtil.h"

class Shader
{
public:
	Shader();
	Shader(LPTSTR fileName);
	Shader(LPTSTR fileName, LPCSTR entry/*셰이더 메인함수*/, LPCSTR profile/*셰이버 버전명에 따른 정보*/);
	virtual ~Shader();

	virtual bool CompileShader(ID3D11Device* device) = 0;
	virtual bool CreateShader(ID3D11Device* device) = 0;
	virtual bool BindShader(ID3D11DeviceContext* device) = 0;
	virtual void Release();

	void SetFileName(LPCTSTR fileName)
	{
		this->fileName = fileName;
	}
	//Getters
	LPCTSTR GetFileName() const { return fileName; }
	LPCSTR GetEntryPoint() const { return entryPoint; }
	LPCSTR GetProfile() const { return profile; }
	ID3DBlob* GetShaderBuffer() const { return shaderBuffer; }


protected:
	LPCTSTR fileName;		//셰이더 파일 이름(확장자 포함)
	LPCSTR entryPoint;		//셰이더 진입(main) 함수 이름
	LPCSTR profile;			//셰이더 버전 정보 ex"vs_4_0"
	ID3DBlob* shaderBuffer;	//셰이더 컴파일 결과 저장용 변수
};

