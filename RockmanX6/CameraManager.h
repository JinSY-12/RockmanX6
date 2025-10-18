#pragma once
#include "SingletonBase.h"
#include "BossType.h"

class CameraManager : public SingletonBase<CameraManager>
{
private:

#pragma region 좌표값들
	
	struct CameraPos
	{
		int x;
		int y;
	};

	struct MapSize
	{
		int x;
		int y;
	};

	struct PlayerPos
	{
		int x;
		int y;
	};

	struct CameraRange
	{
		int top;
		int bottom;
		int left;
		int right;
	};

#pragma endregion
	
	// 카메라 좌표
	CameraPos camera;
	MapSize mapSize;
	PlayerPos playerPos;
	CameraRange cameraRange;

	// 페이드 인/아웃 세팅
	GImage* _blackImage;
	GImage* _whiteImage;

	float _blackAlpha;					// 카메라 화면 알파값
	float _whiteAlpha;					// 카메라 화면 알파값

	float _padeTime;					// 카메라 페이드 인/아웃 지속시간

	bool _isBlackPadeIn;				// 카메라 페이드인
	bool _isBlackPadeOut;				// 카메라 페이드아웃
	
	bool _isWhitePadeIn;				// 카메라 페이드인
	bool _isWhitePadeOut;				// 카메라 페이드아웃

	bool _isPadeResult;					// 카메라 페이드 결과

	// 맵 클피핑 관련 변수
	bool cameraLockX;
	bool cameraLockY;

	int stageType;

	BossType mBtype;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void padeIn(float padeTime);
	void padeOut(float padeTime);
	void whiteIn(float padeTime);
	void whiteOut(float padeTime);
		
	// 카메라 위치 조절 - 이동 / 고정
	
	void fixPos(int x, int y) { camera.x = x, camera.y = y; }

	bool isPadeOutComplete() { return _isPadeResult; }

	// 플레이어 위치 체크
	PlayerPos getPlayerPos(void) { return playerPos; }
	void setPlayerPos(int x, int y) { playerPos.x = x, playerPos.y = y; }

	// 내가 사용할 새로운 함수들은 여기에 정립해라
	
	// 카메라 좌표 설정
	CameraPos getCameraPos(void) { return camera; }
	void setCameraPos(int x, int y) { camera.x += x, camera.y += y; }

	MapSize getMapSize(void) { return mapSize; }
	void settingMapMaxSize(int x, int y) { mapSize.x = x, mapSize.y = y; }

	void cameraOffset(void);

	bool getLockX() { return cameraLockX; }
	bool getLockY() { return cameraLockY; }
	
	void setMaxCameraRange(void);
	void setStage(BossType stage) { mBtype = stage; }
	void cameraTest(void);


	// 카메라 초기값 -> 나중에 링크 침대에서 시작하는걸로 카메라 좌표 수정
	// 링크 하우스 입구
	// CameraManager() { camera.x = -255, camera.y = -480; }
	// 링크네 집
	//CameraManager() { camera.x = 100, camera.y = 100; }

	CameraManager() {}
	~CameraManager() {}
};


