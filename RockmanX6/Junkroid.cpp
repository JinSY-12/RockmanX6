#include "Stdafx.h"
#include "Junkroid.h"
#include "BulletManager.h"

HRESULT Junkroid::init(void)
{
    return S_OK;
}

HRESULT Junkroid::init(int x, int y)
{
    eStatus.maxHp = 4;
    eStatus.hp = eStatus.maxHp;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_Junkroid")->cloneImage();

    eStatus.width = eStatus.eImage->getFrameWidth() - 10 * SCALE_FACTOR;
    eStatus.height = eStatus.eImage->getFrameHeight() - 5 * SCALE_FACTOR;

    eStatus.sightWidth = eStatus.width * 2;
    eStatus.sightHeight = eStatus.height * 2;

    ePos.x = x;
    ePos.y = y;

    eState = EnemyState::Idle;
    eStatus.overpower = false;
    eStatus.dead = false;

    fPos.x = 0 * SCALE_FACTOR;
    fPos.y = IMAGEMANAGER->findImage("SFX_JunkBullet")->getFrameHeight();

    eStatus.eHitBox = RectMakeCenter(x, y - eStatus.height / 2, eStatus.width, eStatus.height);
    eStatus.attSight = RectMakeCenter(x, y - eStatus.sightHeight, eStatus.sightWidth, eStatus.sightHeight);

    eStatus.lookRight = false;

    patternTimer = 0.0f;
    maxPatternTime = 10.0f;

    eStatus.isAtt = false;

    return S_OK;
}

void Junkroid::release(void)
{
}

void Junkroid::update(void)
{
    eStatus.eImage->play(0.07f);

    if (patternTimer >= maxPatternTime) attackAble = true;
    else
    {
        attackAble = false;
        patternTimer += 0.1f;
    }

    setEnemyHitbox();
    animChange();
    isDead();
}

void Junkroid::animChange()
{
    if (eState == EnemyState::Idle)
    {
        eStatus.eImage->setFrameX(0);
        eStatus.eImage->pause();
    }

    else if (eState == EnemyState::Attack)
    {
        eStatus.eImage->resume();
        if (eStatus.eImage->getFrameX() >= eStatus.eImage->getMaxFrameX()) eState = EnemyState::Idle;
    }
}

void Junkroid::attack(void)
{
    eState = EnemyState::Attack;
    bManager->fire(EnemyBulletType::JunkBullet, ePos.x, ePos.y - eStatus.height / 2 - + fPos.y, eStatus.lookRight);
    patternTimer = 0.0f;
}
