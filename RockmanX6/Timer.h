#pragma once

//고성능 타이머 무겁기 때문에 node를 받지 않고 메인게임에 바로 들어가자

// int는 4byte
// long은 64비트에서 8byte


#pragma region
/*
- API 공용함수
ㄴ 문제가 있다.
ㄴ 최초 설계 단게에서 게임을 고려하고 만든 타이머들이 아니기 때문에 정밀도가 떨어진다.


timeGetTime ##	(윈도우가 시작된지 1분 지났다면 60000반환)
GetTickCount	(부팅 시간 체크 -> 부팅된 이후 1초에 1000씩 증가) 49.7일 동안 윈도우를 켜놓으면 0으로 초기화되어 문제 발생
GetTickCount64

- 타이머
ㄴ 시간을 측정하는 함수들은 기본적으로 윈도우가 시작된 이후 지난 시간을 Mili second 단위로 반환.
ㄴ 밀리세컨은 게임에서 쓰기에는 정밀도가 떨어진다.
	따라서 고성능 타이머가 필요하다!
	9000000만 분의 1초마다

해상도, 클럭

◆ 해상도?
ㄴ 1분을 기준으로 얼마나 많은 프레임으로 나눌 수 있는지

◆ 클럭(진동수)?
ㄴ CPU 클럭
ㄴ 디지털 회로의 전기적 진동수 Hz(초당 진동수) 단위로 표기
ㄴ 진동 1번에 2진법 신호 1번, 1비트를 옮길 수 있다.
1GHz 1초에 1000000000번 = 10^9 = 10억번

단위
	ㄴ Milli Second
	ㄴ 10 Milli Second		Centi
	ㄴ 100 Milli Second		Deci
*/
#pragma endregion timer

class Timer
{
private:
	float _timeScale;				// 경과 시간
	float _timeElapsed;				// 마지막 시간과 현재 시간의 경과값
	bool _isHardWare;				// 하드웨어 지연 여부

	__int64 _curTime;				// 현재 프레임 시간
	__int64 _lastTime;				// 이전 프레임 시간
	__int64 _periodFrequency;		// 시간 주기

	unsigned long _frameRate;		// 초당 프레임 수
	unsigned long _FPSFrameCount;	// 초당 프레임 카운트

	float _FPSTimeElapsed;			// FPS 시간 경과량 체크
	float _worldTime;				// 게임 시작 후 경과시간

public:
	HRESULT init(void);
	// -> 수직동기화 시스템을 위해
	void tick(float lockFPS = 0.0f); //프레임 제한


	//오염되면 안되는 값.
	//함수에 붙은 const 외부에서 값 변경을 허용하지 않기 위해
	unsigned long getFrameRate(char* str = nullptr) const;
	inline float getElapsedTime(void) const { return _timeElapsed; }
	inline float getWorldTime(void) const { return _worldTime; }

	Timer() {}
	~Timer() {}
};

