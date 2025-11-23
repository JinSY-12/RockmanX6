#include "Stdafx.h"
#include "BulletManager.h"
#include "Player.h"
#include "MetaWheel.h"

HRESULT MetaWheel::init(void)
{
    return S_OK;
}

HRESULT MetaWheel::init(int x, int y)
{
    eType = EnemyType::MetaWheel;

    eStatus.maxHp = 10;
    eStatus.hp = eStatus.maxHp;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_MetaWheel")->cloneImage();

    eStatus.width = eStatus.eImage->getFrameWidth() - 5 * SCALE_FACTOR;
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
    eStatus.isOnAttack = false;

    eStatus.VelocityX = 0.0f;
    eStatus.VelocityY = 0.0f;
    
    eStatus.moveSpeed = 3.0f;
    eStatus.updownSpeed = 3.0f;

    return S_OK;
}

void MetaWheel::release(void)
{
}

void MetaWheel::update(void)
{
    setEnemyHitbox();
    checkPlayerCollision();
    checkPlayerAttCollision();
    checkBulletCollision();
    enemyInvincibleTimerUpdate();
    isDead();

    if (eStatus.touchLeftWall && !eStatus.lookRight)
    {
        eState = EnemyState::Idle;
        eStatus.isOnAttack = false;
        changeDirection();
        eStatus.eImage->setFrameX(eStatus.eImage->getMaxFrameX() - eStatus.eImage->getFrameX());
    }

    else if (eStatus.touchRightWall && eStatus.lookRight)
    {
        eState = EnemyState::Idle;
        eStatus.isOnAttack = false;
        changeDirection();
        eStatus.eImage->setFrameX(eStatus.eImage->getMaxFrameX() - eStatus.eImage->getFrameX());
    }

    switch (eState)
    {
    case EnemyBase::EnemyState::Idle:
        pattern();
        break;
    case EnemyBase::EnemyState::Attack:
        eStatus.isOnAttack = true;
        eStatus.eImage->play(0.3f);
        eStatus.VelocityX = eStatus.lookRight ? eStatus.moveSpeed : -eStatus.moveSpeed;

        eStatus.worldRect.left += eStatus.VelocityX;
        eStatus.worldRect.right += eStatus.VelocityX;
        ePos.x += eStatus.VelocityX;
        break;
    default:
        break;
    }
}

void MetaWheel::attack(void)
{
    eState = EnemyState::Attack;
    eStatus.patternTimer = 0.0f;
}

void MetaWheel::checkPlayerCollision(void)
{
    RECT temp;
    if (IntersectRect(&temp, &eStatus.attSight, &player->getPlayerRect()))
    {
        if (eStatus.attackAble)
        {
            attack();
            eStatus.attackAble = false;
        }
    }
}
