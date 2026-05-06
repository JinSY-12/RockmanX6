#include "Stdafx.h"
#include "ProgressBar.h"

HRESULT ProgressBar::init(PlayerType pType)
{
    progressBar = IMAGEMANAGER->findImage("HUD_HpBar");
    progressHead = IMAGEMANAGER->findImage("HUD_HpBarHead");
    mainGaugeBar = IMAGEMANAGER->findImage("HUD_GreenBar");
    subGaugeBar = IMAGEMANAGER->findImage("HUD_RedBar");
    weaponNumber = IMAGEMANAGER->findImage("HUD_Number");

    switch (pType)
    {
    case PlayerType::X:
    case PlayerType::Palcon:
    case PlayerType::Blade:
    case PlayerType::Shadow:
        playerLogo = IMAGEMANAGER->findImage("HUD_ProgressBar_X");
        break;
    }

    gameStart = false;

    return S_OK;
}

HRESULT ProgressBar::init(BossType bType)
{
    progressBar = IMAGEMANAGER->findImage("HUD_HpBar");
    progressHead = IMAGEMANAGER->findImage("HUD_HpBarHead");
    mainGaugeBar = IMAGEMANAGER->findImage("HUD_GreenBar");
    subGaugeBar = IMAGEMANAGER->findImage("HUD_RedBar");
    weaponNumber = IMAGEMANAGER->findImage("HUD_Number");

    switch (bType)
    {
    case BossType::Intro:
        break;
    case BossType::CommanYanmark:
        break;
    }

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
    if (gameStart == true)
    {
        playerLogo->render(hdc, WINSIZE_X / 100 * 3, WINSIZE_Y / 100 * 34);
        progressBar->render(hdc, WINSIZE_X / 100 * 3 + 7 * SCALE_FACTOR , WINSIZE_Y / 100 * 14 - ((int)currentMaxHp - 49) * SCALE_FACTOR,  0, 0, progressBar->getWidth(), ((int)currentMaxHp -2) * SCALE_FACTOR);
        progressHead->render(hdc, WINSIZE_X / 100 * 3 + 7 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)currentMaxHp - 45) * SCALE_FACTOR);
        subGaugeBar->render(hdc, WINSIZE_X / 100 * 3 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)prevHp - 47) * SCALE_FACTOR, 0, 0, subGaugeBar->getWidth(), (int)prevHp * SCALE_FACTOR);
        mainGaugeBar->render(hdc, WINSIZE_X / 100 * 3 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)currentHp - 47) * SCALE_FACTOR, 0, 0, mainGaugeBar->getWidth(), (int)currentHp * SCALE_FACTOR);

        weaponNumber->render(hdc, WINSIZE_X / 100 * 7, WINSIZE_Y / 100 * 41,((currentLife / 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, weaponNumber->getHeight());
        weaponNumber->render(hdc, WINSIZE_X / 100 * 7 + 5 * SCALE_FACTOR, WINSIZE_Y / 100 * 41, ((currentLife % 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, weaponNumber->getHeight());

        // 1의 자리는 (currentLife % 10)
    }
}

void ProgressBar::setCharacter(int character, int boss)
{
    // 대충 플레이어의 스텟을 들고 올것
    // 최대 체력, 목숩 갯수 등등
}

void ProgressBar::setPlayerInfo(int hp, int maxHp, int weapon, int life)
{
    currentHp = prevHp = currentMaxHp = maxHp;
    currentWeaponGauge = weapon;
    currentLife = life;
}

void ProgressBar::updatePlayerInfop(int hp, int maxHp, int weapon, int life)
{
    currentHp = hp;
    currentMaxHp = maxHp;
    currentWeaponGauge = weapon;
    currentLife = life;
}
