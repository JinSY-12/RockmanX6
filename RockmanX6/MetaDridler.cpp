#include "Stdafx.h"
#include "MetaDridler.h"
#include "BulletManager.h"
#include "Player.h"

HRESULT MetaDridler::init(int x, int y, int locationX, int locationY)
{
    status.type = CombatEntityType::Enemy;
    eType = EnemyType::MetaDridler;

    status.maxHp = 10;
    status.hp = status.maxHp;
    status.physicalDamage = 4;

    eStatus.eImage = new GImage;
    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_DridlerIdle")->cloneImage();
    eStatus.ePartsImage = IMAGEMANAGER->findImage("Enemy_DridlerDrill")->cloneImage();

    status.width = eStatus.eImage->getFrameWidth() - 16 * SCALE_FACTOR;
    status.height = eStatus.eImage->getFrameHeight() - 22 * SCALE_FACTOR;

    eStatus.sightWidth = status.width * 2;
    eStatus.sightHeight = status.height + 16 * SCALE_FACTOR;

    eState = EnemyState::Idle;
    dState = DridlerState::Idle;

    status.overpower = false;
    status.dead = false;

    eStatus.eHitBox = RectMakeCenter(x + status.width / 2, y + status.height / 2, status.width, status.height);
    eStatus.worldRect = eStatus.eHitBox;
    eStatus.subHitBox = RectMakeCenter(x + status.width / 2, y + status.height / 2, status.width - 10 * SCALE_FACTOR, status.height - 10 * SCALE_FACTOR);
    eStatus.attSight = RectMakeCenter(x + eStatus.sightWidth / 2, y + eStatus.sightHeight, eStatus.sightWidth, eStatus.sightHeight);

    eStatus.animOffsetX = 8 * SCALE_FACTOR;
    eStatus.animOffsetY = 22 * SCALE_FACTOR;

    eStatus.subOffsetX = 10 * SCALE_FACTOR;
    eStatus.subOffsetY = 0 * SCALE_FACTOR;

    pos.x = x; // x = left
    pos.y = prevLoacationY = y; // y = top

    status.lookRight = false;
    eStatus.attackAble = true;

    eStatus.patternTimer = 0.0f;
    eStatus.maxPatternTime = 20.0f;
    eStatus.invincibleTimer = 0.0f;
    eStatus.invincibleMaxTime = 0.0f;
    eStatus.isOnAttack = false;

    eStatus.VelocityX = 0.0f;
    eStatus.VelocityY = 0.0f;

    eStatus.moveSpeed = 3.0f;
    eStatus.updownSpeed = 3.0f;

    destinationY = locationY;

    raadyTime = 10.0f;

    downSpeed = 1.0f;
	maxDownSpeed = 20.0f;
    upSpeed = 1.5f;

    return S_OK;
}

void MetaDridler::release(void)
{

}

void MetaDridler::update(void)
{
    setEnemyHitbox();
    eStatus.eImage->play(0.06f);
    eStatus.ePartsImage->play(0.06f);
    isDead();

    switch (dState)
    {
        case DridlerState::Idle:
            checkAttCollision();
			break;

        case DridlerState::Down:
            if (eStatus.attackAble)
            {
                eStatus.patternTimer += 0.1f;

                if (eStatus.patternTimer > raadyTime)
                {
                    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_DridlerDown")->cloneImage();
                    eStatus.animOffsetX = 8 * SCALE_FACTOR;
                    eStatus.animOffsetY = 22 * SCALE_FACTOR;

                    eStatus.subOffsetX = 10 * SCALE_FACTOR;
                    eStatus.subOffsetY = 0 * SCALE_FACTOR;

                    eStatus.attackAble = false;
                }
            }

            else
            {
                downAtt();
            }
            break;

        case DridlerState::DownStop:
            if (eStatus.attackAble)
            {
                eStatus.patternTimer += 0.1f;

                if (eStatus.patternTimer > 10.0f)
                {
                    eStatus.eImage = IMAGEMANAGER->findImage("Enemy_DridlerUp")->cloneImage();
                    eStatus.animOffsetX = 8 * SCALE_FACTOR;
                    eStatus.animOffsetY = 22 * SCALE_FACTOR;

                    eStatus.subOffsetX = 10 * SCALE_FACTOR;
                    eStatus.subOffsetY = 0 * SCALE_FACTOR;

                    eStatus.attackAble = false;
                }
            }

            else
            {
                upReady();
            }
            break;

        case DridlerState::Up:
            eStatus.eImage = IMAGEMANAGER->findImage("Enemy_DridlerUp")->cloneImage();
            eStatus.animOffsetX = 8 * SCALE_FACTOR;
            eStatus.animOffsetY = 22 * SCALE_FACTOR;

            eStatus.subOffsetX = 10 * SCALE_FACTOR;
            eStatus.subOffsetY = 0 * SCALE_FACTOR;
            break;

        case DridlerState::UpStop:
            eStatus.animOffsetX = 8 * SCALE_FACTOR;
            eStatus.animOffsetY = 22 * SCALE_FACTOR;

            eStatus.subOffsetX = 10 * SCALE_FACTOR;
            eStatus.subOffsetY = 0 * SCALE_FACTOR;
			dState = DridlerState::Down;
            break;
        
    }
}

void MetaDridler::downAtt(void)
{
    if (pos.y + status.height < destinationY)
    {
        if (downSpeed < maxDownSpeed) downSpeed += 0.10f;
        else downSpeed = maxDownSpeed;
        
        pos.y += downSpeed;
    }

    else
    {
        eStatus.attackAble = true;
        eStatus.patternTimer = 0.0f;
        raadyTime = 15.0f;
		eStatus.eImage = IMAGEMANAGER->findImage("Enemy_DridlerIdle")->cloneImage();
        downSpeed = 1.0f;
		dState = DridlerState::DownStop;
    }

}

void MetaDridler::upReady(void)
{
    
    if (pos.y > prevLoacationY)
    {
        pos.y -= upSpeed;
    }

    else
    {
        eStatus.attackAble = true;
        eStatus.patternTimer = 0.0f;
		dState = DridlerState::Down;
    }
}

void MetaDridler::attack(void)
{
    eState = EnemyState::Attack;
    dState = DridlerState::Down;
}

void MetaDridler::checkAttCollision(void)
{
    RECT temp;
    if (IntersectRect(&temp, &eStatus.attSight, &player->getPlayerHitBox()))
    {
        attack();
    }
}

void MetaDridler::setEnemyHitbox(void)
{
	// 월드 좌표 설정
    eStatus.eHitBox.left = pos.x - CAMERAMANAGER->getCameraPos().x;
    eStatus.eHitBox.right = eStatus.eHitBox.left + status.width - CAMERAMANAGER->getCameraPos().x;

    eStatus.eHitBox.bottom = pos.y - CAMERAMANAGER->getCameraPos().y;
    eStatus.eHitBox.top = eStatus.eHitBox.bottom - status.height - CAMERAMANAGER->getCameraPos().y;

	// 서브 히트박스 설정
	eStatus.subHitBox.left = eStatus.eHitBox.left + 17 * SCALE_FACTOR;
    eStatus.subHitBox.right = eStatus.subHitBox.left + 22 * SCALE_FACTOR;

    eStatus.subHitBox.top = eStatus.eHitBox.top + 69 * SCALE_FACTOR;
    eStatus.subHitBox.bottom = eStatus.subHitBox.top + 50 * SCALE_FACTOR;

	// 공격 범위 설정
    // 판정 테스트
    // eStatus.attSight.left = eStatus.eHitBox.left;
    // eStatus.attSight.right = eStatus.attSight.left + status.width;

    // 원래 판정
    eStatus.attSight.left = eStatus.eHitBox.left - status.width;
    eStatus.attSight.right = eStatus.attSight.left + status.width * 3;

    eStatus.attSight.top = eStatus.eHitBox.bottom - 10 * SCALE_FACTOR;
    eStatus.attSight.bottom = eStatus.attSight.top + status.height * 3;

}

