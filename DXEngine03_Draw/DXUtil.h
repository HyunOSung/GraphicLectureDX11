#pragma once

#include<d3d11.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib") //�Ӽ�->��Ŀ, ���丮���� ���̺귯���� ������ �� ����

namespace Memory
{
	template<class T> void SafeRelease(T& t) //C#�̳� �ڹ��� ���׸��� ���⼭ ��
	{
		if (t)
		{
			t->Release();
			t = NULL;
		}
	}

	template<class T>void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	template<class T>void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = NULL;
		}
	}
}

//���� Ȯ�ο� �Լ�.
bool isError(HRESULT hr, LPCTSTR msg);