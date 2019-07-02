#pragma once

#include<d3d11.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib") //속성->링커, 디렉토리에서 라이브러리를 설정할 수 있음

namespace Memory
{
	template<class T> void SafeRelease(T& t) //C#이나 자바의 제네릭이 여기서 옴
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