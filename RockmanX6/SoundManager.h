#pragma once
#include "SingletonBase.h"

#pragma region 사운드 (FMOD + 트릭스터 + 배터리 엔진)
/*
FMOD

- 사운드 엔진 라이브러리
	사운드 관련 엔진이기 때문에 일반적인 확장자는 전부 지원한다
	다양한 플랫폼까지 지원을 한다.

- WinAPI / DirectX 같은 경우에도 자체 라이브러리를 가지고 있으나 가공이 필요한 경우가 많아 불편하다.
	개발 기간 단축 (별도의 스레드로 분리되어 더욱 활용하기 좋아졌다.)

▷ 상용 엔진

- 언리얼 + 유니티에서는 라이센스 계약으로 기본 내장되어있다.
	그렇기 때문에 무료인 경우가 많고 출시를 목표로 할때만 라이센스를 필요로 한다.

- 언리얼 엔진
	매우 무겁기 때문에 라이센스 계약으로 (기본 플래스쉽만)내장되어있다.
	플러그인 추가해서 쓴다.
	ㄴ EX) .bank  - fmod visualizer

- 유니티 엔진
	기본적으로 FMOD채용, 모든 기능을 채용하지는 않는다.
	추가 기능을 사용하기 위해서는 추가 에셋을 설치해야 한다.
	ㄴ EX) .bank  - fmod visualizer

- 이외의 플랫폼
	사용하기 위한 선행처리가 필요하다.
	헤더, 클래스, 이름 공간, 라이브러리 링크 ->  파일??

- 던파, 와우, 바이오쇼크 시리즈, DJMAX RespectV 등등 많은 게임에서 사용하고있다.

▷ FMOD 구성

- 크게 3가지로 나뉜다.
ㄴ 라이브러리 + 프로그램

1. FMOD Ex
	ㄴStudioAPI / CoreAPI
	ㄴ 로우레벨 사운드 엔진(Native 언어에서 채용되는 API)
	ㄴ API 형태 - 가볍고 빠르다.
	ㄴ 코딩을 못하면 전혀 쓸수가 없다. (단점)
	ㄴ 일반적인 음악 포맷을 사용하거나 디자이너가 없을때도 선택이되는 경우가 많다. (wav, mp3, mid)


2. FMOD 이벤트 시스템
	ㄴ Ex의 상위 레이어
	ㄴ 디자이너가 작업한 사운드 컨텐츠를 재생하는 Data 기반 Driven API
	ㄴ 디자이너의 결과물에 따라 최적화된 코드로 성능향상을 노릴 수 있다.
	ㄴ 디자이너에 의해 사운드 튜닝이 가능하기 때문에 프로그래머가 사운드에 신경쓰지 않아도 된다. -> 개발속도 향상

	ㄴ 상황별 사운드를 컨테이너에 넣어두고
	ㄴ 이벤트에 따른 인덱스를 인자로 넣어주고 플레이만 해주면 되는 방식


3. FMOD 디자이너
	ㄴ 사운드 이벤트를 만들어내는 툴
	ㄴ 기획 + 설계 + 방향성 단계에서 FMOD가 채택이 되면 사운드 디자이너가 주로 사용한다.
	ㄴ 상용 엔진, 디자이너 툴이던 트랜지션 설계를 할 예정이기 때문에 프로그래머로서는 편리하다.

디자이너와 협업을 하든지 내가 하든지 상관없이 알아야 하는 내용들이다.

FMOD 포함 외부기능 자체는 라이브러리를 통해 해당 플랫폼과 언어에 맞게끔 커스텀되기 때문에
외부 라이브러리를 사용하기 위해서는 라이브러리에 대한 이해가 필요하다.

	▶ 동적 라이브러리 - .dll
		런타임에서 찾아서 쓰기 때문에 파일패스 신경쓰자.


	▶ 정적 라이브러리 - .lib
		ㄴ 포함되어있기 때문에 따로 찾지 않아도 된다.	(장점)
		ㄴ 라이브러리가 크면 프로젝트가 무거워질수 있다.(단점)

	연산이 필요한 기능이다? ->정적 라이브러리로 속도를 챙기자.



*/
#pragma endregion

using namespace FMOD;

// 사운드 버퍼를 총 30개 + 10개의 여유분으로 준비
#define SOUNDBUFFER 64
#define EXTRACHANNELBUFFER 10

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class SoundManager : public SingletonBase<SoundManager>
{
private:
	struct ChannelInfo
	{
		string soundName;
		Channel* chaneel;
	};

	typedef map<string, Sound*>					mapSoundList;
	typedef map<string, Sound*>::iterator		mapSoundIter;

private:
	// FMOD를 사용하기 위한 시작지점, 끝지점
	System* _system;

	Channel* _chanel;

	// 사운드 리스트
	mapSoundList _mSoundList;

	// 현재 활성화된 채널
	vector<ChannelInfo> _mActiveChannels;


	float menuSound;
	float voiceSound;
	float bgmSound;

public:
	HRESULT init();
	void release();
	void update();

	void addSound(string strKey, const char* fileName, bool bgm, bool loop);

	void play(string strKey, float volume = 1.0f);
	void stop(string strKey);
	void pause(string strKey);

	SoundManager();
	~SoundManager() {}
};