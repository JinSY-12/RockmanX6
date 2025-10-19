#include "Stdafx.h"
#include "ProgressBar.h"

HRESULT ProgressBar::init(void)
{
    // Do Nothing!

    return S_OK;
}

HRESULT ProgressBar::init(PlayerType pType, BossType bType)
{
    // ü�¹� �⺻ ����
    progressBar = IMAGEMANAGER->findImage("HUD_HpBar");
    progressHead = IMAGEMANAGER->findImage("HUD_HpBarHead");
    mainGaugeBar = IMAGEMANAGER->findImage("HUD_GreenBar");
    subGaugeBar = IMAGEMANAGER->findImage("HUD_RedBar");
    weaponNumber = IMAGEMANAGER->findImage("HUD_Number");

    // ĳ���Ϳ� �´� ĳ���� ������ ����
    switch (pType)
    {
    case PlayerType::X:
        playerLogo = IMAGEMANAGER->findImage("HUD_ProgressBar_X");
        break;
    }

    // ������ �´� ���� ������ ����
    switch (bType)
    {
    case BossType::Intro:
        break;
    case BossType::CommanYanmark:
        break;
    }
    
    gameStart = false;    

    return S_OK;
}

void ProgressBar::release(void)
{
}

void ProgressBar::update(void)
{
    if (prevHp > currentHp) prevHp -= 0.2f;
}

void ProgressBar::render(HDC hdc)
{
    if (charType != -1 && gameStart == true)
    {
        playerLogo->render(hdc, WINSIZE_X / 100 * 3, WINSIZE_Y / 100 * 34);
        progressBar->render(hdc, WINSIZE_X / 100 * 3 + 7 * SCALE_FACTOR , WINSIZE_Y / 100 * 14 - ((int)currentMaxHp - 49) * SCALE_FACTOR,  0, 0, progressBar->getWidth(), ((int)currentMaxHp -2) * SCALE_FACTOR);
        progressHead->render(hdc, WINSIZE_X / 100 * 3 + 7 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)currentMaxHp - 45) * SCALE_FACTOR);
        subGaugeBar->render(hdc, WINSIZE_X / 100 * 3 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)prevHp - 47) * SCALE_FACTOR, 0, 0, subGaugeBar->getWidth(), (int)prevHp * SCALE_FACTOR);
        mainGaugeBar->render(hdc, WINSIZE_X / 100 * 3 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)currentHp - 47) * SCALE_FACTOR, 0, 0, mainGaugeBar->getWidth(), (int)currentHp * SCALE_FACTOR);

        weaponNumber->render(hdc, WINSIZE_X / 100 * 7, WINSIZE_Y / 100 * 41,((currentLife / 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, weaponNumber->getHeight());
        weaponNumber->render(hdc, WINSIZE_X / 100 * 7 + 5 * SCALE_FACTOR, WINSIZE_Y / 100 * 41, ((currentLife % 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, weaponNumber->getHeight());

        // 1�� �ڸ��� (currentLife % 10)
    }
}

void ProgressBar::setCharacter(int character, int boss)
{
    // ���� �÷��̾��� ������ ��� �ð�
    // �ִ� ü��, ��� ���� ���
}
