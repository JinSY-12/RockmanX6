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

    status.maxHp = 4;
    status.hp = status.maxHp;
    status.physicalDamage = 2;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_Junkroid")->cloneImage();

    status.width = eStatus.eImage->getFrameWidth() - 10 * SCALE_FACTOR;
    status.height = eStatus.eImage->getFrameHeight() - 5 * SCALE_FACTOR;

    eStatus.sightWidth = status.width * 2;
    eStatus.sightHeight = status.height - 16 * SCALE_FACTOR;

    eState = EnemyState::Idle;
    status.overpower = false;
    status.dead = false;

    eStatus.touchLeftWall = false;
    eStatus.touchRightWall = false;

    fPos.x = 0 * SCALE_FACTOR;
    fPos.y = IMAGEMANAGER->findImage("SFX_JunkBullet")->getFrameHeight();

    eStatus.eHitBox = RectMakeCenter(x + status.width / 2, y + status.height / 2, status.width, status.height);
    eStatus.worldRect = eStatus.eHitBox;
    eStatus.attSight = RectMakeCenter(x + eStatus.sightWidth / 2, y + eStatus.sightHeight, eStatus.sightWidth, eStatus.sightHeight);

    pos.x = x;
    pos.y = eStatus.worldRect.bottom;

    status.lookRight = false;

    eStatus.patternTimer = 0.0f;
    eStatus.maxPatternTime = 20.0f;
	eStatus.invincibleTimer = 0.0f;
    eStatus.invincibleMaxTime = 0.0f;
    eStatus.isOnAttack = false;

    eStatus.moveSpeed = 0.0f;
    eStatus.updownSpeed = 0.0f;
        

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
    // checkPlayerCollision();
    // checkPlayerAttCollision();
    checkBulletCollision();
    enemyInvincibleTimerUpdate();
    isDead();
}

void Junkroid::animChange()
{
    if (eState == EnemyState::Idle)
    {
        eStatus.isOnAttack = false;
        eStatus.eImage->setFrameX(0);
        eStatus.eImage->pause();
    }

    else if (eState == EnemyState::Attack)
    {
        eStatus.eImage->resume();
        eStatus.isOnAttack = true;
        if (eStatus.eImage->getFrameX() >= eStatus.eImage->getMaxFrameX()) eState = EnemyState::Idle;
    }
}

void Junkroid::attack(void)
{
    eState = EnemyState::Attack;
    bManager->fire(EnemyBulletType::JunkBullet, (eStatus.worldRect.left + eStatus.worldRect.right) / 2,
        eStatus.worldRect.top + fPos.y, status.lookRight);
}

void Junkroid::checkPlayerCollision()
{
    RECT temp;
    if (IntersectRect(&temp, &eStatus.attSight, &player->getPlayerRect()))
    {
        if (eStatus.attackAble)
        {
			// attack();
            eStatus.attackAble = false;
        }
	}
}
