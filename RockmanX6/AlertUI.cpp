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
		noticeStart = false;

		break;

	case UiType::Warning:
		// 워닝 이미지 추가
		alertImage = IMAGEMANAGER->findImage("Ui_Warning0");
		uiPos.x = (WINSIZE_X - alertImage->getFrameWidth()) / 2;
		uiPos.y = WINSIZE_Y / 8;

		SOUNDMANAGER->play("SFX_Warning", 0.5f);
		nextSprite = 0;

		noticeStart = true;
		break;
	}

	uType = type;
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
	if (noticeStart) alertImage->frameRender(hdc, uiPos.x, uiPos.y, alertImage->getFrameX(), 0);
}

void AlertUI::Ready(void)
{
	// 정지
	if (TIMEMANAGER->getWorldTime() - readyTimer >= 1.5f)
	{
		alertImage->play(0.02f);
		noticeStart = true;
	}

	// 각 프레임마다 효과 추가
	if (prevFrame != alertImage->getFrameX())
	{
		if (alertImage->getFrameX() == 3) SOUNDMANAGER->play("SFX_Ready", 0.5f);
		else if (alertImage->getFrameX() == 7) readyTimer = TIMEMANAGER->getWorldTime();		
	}

	// 애니메이션 끝나면 삭제 처리 요청
	if (alertImage->getChangeReady()) uiDead = true;
	
	// 프레임 변화 확인용
	prevFrame = alertImage->getFrameX();
}

void AlertUI::Warning()
{
	alertImage->play(0.04f);

	if (alertImage->getChangeReady())
	{
		alertImage->setChangeReady(false);
		alertImage->setFrameX(0);
		if (nextSprite < 3)
		{
			nextSprite++;
			alertImage = IMAGEMANAGER->findImage("Ui_Warning" + to_string(nextSprite));
		}

		else
		{
			cout << "??" << endl;
			uiDead = true;
			nextSprite = 0;
		}
	}
}
