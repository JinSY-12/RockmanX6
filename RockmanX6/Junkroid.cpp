#include "Stdafx.h"
#include "Junkroid.h"
#include "BulletManager.h"
#include "Player.h"
#include "MetaWheel.h"

HRESULT Junkroid::init(void)
{
    return S_OK;
}

HRESULT Junkroid::init(int x, int y)
{
    eType = EnemyType::Junkroid;

    eStatus.maxHp = 4;
    eStatus.hp = eStatus.maxHp;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_Junkroid")->cloneImage();

    eStatus.width = eStatus.eImage->getFrameWidth() - 10 * SCALE_FACTOR;
    eStatus.height = eStatus.eImage->getFrameHeight() - 5 * SCALE_FACTOR;

    eStatus.sightWidth = eStatus.width * 2;
    eStatus.sightHeight = eStatus.height - 16 * SCALE_FACTOR;

    eState = EnemyState::Idle;
    eStatus.overpower = false;
    eStatus.dead = false;

    fPos.x = 0 * SCALE_FACTOR;
    fPos.y = IMAGEMANAGER->findImage("SFX_JunkBullet")->getFrameHeight();

    eStatus.eHitBox = RectMakeCenter(x + eStatus.width / 2, y + eStatus.height / 2, eStatus.width, eStatus.height);
    eStatus.worldRect = eStatus.eHitBox;
    eStatus.attSight = RectMakeCenter(x + eStatus.sightWidth / 2, y + eStatus.sightHeight, eStatus.sightWidth, eStatus.sightHeight);

    ePos.x = x;
    ePos.y = eStatus.worldRect.bottom;

    eStatus.lookRight = false;

    eStatus.patternTimer = 0.0f;
    eStatus.maxPatternTime = 20.0f;
	eStatus.invincibleTimer = 0.0f;
    eStatus.invincibleMaxTime = 0.0f;
    eStatus.isAtt = false;

    return S_OK;
}

void Junkroid::release(void)
{

}

void Junkroid::update(void)
{
    eStatus.eImage->play(0.05f);

    pattern();
    changeDirection();
    setEnemyHitbox();
    animChange();
    checkPlayerCollision();
    checkPlayerAttCollision();
    checkBulletCollision();
    enemyInvincibleTimerUpdate();
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

void MetaWheel::attack(void)
{
}

void MetaWheel::checkPlayerCollision(void)
{
}

void MetaWheel::checkPlayerAttCollision(void)
{
}

void Junkroid::attack(void)
{
    eState = EnemyState::Attack;
    bManager->fire(EnemyBulletType::JunkBullet, (eStatus.worldRect.left + eStatus.worldRect.right) / 2,
        eStatus.worldRect.top + fPos.y, eStatus.lookRight);
    eStatus.patternTimer = 0.0f;
}

void Junkroid::checkPlayerCollision()
{
    RECT temp;
    if (IntersectRect(&temp, &eStatus.attSight, &player->getPlayerRect()))
    {
        if (eStatus.attackAble)
        {
			attack();
        }
	}
}
