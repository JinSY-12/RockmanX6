#include "Stdafx.h"
#include "ProgressBar.h"

HRESULT ProgressBar::init(void)
{
    // Do Nothing!

    return S_OK;
}

HRESULT ProgressBar::init(int num)
{
    charType = num;
    gameStart = false;

    currentMaxHp = 30.f;
    prevHp = currentMaxHp;
    currentHp = currentMaxHp;

    return S_OK;
}

void ProgressBar::release(void)
{
}

void ProgressBar::update(void)
{
    if (prevHp > currentHp) prevHp -= 0.2f;

    // 체력 증가 테스트 - 체력 회복
    if (KEYMANAGER->isOnceKeyDown('F'))
    {
        currentHp += 10.0f;
        prevHp += 10.0f;

        currentHp = min(currentHp, currentMaxHp);
        prevHp = min(currentHp, currentMaxHp);
    }

    // 체력 감소 테스트 - 체력 감소 애니메이션
    if (KEYMANAGER->isOnceKeyDown('G'))
    {
        currentHp -= 10.0f;
        if (currentHp < 0.0f) currentHp = 0.0f;
    }
    
    // 최대 체력 증가 테스트 - 하트 습득
    if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
    {
        currentMaxHp += 2.0f;
        currentHp += 2.0f;
        prevHp += 2.0f;
    }
}

void ProgressBar::render(HDC hdc)
{
    if (charType != -1 && gameStart == true)
    {
        playerLogo->render(hdc, WINSIZE_X / 30, WINSIZE_Y / 50 * 17);
        ProgressBar->render(hdc, WINSIZE_X / 30 + 7 * SCALE_FACTOR , WINSIZE_Y / 50 * 7 - ((int)currentMaxHp - 49) * SCALE_FACTOR,  0, 0, ProgressBar->getWidth(), ((int)currentMaxHp -2) * SCALE_FACTOR);
        ProgressHead->render(hdc, WINSIZE_X / 30 + 7 * SCALE_FACTOR, WINSIZE_Y / 50 * 7 - ((int)currentMaxHp - 45) * SCALE_FACTOR);

        subGaugeBar->render(hdc, WINSIZE_X / 30 + 10 * SCALE_FACTOR, WINSIZE_Y / 50 * 7 - ((int)prevHp - 47) * SCALE_FACTOR, 0, 0, mainGaugeBar->getWidth(), (int)prevHp * SCALE_FACTOR);
        mainGaugeBar->render(hdc, WINSIZE_X / 30 + 10 * SCALE_FACTOR, WINSIZE_Y / 50 * 7 - ((int)currentHp - 47) * SCALE_FACTOR, 0, 0, mainGaugeBar->getWidth(), (int)currentHp * SCALE_FACTOR);
    }
}

void ProgressBar::setCharacter(int character, int boss)
{
    charType = character;
    bossType = boss;

    ProgressBar = IMAGEMANAGER->findImage("HUD_HpBar");
    ProgressHead = IMAGEMANAGER->findImage("HUD_HpBarHead");
    mainGaugeBar = IMAGEMANAGER->findImage("HUD_GreenBar");
    subGaugeBar = IMAGEMANAGER->findImage("HUD_RedBar");

    switch (charType)
    {
    // 엑스
    case 0:
        playerLogo = IMAGEMANAGER->findImage("HUD_ProgressBar_X");
        break;
    // 엑스 웨폰
    case 1:
        playerLogo = IMAGEMANAGER->findImage("HUD_ProgressBar_Xweapon");
        break;
    // 제로
    case 2:
        break;
    // 제로 웨폰
    case 3:
        break;
    }

    switch (bossType)
    {
        // 인트로 보스
    case 0:
        // 대충 보석 모양 보스 로고
        break;
        // 커맨드 얀마크
    case 1:
        break;
    }

}
