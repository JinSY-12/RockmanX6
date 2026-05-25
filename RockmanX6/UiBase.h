#pragma once
#include "PlayerType.h"
#include "BossType.h"

class UiBase
{
protected:
	struct Position
	{
		int x;
		int y;
	};

	bool isUiMode;
	bool uiDead;

protected:
	Position pos;

public:
	virtual HRESULT init(UiType uType);
	virtual HRESULT init(UiType uType, int sceneNum);
	virtual HRESULT init(int x, int y);
	virtual HRESULT init(PlayerType pType);
	virtual HRESULT init(BossType bType);
	virtual HRESULT init(PlayerType pType, BossType bType);

	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	bool getUiDead(void) { return uiDead; }
};

