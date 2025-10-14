#pragma once
#include "GameNode.h"

class TitleScene : public GameNode
{
private:

	struct Button
	{
		int x, y;
		bool dark;
	}button[5];
	
	bool insert;
	int buttonSelect;

	float delayTimer;

	float mSelectTimer;
	float mMoveTimer;

	GImage* _image;

	typedef vector<GImage*> vButton;
	typedef vector<GImage*>::iterator viButton;

private:
	vButton _vButton;
	viButton _viButton;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void addImage(void);
	void addSound(void);
	void settingScene(void);

	void loadStat(void);

	TitleScene() {}
	~TitleScene() {}
};

