#pragma once
#include "SingletonBase.h"
#include "PlayerType.h"
#include "BossType.h"

#include "UiType.h"
#include "UiBase.h"
#include "AlertUI.h"
#include "ProgressBar.h"
#include "Dialogue.h"

class UiManager : public SingletonBase<UiManager>
{
private:
	struct PlayerInfo
	{
		int hp;
		int maxHp;
		int mp;
		int life;
	};

	GImage* _hud;
	GImage* _number;
	GImage* _heart;
	GImage* _mana;
	GImage* _dialogue;
	GImage* _textIcon;

	UiBase* Ui;

	bool isUiMode;
	bool isUiPrint;
	bool playerFreeze;
	bool nextAlbe;

	int mEventNum;
	int mCurrentLine;

	float mTextDelay;

	bool isDebugMode;

	int charType;
	int bossType;

	typedef vector<UiBase*> vUiVector;
	typedef vector<UiBase*>::iterator viUiVector;
		
	typedef vector<int> vEventNum;
private:
	vUiVector _vUi;
	
	vEventNum _vEventNum;

	PlayerType player;
	BossType boss;

	PlayerInfo pInfo;

public:
	HRESULT init(void);
	void update(void);
	void release(void);
	void render(HDC hdc);

	void addUi(UiType uType, int dialogueNum = 0);
	
	inline void SettingProgressBar(PlayerType pType, BossType bType) {
		player = pType;
		boss = bType;
	}

	inline int isCurrentLine() { return mCurrentLine; }
	inline void setCurrentLine(int line) { mCurrentLine = line; }

	void setIsUiMode(bool mode) { isUiMode = mode; }
	void setIsUiPrint(bool print) { isUiPrint = print; }
	void setFreeze(bool stop) { playerFreeze = stop; }

	bool getIsUiMode(void) { return isUiMode; }
	bool getIsUiPrint(void) { return isUiPrint; }
	bool getFreeze(void) { return playerFreeze; }
	
	void changeUiMode(void) { isUiMode = !isUiMode; }

	inline void chageIsUiMode(void) { isUiMode = (isUiMode) ? false : true; }

	inline void changeNextable(void) { nextAlbe = true; }

	inline bool getIsDebugMode(void) { return isDebugMode; }
	inline bool getUiNull(void)
	{
		if (_vUi.size() == 0) return true;
		else return false;
	}

	// inline void playStart() { progressBar->setVisible(true); }

	void printEvent(int eventNum);
};

