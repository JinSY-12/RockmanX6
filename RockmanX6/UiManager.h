#pragma once
#include "SingletonBase.h"
#include "ProgressBar.h"
#include "PlayerType.h"
#include "BossType.h"

class UiManager : public SingletonBase<UiManager>
{
private:
	GImage* _hud;
	GImage* _number;
	GImage* _heart;
	GImage* _mana;
	GImage* _dialogue;
	GImage* _textIcon;

	ProgressBar* progressBar;

	bool isUiMode;
	bool nextAlbe;

	int mEventNum;
	int mCurrentLine;

	float mTextDelay;

	bool isDebugMode;

	int charType;
	int bossType;


public:
	HRESULT init(void);
	void update(void);
	void release(void);
	void render(HDC hdc);

	void SettingProgressBar(PlayerType pType, BossType bType);

	inline int isCurrentLine() { return mCurrentLine; }

	bool getIsUiMode() { return isUiMode; }
	void changeUiMode() { isUiMode = !isUiMode; }

	inline void chageIsUiMode(void) { isUiMode = (isUiMode) ? false : true; }

	inline void changeNextable(void) { nextAlbe = true; }

	inline bool getIsDebugMode(void) { return isDebugMode; }

	inline void playStart() { progressBar->setVisible(true); }

	void setMaxHp(int maxHp);
	void setCurrentPlayerStatus(int hp, int mp, int maxHp, int life);
	void setLife(int currentlife);

	void printEvent(int eventNum);
};

