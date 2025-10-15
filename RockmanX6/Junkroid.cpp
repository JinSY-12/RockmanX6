#include "Stdafx.h"
#include "Junkroid.h"

HRESULT Junkroid::init(void)
{
    return S_OK;
}

HRESULT Junkroid::init(int x, int y)
{
    eStatus.maxHp = 4;
    eStatus.hp = eStatus.maxHp;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_Junkroid");

    eStatus.width;
    eStatus.height;

    eStatus.eHitBox ;
    eStatus.attSight ;

    patternTimer = 0.0f;
    maxPatternTime = 3.0f;

    eStatus.isAtt = false;

    return S_OK;
}

void Junkroid::release(void)
{
}

void Junkroid::update(void)
{
    eStatus.eImage->play(0.07f);
}

void Junkroid::render(void)
{
}

void Junkroid::attack(void)
{
    // bManager->fire(BulletType::Buster);
}
