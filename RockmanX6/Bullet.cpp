#include "Stdafx.h"
#include "Bullet.h"

HRESULT Bullet::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	return S_OK;
}

void Bullet::release(void)
{
	// Do Nothing!
}

void Bullet::update(void)
{
	bStatus.shape->play(0.05f);

	bStatus.pos.x += bStatus.velocityX;
	bStatus.pos.y += bStatus.velocityY;

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;
}

void Bullet::render(void)
{
	// bStatus.shape->frameRender(getMemDC(), bStatus.hitBox.left, bStatus.hitBox.top, bStatus.shape->getFrameX(), bStatus.rightDirect);

	if (UIMANAGER->getIsDebugMode() == true)
		DrawRectMakeColor(getMemDC(), bStatus.hitBox, RGB(0, 255, 0), 2);
}

HRESULT Burster::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	switch (type)
	{
	case BulletType::Burster:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster1")->cloneImage();
		bStatus.faction = BulletFaction::Player;
		//bStatus.demage = 0;
		bStatus.demage = 1;
		bStatus.bType = BulletType::Burster;
		break;

	case BulletType::ChargeBurst1:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster2")->cloneImage();
		bStatus.faction = BulletFaction::Player;
		//bStatus.demage = 0;
		bStatus.demage = 2;
		bStatus.bType = BulletType::ChargeBurst1;
		break;

	case BulletType::ChargeBurst2:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster3")->cloneImage();
		bStatus.faction = BulletFaction::Player;
		//bStatus.demage = 0;
		bStatus.demage = 4;
		bStatus.bType = BulletType::ChargeBurst2;
		break;
	}
	
	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;
	
	bStatus.pos.x = x;
	bStatus.pos.y = y;

	if (isRight) bStatus.hitBox = RectMake(x - 4 * SCALE_FACTOR, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth() , bStatus.shape->getFrameHeight());
	else bStatus.hitBox = RectMake(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth() , bStatus.shape->getFrameHeight());

	int direction = isRight ? 1 : -1;
	bStatus.velocityX = direction * bStatus.bulletSpeed;
	bStatus.velocityY = 0.0f;
	bStatus.isFire = true;

	return S_OK;
}


HRESULT JunkBullet::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	bStatus.shape = new GImage;
	bStatus.shape = IMAGEMANAGER->findImage("SFX_JunkBullet")->cloneImage();
	bStatus.demage = 2;

	bStatus.type = BulletSize::Small;
	bStatus.bType = BulletType::JunkBullet;
	bStatus.faction = BulletFaction::Enemy;

	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;
	
	bStatus.pos.x = x;
	bStatus.pos.y = y;

	if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());
	else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());

	int direction = isRight ? 1 : -1;
	bStatus.velocityX = direction * bStatus.bulletSpeed;
	bStatus.velocityY = 0.0f;
	bStatus.isFire = true;

	return S_OK;
}

HRESULT SiegeShoot::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	// bStatus.shape = new GImage;
	// bStatus.shape = IMAGEMANAGER->findImage("SFX_SiegeShootFire")->cloneImage();
	bStatus.demage = 4;

	bStatus.type = BulletSize::Large;
	bStatus.bType = BulletType::SiegeShoot;
	bStatus.faction = BulletFaction::Enemy;

	bStatus.width = 30 * SCALE_FACTOR;
	bStatus.height = 30 * SCALE_FACTOR;

	// bStatus.width = bStatus.shape->getFrameWidth();
	// bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;

	bStatus.pos.x = x;
	bStatus.pos.y = y;

	bStatus.velocityX = velocityX * bStatus.bulletSpeed;
	bStatus.velocityY = velocityY * bStatus.bulletSpeed;
	
	if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.height / 2, bStatus.width, bStatus.height);
	else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.height / 2, bStatus.width, bStatus.height);

	// if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());
	// else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());

	bStatus.isFire = true;
	fireStart = true;

	return S_OK;
}


void SiegeShoot::update(void)
{
	// bStatus.shape->play(0.05f);

	/*
	if(!fireStart)
	{
		bStatus.pos.x += bStatus.velocityX;
		bStatus.pos.y += bStatus.velocityY;
	}
	*/
	bStatus.pos.x += bStatus.velocityX * 0.5f;
	bStatus.pos.y += bStatus.velocityY * 0.5f;

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	// bStatus.hitBox.top = bStatus.pos.y - bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;
	// bStatus.hitBox.bottom = bStatus.pos.y + bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;

	/*
	if (bStatus.shape->getFrameX() > bStatus.shape->getMaxFrameX())
	{
		if (fireStart)
		{
			bStatus.shape = IMAGEMANAGER->findImage("SFX_SiegeShootLoop")->cloneImage();
			fireStart = false;
		}
	}
	*/

}