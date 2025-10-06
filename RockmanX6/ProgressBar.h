#pragma once

class ProgressBar
{
private:
	GImage* playerLogo;
	GImage* enemyLogo;

	GImage* ProgressBar;
	GImage* ProgressHead;
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
	HRESULT init(int num);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void setCharacter(int character, int boss);
	void setVisible(bool isStart) { gameStart = isStart; }
	void setMaxHP(int maxHp) { currentMaxHp = maxHp;}
	void reduecHp(int damage) { currentHp -= damage; }
};

/*
	GImage* playerLogo;
	GImage* enemyLogo;

	GImage* ProgressBar;
	GImage* mainGaugeBar;
	GImage* subGaugeBar;
*/
