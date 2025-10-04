#pragma once
#include "SingletonBase.h"

class UiManager : public SingletonBase<UiManager>
{
private:

	GImage* _hud;
	GImage* _number;
	GImage* _heart;
	GImage* _mana;
	GImage* _dialogue;
	GImage* _textIcon;


	GImage* playerLogo;
	GImage* enemyLogo;

	GImage* middleHpBar;
	GImage* topHpBar;

	GImage* middleMpBar;
	GImage* topMpBar;

	bool isUiMode;
	bool nextAlbe;

	int mEventNum;
	int mCurrentLine;

	float mTextDelay;

	bool isDebugMode;

public:
	HRESULT init(void);
	void update(void);
	void release(void);
	void render(HDC hdc);

	inline int isCurrentLine() { return mCurrentLine; }

	bool getIsUiMode() { return isUiMode; }
	void changeUiMode() { isUiMode = !isUiMode; }

	inline void chageIsUiMode(void) { isUiMode = (isUiMode) ? false : true; }

	inline void changeNextable(void) { nextAlbe = true; }

	inline bool getIsDebugMode(void) { return isDebugMode; }

	void printEvent(int eventNum);
};

