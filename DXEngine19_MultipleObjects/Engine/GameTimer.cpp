#include "GameTimer.h"



GameTimer::GameTimer()
{
}


GameTimer::~GameTimer()
{
}

void GameTimer::StartTimer()
{
	//값을 전달 받을 변수 선언
	LARGE_INTEGER frequencyCount;

	//하드웨어(CPU) 진동수 저장(초단위)
	QueryPerformanceFrequency(&frequencyCount);

	//더블 타입으로 변환해서 저장
	countsPerSecond = static_cast<double>(frequencyCount.QuadPart);

	//틱(Tick - Frame - Update)
	//프레임간격을 측정하는 시간 단위를 델타 타임, 언리얼은 델타지만 델타 세컨드(Delta Second)를 사용
	QueryPerformanceCounter(&frequencyCount);
	countStart = frequencyCount.QuadPart;

}


//엔진 시작 이후로 지난 시간 변환
double GameTimer::GetTime() const
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	double time = static_cast<double>((currentTime.QuadPart - countStart) / countsPerSecond);

	//받은 time을 리턴해줘야 FPS를 계산할 수 있다
	return time;
}

float GameTimer::GetDeltaTime() 
{
	//틱 단위 현재시간 구하기
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	//틱 단위 간격 계산
	_int64 tickCount = currentTime.QuadPart - previousFrameTime;

	//이전 프레임 시간 갱신
	previousFrameTime = currentTime.QuadPart;

	//예외처리
	if (tickCount < 0)
	{
		tickCount = 0;
	}

	//프레임 시간을 초단위로 변환
	float deltaTime = static_cast<float>(tickCount / countsPerSecond);

	//예외처리
	if (deltaTime < 0.0f)
	{
		deltaTime = 0.0f;
	}

	return deltaTime;
}

void GameTimer::UpdateTimer(AppWindow * window)
{
	//틱 단위 프레임 수 증가시키기
	++frameCount;

	//1초 지나면 타이머 초기화
	//일반적인 타이머 생성시에는 여기서 만들면 안됨, 밖으로 빼야 함
	if (GetTime() > 1.0f)
	{
		Reset(window);
	}
}

void GameTimer::Reset(AppWindow * window)
{
	//FPS 기록
	fps = frameCount;
	frameCount = 0;
	StartTimer();

	//창 타이틀 바에 FPS 문자열 추가
	TCHAR buffer[256];
	_itow_s(fps, buffer, 10);		//숫자를 문자로 변환해서 버퍼에 넣어줌 인티저 투 와이드캐릭터

	//비주얼 스튜디오에서만 가능한 출력 도구 : 콘솔에 출력(Visual Studio Console)에 출력
	OutputDebugString(TEXT("FPS: "));
	OutputDebugString(buffer);
	OutputDebugString(TEXT("\n"));


	//문자열 조합해서 창 제목 바꾸기
	std::wstring newTitle = std::wstring(window->GetApplicationName());
	newTitle = newTitle + TEXT("FPS: ") + buffer;
	SetWindowText(window->GetWindowHandle(), newTitle.c_str());
}

int GameTimer::GetFPS() const
{
	return fps;
}
