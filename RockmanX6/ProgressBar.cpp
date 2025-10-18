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

    // ü�� ���� �׽�Ʈ - ü�� ȸ��
    if (KEYMANAGER->isOnceKeyDown('F'))
    {
        currentHp += 10.0f;
        prevHp += 10.0f;

        currentHp = min(currentHp, currentMaxHp);
        prevHp = min(currentHp, currentMaxHp);
    }

    // ü�� ���� �׽�Ʈ - ü�� ���� �ִϸ��̼�
    if (KEYMANAGER->isOnceKeyDown('G'))
    {
        currentHp -= 10.0f;
        if (currentHp < 0.0f) currentHp = 0.0f;
    }
    
    // �ִ� ü�� ���� �׽�Ʈ - ��Ʈ ����
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
        progressBar->render(hdc, WINSIZE_X / 30 + 7 * SCALE_FACTOR , WINSIZE_Y / 50 * 7 - ((int)currentMaxHp - 49) * SCALE_FACTOR,  0, 0, progressBar->getWidth(), ((int)currentMaxHp -2) * SCALE_FACTOR);
        progressHead->render(hdc, WINSIZE_X / 30 + 7 * SCALE_FACTOR, WINSIZE_Y / 50 * 7 - ((int)currentMaxHp - 45) * SCALE_FACTOR);

        subGaugeBar->render(hdc, WINSIZE_X / 30 + 10 * SCALE_FACTOR, WINSIZE_Y / 50 * 7 - ((int)prevHp - 47) * SCALE_FACTOR, 0, 0, mainGaugeBar->getWidth(), (int)prevHp * SCALE_FACTOR);
        mainGaugeBar->render(hdc, WINSIZE_X / 30 + 10 * SCALE_FACTOR, WINSIZE_Y / 50 * 7 - ((int)currentHp - 47) * SCALE_FACTOR, 0, 0, mainGaugeBar->getWidth(), (int)currentHp * SCALE_FACTOR);
    }
}

void ProgressBar::setCharacter(int character, int boss)
{
    // ���� �÷��̾��� ������ ��� �ð�
    // �ִ� ü��, ��� ���� ���
}
