#pragma once
#include "PlayerType.h"
#include "BossType.h"

class ProgressBar
{
private:
	GImage* playerLogo;
	GImage* enemyLogo;

	GImage* progressBar;
	GImage* progressHead;
	GImage* mainGaugeBar;
	GImage* subGaugeBar;

	int charType;
	int bossType;

	float currentHp;
	float prevHp;
	float currentMaxHp;

	bool gameStart;

public:
	HRESULT init(void);
	HRESULT init(PlayerType pType, BossType bType);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void setCharacter(int character, int boss);
	void setVisible(bool isStart) { gameStart = isStart; }
	bool getVisible(void) { return gameStart; }
	void setMaxHP(int maxHp) { currentMaxHp = maxHp;}
	void reduecHp(int damage) { currentHp -= damage; }

	ProgressBar() { gameStart = false; }
	~ProgressBar() {}
};
