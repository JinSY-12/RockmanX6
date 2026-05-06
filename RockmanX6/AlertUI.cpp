#include "Stdafx.h"
#include "AlertUI.h"

HRESULT AlertUI::init(UiType type)
{
	switch (type)
	{
	case UiType::Ready:
		// 레디 이미지 추가
		alertImage = IMAGEMANAGER->findImage("Ui_Ready")->cloneImage();
		uiPos.x = (WINSIZE_X - alertImage->getFrameWidth()) / 2;
		uiPos.y = (WINSIZE_Y - alertImage->getFrameHeight()) / 2;
		
		readyTimer = TIMEMANAGER->getWorldTime();
		uType = type;

		break;

	case UiType::Warning:
		// 워닝 이미지 추가
		break;
	}

	playOnce = false;
	uiDead = false;
	return S_OK;
}

void AlertUI::release()
{

}


void AlertUI::update()
{
	switch (uType)
	{
	case UiType::Ready:
		Ready();
		break;

	case UiType::Warning:
		Warning();
		break;
	}
	
}


void AlertUI::render(HDC hdc)
{
	alertImage->frameRender(hdc, uiPos.x, uiPos.y, alertImage->getFrameX(), 0);
	// if(isUiMode) 
}

void AlertUI::Ready(void)
{
	if (!playOnce)
	{
		if (TIMEMANAGER->getWorldTime() - readyTimer >= 1.5f)
		{
			alertImage->play(0.025f);
		}

		if (alertImage->getFrameX() == 3)
		{
			SOUNDMANAGER->play("SFX_Ready", 0.5f);
			readyTimer = TIMEMANAGER->getWorldTime();
			playOnce = true;
		}
	}

	else
	{
		if (TIMEMANAGER->getWorldTime() - readyTimer >= 1.5f)
		{
			alertImage->play(0.025f);
		}
	}

	// 이미지 애니메이션 쫘라락
	if (alertImage->getChangeReady()) uiDead = true;
}

void AlertUI::Warning()
{
	uiDead = true;
}
