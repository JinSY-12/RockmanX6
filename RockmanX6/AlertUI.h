#pragma once
#include "UiBase.h"
#include "UiType.h"

class AlertUI : public UiBase
{
private:
	GImage* alertImage;

	bool noticeStart;
	bool playAble;

	float readyTimer;

	int prevFrame;
	int nextSprite;

	Position uiPos;

	UiType uType;

public:
	HRESULT init(UiType type);
	void release();
	void update();
	void render(HDC hdc);

	void Ready(void);
	void Warning();
};

