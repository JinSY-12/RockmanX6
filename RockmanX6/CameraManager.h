#pragma once
#include "SingletonBase.h"
#include "BossType.h"

class CameraManager : public SingletonBase<CameraManager>
{
private:

#pragma region 좌표값들
	enum CameraMoveDir
	{
		None,
		Up,
		Down
	};

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

	struct CheckPointSave
	{
		int x;
		int y;
	};

#pragma endregion
	
	// 카메라 좌표
	CameraPos camera;
	MapSize mapSize;
	PlayerPos playerPos;
	CheckPointSave checkPointSave;

	CameraRange cameraRange;
	CameraMoveDir moveDir;

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

	bool _isPadeInResult;				// 카메라 페이드 인 결과
	bool _isPadeOutResult;				// 카메라 페이드 아웃 결과

	// 맵 클피핑 관련 변수
	bool cameraLockX;
	bool cameraLockY;

	int stageType;

	BossType mBtype;

	bool isAutoMove;
	float targetX;
	float targetY;
	float lerpSpeed;
	bool useLerpY;

	string zoneName;
	int zoneNumber;
	int prevZoneNumber;

	bool cameraMove;
	bool lerpCompleteX;
	bool lerpCompleteY;

	bool cameraMoveEnd;
	float timer;

	bool test;

	int locationTestX;
	int locationTestRight;

	int locationTestY;
	int locationTestBottom;

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

	bool isPadeInComplete() { return _isPadeInResult; }
	bool isPadeOutComplete() { return _isPadeOutResult; }

	// 플레이어 위치 체크
	PlayerPos getPlayerPos(void) { return playerPos; }
	void setPlayerPos(int x, int y) { playerPos.x = x, playerPos.y = y; }
	CheckPointSave getCheckPoint() { return checkPointSave; }
	inline void setCheckPoint(int x, int y, int zone)
	{
		if (zoneNumber >= prevZoneNumber)
		{
			checkPointSave.x = x;
			checkPointSave.y = y;
			prevZoneNumber = zone;
		}
	}

	// 내가 사용할 새로운 함수들은 여기에 정립해라
	
	// 카메라 좌표 설정
	CameraPos getCameraPos(void) { return camera; }
	void setCameraPos(int x, int y) { camera.x += x, camera.y += y; }
	CameraRange getCameraRange() { return cameraRange; }


	MapSize getMapSize(void) { return mapSize; }
	void settingMapMaxSize(int x, int y) { mapSize.x = x, mapSize.y = y; }

	void cameraOffset(void);

	bool getLockX() { return cameraLockX; }
	bool getLockY() { return cameraLockY; }
	
	void setMaxCameraRange(void);
	void setStage(BossType stage) { mBtype = stage; }

	void startAutoMove(float x, float y, bool useLerpY);
	void bossRoomOffest(void);

	bool getIsAutoMove() { return isAutoMove; }
	inline bool getIsCamaraMove() { return cameraMove; }
	inline bool getCameraMoveEnd() { return cameraMoveEnd; }

	string getZoneName() { return zoneName; }
	int getZoneNumber() { return zoneNumber; }
	int getZoneResetNumber() { return prevZoneNumber; }

	float cameraLerpY(float slopeStartX, float slopeEndX, float bottomStart, float bottomEnd);

	bool forceCameraMove(int targetPointLeft, int targetPointBottom, int targetRight, int targetTop);

	int getCameraX() { return camera.x; }

	inline void bossCameraMove(int left, int bottom, int right, int top)
	{
		locationTestX = left;
		locationTestRight = right;

		locationTestY = top;
		locationTestBottom = bottom;

		test = true;
		cameraMove = true;
	}


	CameraManager() {}
	~CameraManager() {}
};


