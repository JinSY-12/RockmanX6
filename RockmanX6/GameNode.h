#pragma once
#include"Gimage.h"
#include "PlayerType.h"
#include "BossType.h"
#include <Windows.h>

static GImage* _backBuffer = IMAGEMANAGER->addImage("backBueffer", WINSIZE_X, WINSIZE_Y);

class GameNode
{
private:
	HDC _hdc;
	bool _managerInit;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int num);
	virtual HRESULT init(int x, int y);
	virtual HRESULT init(PlayerType pType, BossType bType);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void alphaRender(void);
	virtual void objectRender(void);

	GImage* getBackBuffer(void) { return _backBuffer; }

	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	GameNode() {}
	virtual ~GameNode() {}
};

