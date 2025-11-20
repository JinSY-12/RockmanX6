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
    eStatus.maxPatternTime = 15.0f;
    eStatus.invincibleTimer = 0.0f;
    eStatus.invincibleMaxTime = 0.0f;
    eStatus.isAtt = false;

    return S_OK;
}

void MetaWheel::release(void)
{
}

void MetaWheel::update(void)
{
}
