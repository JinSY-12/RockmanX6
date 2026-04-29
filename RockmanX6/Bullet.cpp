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
	bStatus.shape->play(bStatus.animSpeed);

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
	// bStatus.shape->frameRender(getMemDC(), bStatus.hitBox.left - bStatus.animOffsetX,
		// bStatus.hitBox.top - bStatus.animOffsetY, bStatus.shape->getFrameX(), bStatus.rightDirect);

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
	bStatus.shape = new GImage;
	bStatus.shape = IMAGEMANAGER->findImage("SFX_SiegeShootFire")->cloneImage();
	bStatus.demage = 0;

	bStatus.type = BulletSize::Large;
	bStatus.bType = BulletType::SiegeShoot;
	bStatus.faction = BulletFaction::Enemy;

	bStatus.width = 30 * SCALE_FACTOR;
	bStatus.height = 30 * SCALE_FACTOR;

	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

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

	bStatus.soundName = "SFX_SiegeShootFire";
	SOUNDMANAGER->play(bStatus.soundName, 0.3f);

	return S_OK;
}

void SiegeShoot::update(void)
{
	bStatus.shape->play(bStatus.animSpeed);

	if(!fireStart)
	{
		bStatus.pos.x += bStatus.velocityX * 0.3f;
		bStatus.pos.y += bStatus.velocityY * 0.3f;
		bStatus.animSpeed = 0.06f;
	}
	
	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;

	if (bStatus.shape->getChangeReady())
	{
		bStatus.shape = IMAGEMANAGER->findImage("SFX_SiegeShootLoop")->cloneImage();
		fireStart = false;
	}
}

HRESULT DeathBall1::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	bStatus.shape = new GImage;
	// bStatus.shape = IMAGEMANAGER->findImage("SFX_DeathBall")->cloneImage();
	bStatus.demage = 4;

	bStatus.type = BulletSize::Large;
	bStatus.bType = BulletType::DeathBall1;
	bStatus.faction = BulletFaction::Enemy;

	bStatus.width = 30 * SCALE_FACTOR;
	bStatus.height = 30 * SCALE_FACTOR;

	// bStatus.width = bStatus.shape->getFrameWidth();
	// bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;

	bStatus.pos.x = x;
	bStatus.pos.y = fireStartPointY = y;

	bStatus.velocityX = bStatus.bulletSpeed;
	bStatus.velocityY = bStatus.bulletSpeed;

	if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.height / 2, bStatus.width, bStatus.height);
	else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.height / 2, bStatus.width, bStatus.height);

	// if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());
	// else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());

	bStatus.isFire = true;

	bStatus.soundName = "SFX_SiegeShootFire";
	SOUNDMANAGER->play(bStatus.soundName, 0.3f);

	cout << "데스볼 소환!" << endl;

	return S_OK;
}

void DeathBall1::update(void)
{
	// 벽에 닿으면 위로 상승
	if (bStatus.wallTouch)
	{
		bStatus.pos.y -= bStatus.velocityY;
	}

	// 벽이 아니면 내려갔다가 반대쪽으로 이동
	else
	{
		// 하강 이동
		if (bStatus.pos.y - fireStartPointY >= 0.0f && bStatus.pos.y - fireStartPointY <= 80.0f)
		{
			bStatus.pos.y += bStatus.velocityY;
		}

		// 측면 이동
		else
		{
			if(bStatus.rightDirect) bStatus.pos.x += bStatus.velocityX;
			else bStatus.pos.x -= bStatus.velocityX;
		}
	}

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;
}
