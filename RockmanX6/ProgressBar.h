#pragma once
#include "PlayerType.h"
#include "BossType.h"

class ProgressBar
{
private:

	struct hpBar
	{
		// ¿ÃπÃ¡ˆ
		GImage* playerLogo;
		GImage* enemyLogo;

		GImage* progressBar;
		GImage* progressHead;
		GImage* mainGaugeBar;
		GImage* subGaugeBar;

		GImage* weaponNumber;

		float currentHp;
		float currentWeaponGauge;
		float prevHp;
		float currentMaxHp;
		int currentLife;
		int bossLevel;
		bool gameStart;
	};
	
	int charType;
	int bossType;

	hpBar playerHp;
	hpBar bossHp;
	

public:
	HRESULT init(PlayerType pType);
	HRESULT init(BossType bType, int maxHp);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void setCharacter(int character, int boss);
	void setPlayerVisible(bool isStart) { playerHp.gameStart = isStart; }
	bool getPlayerVisible(void) { return playerHp.gameStart; }

	void setBossVisible(bool isStart) { bossHp.gameStart = isStart; }
	bool getBossVisible(void) { return bossHp.gameStart; }

	void setPlayerFirstMaxHP(int maxHp)
	{
		playerHp.currentMaxHp = maxHp;
		playerHp.currentHp = playerHp.currentMaxHp;
		playerHp.prevHp = playerHp.currentMaxHp;
	}

	void setPlayerMaxHP(int maxHp) { playerHp.currentMaxHp = maxHp; }
	void setPlayerCurrentWeaponGauge(int weapon) { playerHp.currentWeaponGauge = weapon; }
	void setPlayerCurrentHp(int hp) { playerHp.currentHp = hp; }
	void setPlayerCurrentLife(int life) { playerHp.currentLife = life; }
	
	void setPlayerInfo(int hp, int maxHp, int weapon, int life);
	void updatePlayerInfop(int hp, int maxHp, int weapon, int life);

	void setBossInfo(int hp, int maxHp, int weapon, int life);
	void updateBossInfop(int hp, int maxHp, int weapon, int life);

	void reduecHp(int damage) { playerHp.currentHp -= damage; }

	ProgressBar()
	{
		playerHp.gameStart = false;
		bossHp.gameStart = false;
	}
	~ProgressBar() {}
};
