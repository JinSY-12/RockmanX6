#include "Stdafx.h"
#include "UiManager.h"

HRESULT UiManager::init(void)
{
	mEventNum = 0;
	mCurrentLine = 0;
	isUiMode = false;
	mTextDelay = 0.f;
	nextAlbe = false;
	_textIcon = IMAGEMANAGER->findImage("Next");
	charType = -1;
	bossType - -1;
	isDebugMode = false;

	return S_OK;
}

void UiManager::update(void)
{
	if ((KEYMANAGER->isOnceKeyDown('X') || KEYMANAGER->isOnceKeyDown('C')) && isUiMode == true)
	{
		// ���ڰ� �� ��µǾ��� ���� ��� ���ð� �� ��ٸ���
		if (TEXTMANAGER->ReadComplete() == true && nextAlbe)
		{
			// ���� ��� ��� ���ð� �ʱ�ȭ, ���� �� ��� �� TEXTMANAGER���� ���� �� ����
			mTextDelay = TIMEMANAGER->getWorldTime();
			mCurrentLine++;
			TEXTMANAGER->ReadDialogue();
		}

		// ��� �� �ȵǾ����� ���� ��� ���ð��� �ٵǸ� ������ ����!!
		else if(TEXTMANAGER->ReadComplete() == false && nextAlbe) TEXTMANAGER->ReadEnd();
		// �ؿ� ���ð� ���̸� �� ���� ��ŵ ����

		if (TEXTMANAGER->EventComplete() == true) {
			isUiMode = false;
		}
		// �ؽ�Ʈ�� �� ��� �� �����̸�?? UI��� ����
		
	}
	
	// UI��尡 ���� ���¸�?? FadeOut
	// FadeOut�� ������? �� ����
	if (!isUiMode) CAMERAMANAGER->padeOut(3.0f);
	// if (CAMERAMANAGER->isPadeOutComplete());

	if (KEYMANAGER->isOnceKeyDown(VK_TAB)) isDebugMode = !isDebugMode;
	
	if (progressBar != nullptr) progressBar->update();

	_textIcon->play(0.5f);
}

void UiManager::release(void)
{

}

void UiManager::render(HDC hdc)
{
	if (isUiMode == true)
	{
		//TIMEMANAGER->getWorldTime() - mTextDelay ���� �ϴ� �ɷ� ��ȭ ��Ÿ �ӵ� ���� ����
		if (TIMEMANAGER->getWorldTime() - mTextDelay > 1.1f)
		{
			TEXTMANAGER->drawName(hdc, WINSIZE_X / 24, WINSIZE_Y * 0.65, mEventNum, mCurrentLine, "DNF_M_45");
			TEXTMANAGER->drawDialogue(hdc, WINSIZE_X / 16, WINSIZE_Y * 0.68, mEventNum, mCurrentLine, "DNF_M_34");
			nextAlbe = true;
		}
		else nextAlbe = false;

		if (TEXTMANAGER->showLine == true)
		{
			_textIcon->frameRender(hdc, WINSIZE_X / 2 - IMAGEMANAGER->findImage("Next")->getFrameWidth() / 2, WINSIZE_Y / 20 * 19);
		}
	}

	if (progressBar != nullptr) progressBar->render(hdc);
}

void UiManager::SettingProgressBar(PlayerType pType, BossType bType)
{
	progressBar = new ProgressBar;
	progressBar->init(pType, bType);
}


void UiManager::printEvent(int eventNum)
{
	chageIsUiMode();

	mEventNum = eventNum;
	mCurrentLine = 0;
	mTextDelay = TIMEMANAGER->getWorldTime();
}

