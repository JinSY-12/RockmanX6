#include "Stdafx.h"
#include "ProgressBar.h"

HRESULT ProgressBar::init(PlayerType pType)
{
    playerHp.progressBar = IMAGEMANAGER->findImage("HUD_HpBar");
    playerHp.progressHead = IMAGEMANAGER->findImage("HUD_HpBarHead");
    playerHp.mainGaugeBar = IMAGEMANAGER->findImage("HUD_GreenBar");
    playerHp.subGaugeBar = IMAGEMANAGER->findImage("HUD_RedBar");
    playerHp.weaponNumber = IMAGEMANAGER->findImage("HUD_Number");

    switch (pType)
    {
    case PlayerType::X:
    case PlayerType::Palcon:
    case PlayerType::Blade:
    case PlayerType::Shadow:
        playerHp.playerLogo = IMAGEMANAGER->findImage("HUD_ProgressBar_X");
        break;
    }

    playerHp.gameStart = false;

    return S_OK;
}

HRESULT ProgressBar::init(BossType bType, int maxHp)
{
    bossHp.progressBar = IMAGEMANAGER->findImage("HUD_HpBar");
    bossHp.progressHead = IMAGEMANAGER->findImage("HUD_HpBarHead");
    bossHp.mainGaugeBar = IMAGEMANAGER->findImage("HUD_GreenBar");
    bossHp.subGaugeBar = IMAGEMANAGER->findImage("HUD_RedBar");
    bossHp.weaponNumber = IMAGEMANAGER->findImage("HUD_Number");

    switch (bType)
    {
    case BossType::Intro:
        bossHp.playerLogo = IMAGEMANAGER->findImage("HUD_ProgressBar_HighMax");
        break;
    case BossType::CommanYanmark:
        break;
    }

    bossHp.currentMaxHp = maxHp;

    return S_OK;
}

void ProgressBar::release(void)
{
}

void ProgressBar::update(void)
{
    if (playerHp.prevHp > playerHp.currentHp) playerHp.prevHp -= 0.2f;
    if (bossHp.prevHp > bossHp.currentHp) bossHp.prevHp -= 0.2f;
}

void ProgressBar::render(HDC hdc)
{
    if (playerHp.gameStart == true)
    {
        playerHp.playerLogo->render(hdc, WINSIZE_X / 100 * 3, WINSIZE_Y / 100 * 34);
        playerHp.progressBar->render(hdc, WINSIZE_X / 100 * 3 + 7 * SCALE_FACTOR , WINSIZE_Y / 100 * 14 - ((int)playerHp.currentMaxHp - 49) * SCALE_FACTOR,  0, 0, playerHp.progressBar->getWidth(), ((int)playerHp.currentMaxHp -2) * SCALE_FACTOR);
        playerHp.progressHead->render(hdc, WINSIZE_X / 100 * 3 + 7 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)playerHp.currentMaxHp - 45) * SCALE_FACTOR);
        playerHp.subGaugeBar->render(hdc, WINSIZE_X / 100 * 3 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)playerHp.prevHp - 47) * SCALE_FACTOR, 0, 0, playerHp.subGaugeBar->getWidth(), (int)playerHp.prevHp * SCALE_FACTOR);
        playerHp.mainGaugeBar->render(hdc, WINSIZE_X / 100 * 3 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)playerHp.currentHp - 47) * SCALE_FACTOR, 0, 0, playerHp.mainGaugeBar->getWidth(), (int)playerHp.currentHp * SCALE_FACTOR);

        playerHp.weaponNumber->render(hdc, WINSIZE_X / 100 * 7, WINSIZE_Y / 100 * 41,((playerHp.currentLife / 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, playerHp.weaponNumber->getHeight());
        playerHp.weaponNumber->render(hdc, WINSIZE_X / 100 * 7 + 5 * SCALE_FACTOR, WINSIZE_Y / 100 * 41, ((playerHp.currentLife % 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, playerHp.weaponNumber->getHeight());

        // 1의 자리는 (currentLife % 10)
    }

    if (bossHp.gameStart == true)
    {
        bossHp.playerLogo->render(hdc, WINSIZE_X / 100 * 97, WINSIZE_Y / 100 * 34 + (bossHp.currentMaxHp - 30));
        bossHp.progressBar->render(hdc, WINSIZE_X / 100 * 97 + 7 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)bossHp.currentMaxHp - 49) * SCALE_FACTOR + (bossHp.currentMaxHp - 30), 0, 0, bossHp.progressBar->getWidth(), ((int)bossHp.currentMaxHp - 2) * SCALE_FACTOR);
        bossHp.progressHead->render(hdc, WINSIZE_X / 100 * 97 + 7 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)bossHp.currentMaxHp - 45) * SCALE_FACTOR + (bossHp.currentMaxHp - 30));
        bossHp.subGaugeBar->render(hdc, WINSIZE_X / 100 * 97 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)bossHp.prevHp - 47) * SCALE_FACTOR + (bossHp.currentMaxHp - 30), 0, 0, bossHp.subGaugeBar->getWidth(), (int)bossHp.prevHp * SCALE_FACTOR);
        bossHp.mainGaugeBar->render(hdc, WINSIZE_X / 100 * 97 + 10 * SCALE_FACTOR, WINSIZE_Y / 100 * 14 - ((int)bossHp.currentHp - 47) * SCALE_FACTOR + (bossHp.currentMaxHp - 30), 0, 0, bossHp.mainGaugeBar->getWidth(), (int)bossHp.currentHp * SCALE_FACTOR);
  
        bossHp.weaponNumber->render(hdc, WINSIZE_X / 100 * 97 + 15 * SCALE_FACTOR, WINSIZE_Y / 100 * 41 + (bossHp.currentMaxHp - 30), ((bossHp.bossLevel / 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, bossHp.weaponNumber->getHeight());
        bossHp.weaponNumber->render(hdc, WINSIZE_X / 100 * 97 + 19 * SCALE_FACTOR, WINSIZE_Y / 100 * 41 + (bossHp.currentMaxHp - 30), ((bossHp.bossLevel % 10)) * 3 * SCALE_FACTOR, 0, 3 * SCALE_FACTOR, bossHp.weaponNumber->getHeight());
    }
}

void ProgressBar::setCharacter(int character, int boss)
{
    // 대충 플레이어의 스텟을 들고 올것
    // 최대 체력, 목숩 갯수 등등
}

void ProgressBar::setPlayerInfo(int hp, int maxHp, int weapon, int life)
{
    playerHp.currentHp = playerHp.prevHp = playerHp.currentMaxHp = maxHp;
    playerHp.currentWeaponGauge = weapon;
    playerHp.currentLife = life;
}

void ProgressBar::updatePlayerInfop(int hp, int maxHp, int weapon, int life)
{
    playerHp.currentHp = hp;
    playerHp.currentMaxHp = maxHp;
    playerHp.currentWeaponGauge = weapon;
    playerHp.currentLife = life;
}

void ProgressBar::setBossInfo(int hp, int maxHp, int weapon, int life)
{
    bossHp.currentHp = bossHp.prevHp = bossHp.currentMaxHp = maxHp;
    bossHp.currentWeaponGauge = weapon;
    bossHp.bossLevel = life;
}

void ProgressBar::updateBossInfop(int hp, int maxHp, int weapon, int life)
{
    bossHp.currentHp = hp;
    bossHp.currentMaxHp = maxHp;
    bossHp.currentWeaponGauge = weapon;
    bossHp.bossLevel = life;
}
