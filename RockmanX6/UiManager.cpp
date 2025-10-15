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
		// 글자가 다 출력되었고 다음 출력 대기시간 다 기다리면
		if (TEXTMANAGER->ReadComplete() == true && nextAlbe)
		{
			// 다음 대사 출력 대기시간 초기화, 다음 줄 출력 및 TEXTMANAGER에서 다음 줄 세팅
			mTextDelay = TIMEMANAGER->getWorldTime();
			mCurrentLine++;
			TEXTMANAGER->ReadDialogue();
		}

		// 출력 다 안되었지만 다음 출력 대기시간이 다되면 끝으로 슈웅!!
		else if(TEXTMANAGER->ReadComplete() == false && nextAlbe) TEXTMANAGER->ReadEnd();
		// 밑에 대기시간 줄이면 더 빠른 스킵 가능

		if (TEXTMANAGER->EventComplete() == true) {
			isUiMode = false;
		}
		// 텍스트가 다 출력 된 상태이면?? UI모드 해제
		
	}
	
	// UI모드가 꺼진 상태면?? FadeOut
	// FadeOut이 끝나면? 씬 변경
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
		//TIMEMANAGER->getWorldTime() - mTextDelay 조절 하는 걸로 대화 연타 속도 조절 가능
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

