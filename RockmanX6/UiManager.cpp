#include "Stdafx.h"
#include "UiManager.h"

HRESULT UiManager::init(void)
{
	mEventNum = 0;
	mCurrentLine = 0;
	isUiMode = false;
	mTextDelay = 0.0f;
	nextAlbe = false;
	_textIcon = IMAGEMANAGER->findImage("Next");
	charType = -1;
	bossType - -1;
	isDebugMode = false;
	isUiPrint = false;

	return S_OK;
}

void UiManager::update(void)
{
	for (auto UIs = _vUi.begin(); UIs != _vUi.end();)
	{
		(*UIs)->update();
		
		if ((*UIs)->getUiDead() == true)
		{
			UIs = _vUi.erase(UIs);
			if (_vUi.size() == 0) isUiPrint = false;
		}

		else ++UIs;
	}
	
	if (KEYMANAGER->isOnceKeyDown(VK_TAB)) isDebugMode = !isDebugMode;
}

void UiManager::release(void)
{

}

void UiManager::render(HDC hdc)
{
	for (auto UIs = _vUi.begin(); UIs != _vUi.end(); ++UIs)
	{
		(*UIs)->render(hdc);
	}
}

void UiManager::addUi(UiType uType, int dialogueNum)
{
	isUiPrint = true;

	switch(uType)
	{
	case UiType::Ready:
		Ui = new AlertUI;
		Ui->init(uType);
		_vUi.push_back(Ui);
		break;
	case UiType::Warning:
		Ui = new AlertUI;
		Ui->init(uType);
		_vUi.push_back(Ui);
		break;
	case UiType::MoiveDialogue:
		Ui = new Dialogue;
		Ui->init(uType, dialogueNum);
		_vUi.push_back(Ui);
		break;
	case UiType::EventDialogue:
		bool save = false;
		if (_vEventNum.empty())
		{
			Ui = new Dialogue;
			Ui->init(uType, dialogueNum);
			_vEventNum.push_back(dialogueNum);
			_vUi.push_back(Ui);
		}
		else
		{
			for (auto num = _vEventNum.begin(); num != _vEventNum.end(); ++num)
			{
				if (*num != dialogueNum);
				else save = true;
			}

			if (!save)
			{
				Ui = new Dialogue;
				Ui->init(uType, dialogueNum);
				_vEventNum.push_back(dialogueNum);
				_vUi.push_back(Ui);
			}

			else
			{
				isUiPrint = false;
			}
		}
		break;
	}
}

void UiManager::printEvent(int eventNum)
{
	// chageIsUiMode();

	mEventNum = eventNum;
	mCurrentLine = 0;
	mTextDelay = TIMEMANAGER->getWorldTime();
}

