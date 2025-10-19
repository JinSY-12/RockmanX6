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

	GImage* weaponNumber;

	int charType;
	int bossType;

	float currentHp;
	float currentWeaponGauge;
	float prevHp;
	float currentMaxHp;

	bool gameStart;

	int currentLife;
	

public:
	HRESULT init(void);
	HRESULT init(PlayerType pType, BossType bType);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void setCharacter(int character, int boss);
	void setVisible(bool isStart) { gameStart = isStart; }
	bool getVisible(void) { return gameStart; }

	void setFirstMaxHP(int maxHp)
	{
		currentMaxHp = maxHp;
		currentHp = currentMaxHp;
		prevHp = currentMaxHp;
	}

	void setMaxHP(int maxHp) { currentMaxHp = maxHp; }
	void setCurrentWeaponGauge(int weapon) { currentWeaponGauge = weapon; }
	void setCurrentHp(int hp) { currentHp = hp; }
	void setCurrentLife(int life) { currentLife = life; }
	
	void reduecHp(int damage) { currentHp -= damage; }

	ProgressBar() { gameStart = false; }
	~ProgressBar() {}
};
