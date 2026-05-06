#pragma once
#include "UiBase.h"
#include "UiType.h"

class AlertUI : public UiBase
{
private:
	GImage* alertImage;

	bool noticeStart;
	float readyTimer;
	bool playOnce;

	bool playAble;

	bool soundOnce;

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

