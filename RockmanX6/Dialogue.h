#pragma once
#include"UiBase.h"

class Dialogue : public UiBase
{
private:
	struct MovieEvent
	{
		int sceneNum;
		string movieName;

		string text;
		string bgm;
		string sfx;
	};

	struct DialogueEvent
	{
		int sceneNum;

		string text;
		string dialogueName;
	};

private:
	MovieEvent movie;
	DialogueEvent dialogue;

	UiType type;

	GImage* bg;
	GImage* left;
	GImage* right;

	GImage* textIcon;

	bool nextAlbe;
	float mTextDelay;

	int mCurrentLine;

	bool textBGOpen;
	bool faceOnOff;

public:
	HRESULT init(UiType uType, int sceneNum) override;

	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;

	void movieReady(void);
};

