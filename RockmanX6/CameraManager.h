#pragma once
#include "SingletonBase.h"
#include "BossType.h"

class CameraManager : public SingletonBase<CameraManager>
{
private:

#pragma region ��ǥ����
	
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
	
	// ī�޶� ��ǥ
	CameraPos camera;
	MapSize mapSize;
	PlayerPos playerPos;
	CameraRange cameraRange;

	// ���̵� ��/�ƿ� ����
	GImage* _blackImage;
	GImage* _whiteImage;

	float _blackAlpha;					// ī�޶� ȭ�� ���İ�
	float _whiteAlpha;					// ī�޶� ȭ�� ���İ�

	float _padeTime;					// ī�޶� ���̵� ��/�ƿ� ���ӽð�

	bool _isBlackPadeIn;				// ī�޶� ���̵���
	bool _isBlackPadeOut;				// ī�޶� ���̵�ƿ�
	
	bool _isWhitePadeIn;				// ī�޶� ���̵���
	bool _isWhitePadeOut;				// ī�޶� ���̵�ƿ�

	bool _isPadeResult;					// ī�޶� ���̵� ���

	// �� Ŭ���� ���� ����
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
		
	// ī�޶� ��ġ ���� - �̵� / ����
	
	void fixPos(int x, int y) { camera.x = x, camera.y = y; }

	bool isPadeOutComplete() { return _isPadeResult; }

	// �÷��̾� ��ġ üũ
	PlayerPos getPlayerPos(void) { return playerPos; }
	void setPlayerPos(int x, int y) { playerPos.x = x, playerPos.y = y; }

	// ���� ����� ���ο� �Լ����� ���⿡ �����ض�
	
	// ī�޶� ��ǥ ����
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


	// ī�޶� �ʱⰪ -> ���߿� ��ũ ħ�뿡�� �����ϴ°ɷ� ī�޶� ��ǥ ����
	// ��ũ �Ͽ콺 �Ա�
	// CameraManager() { camera.x = -255, camera.y = -480; }
	// ��ũ�� ��
	//CameraManager() { camera.x = 100, camera.y = 100; }

	CameraManager() {}
	~CameraManager() {}
};


