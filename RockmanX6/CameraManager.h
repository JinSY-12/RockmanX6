#pragma once
#include "SingletonBase.h"

class CameraManager : public SingletonBase<CameraManager>
{
private:
	struct Pos
	{
		int x;
		int y;
	}camera;

	struct MapSize
	{
		int x;
		int y;
	}maxSize;

	struct PlayerPos
	{
		int x;
		int y;
	}playerPos;

	struct ObjectPos
	{
		int x;
		int y;
	}objectPos;

	RECT playerRect;

	GImage* _blackImage;
	GImage* _whiteImage;

	float _blackAlpha;					// 카메라 화면 알파값
	float _whiteAlpha;					// 카메라 화면 알파값

	float _padeTime;				// 카메라 페이드 인/아웃 지속시간

	bool _isBlackPadeIn;					// 카메라 페이드인
	bool _isBlackPadeOut;				// 카메라 페이드아웃
	
	bool _isWhitePadeIn;
	bool _isWhitePadeOut;

	bool _isPadeResult;				// 카메라 페이드 결과

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void padeIn(float padeTime);
	void padeOut(float padeTime);
	void whiteIn(float padeTime);
	void whiteOut(float padeTime);
	void cameraShake(float intensity, float duration);

	void settingMapMaxSize(int x, int y) { maxSize.x = x, maxSize.y = y; }
	MapSize getMapSize(void) { return maxSize; }

	RECT getPlayerRect(void) { return playerRect; }
	void setPlayerRect(RECT player) { playerRect = player; }

	// 카메라 위치 조절 - 이동 / 고정
	Pos getPos(void) { return camera; }
	void setPos(int x, int y) { camera.x += x, camera.y += y; }
	void fixPos(int x, int y) { camera.x = x, camera.y = y; }

	bool isPadeOutComplete() { return _isPadeResult; }

	// 플레이어 위치 체크
	PlayerPos getPlayerPos(void) { return playerPos; }
	void setPlayerPos(int x, int y) { playerPos.x = x, playerPos.y = y; }

	// 카메라 초기값 -> 나중에 링크 침대에서 시작하는걸로 카메라 좌표 수정
	// 링크 하우스 입구
	// CameraManager() { camera.x = -255, camera.y = -480; }
	// 링크네 집
	//CameraManager() { camera.x = 100, camera.y = 100; }
	CameraManager() { camera.x = 0, camera.y = 960 * 3 - 768; }
	~CameraManager() {}
};

