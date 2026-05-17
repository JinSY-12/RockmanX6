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
    status.type = CombatEntityType::Enemy;
    eType = EnemyType::MetaWheel;

    status.maxHp = 10;
    status.hp = status.maxHp;
    status.physicalDamage = 4;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_MetaWheel")->cloneImage();

    status.width = eStatus.eImage->getFrameWidth() - 5 * SCALE_FACTOR;
    status.height = eStatus.eImage->getFrameHeight() - 5 * SCALE_FACTOR;

    eStatus.sightWidth = status.width * 2;
    eStatus.sightHeight = status.height - 16 * SCALE_FACTOR;

    eState = EnemyState::Idle;
    status.overpower = false;
    status.dead = false;

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
    checkAttCollision();
    enemyInvincibleTimerUpdate();
    isDead();

    if (eStatus.touchLeftWall && !status.lookRight)
    {
        eState = EnemyState::Idle;
        eStatus.isOnAttack = false;
        changeDirection();
        eStatus.eImage->setFrameX(eStatus.eImage->getMaxFrameX() - eStatus.eImage->getFrameX());
    }

    else if (eStatus.touchRightWall && status.lookRight)
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
        eStatus.VelocityX = status.lookRight ? eStatus.moveSpeed : -eStatus.moveSpeed;

        eStatus.worldRect.left += eStatus.VelocityX;
        eStatus.worldRect.right += eStatus.VelocityX;
        pos.x += eStatus.VelocityX;
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

void MetaWheel::checkAttCollision(void)
{
    RECT temp;
    if (IntersectRect(&temp, &eStatus.attSight, &player->getPlayerHitBox()))
    {
        if (eStatus.attackAble)
        {
            attack();
            eStatus.attackAble = false;
        }
    }
}
