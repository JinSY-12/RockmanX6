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

	float _blackAlpha;					// ī�޶� ȭ�� ���İ�
	float _whiteAlpha;					// ī�޶� ȭ�� ���İ�

	float _padeTime;				// ī�޶� ���̵� ��/�ƿ� ���ӽð�

	bool _isBlackPadeIn;					// ī�޶� ���̵���
	bool _isBlackPadeOut;				// ī�޶� ���̵�ƿ�
	
	bool _isWhitePadeIn;
	bool _isWhitePadeOut;

	bool _isPadeResult;				// ī�޶� ���̵� ���

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

	// ī�޶� ��ġ ���� - �̵� / ����
	Pos getPos(void) { return camera; }
	void setPos(int x, int y) { camera.x += x, camera.y += y; }
	void fixPos(int x, int y) { camera.x = x, camera.y = y; }

	bool isPadeOutComplete() { return _isPadeResult; }

	// �÷��̾� ��ġ üũ
	PlayerPos getPlayerPos(void) { return playerPos; }
	void setPlayerPos(int x, int y) { playerPos.x = x, playerPos.y = y; }

	// ī�޶� �ʱⰪ -> ���߿� ��ũ ħ�뿡�� �����ϴ°ɷ� ī�޶� ��ǥ ����
	// ��ũ �Ͽ콺 �Ա�
	// CameraManager() { camera.x = -255, camera.y = -480; }
	// ��ũ�� ��
	//CameraManager() { camera.x = 100, camera.y = 100; }
	CameraManager() { camera.x = 0, camera.y = 960 * 3 - 768; }
	~CameraManager() {}
};

