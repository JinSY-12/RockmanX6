#include "Stdafx.h"
#include "Bullet.h"

HRESULT Bullet::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	// Do Nothing!

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

	if(bStatus.hitBox.left > WINSIZE_X + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.top > WINSIZE_Y + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.bottom < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
}

void Bullet::render(void)
{
	bStatus.shape->frameRender(getMemDC(), bStatus.hitBox.left - bStatus.animOffsetX,
		bStatus.hitBox.top - bStatus.animOffsetY, bStatus.shape->getFrameX(), bStatus.rightDirect);

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
		bStatus.demage = 1;
		bStatus.bType = type;
		break;

	case BulletType::ChargeBurst1:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster2")->cloneImage();
		bStatus.faction = BulletFaction::Player;
		bStatus.demage = 2;
		bStatus.bType = type;
		break;

	case BulletType::ChargeBurst2:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster3")->cloneImage();
		bStatus.faction = BulletFaction::Player;
		bStatus.demage = 4;
		bStatus.bType = type;
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
	bStatus.bType = type;
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
	bStatus.demage = 4;

	bStatus.type = BulletSize::Large;
	bStatus.bType = type;
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
		bStatus.pos.x += bStatus.velocityX;
		bStatus.pos.y += bStatus.velocityY;
		bStatus.animSpeed = 0.06f;
	}
	
	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.top > WINSIZE_Y + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.bottom < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;

	if (bStatus.shape->getChangeReady())
	{
		bStatus.shape = IMAGEMANAGER->findImage("SFX_SiegeShootLoop")->cloneImage();
		fireStart = false;
	}
}

HRESULT DeathBall::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	bStatus.shape = new GImage;
	bStatus.shape = IMAGEMANAGER->findImage("SFX_DeathBall")->cloneImage();
	bStatus.demage = 6;

	bStatus.type = BulletSize::Large;
	bStatus.bType = type;
	bStatus.faction = BulletFaction::Enemy;

	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;

	bStatus.pos.x = fireStartPointX =  x;
	bStatus.pos.y = fireStartPointY = y;

	bStatus.velocityX = bStatus.bulletSpeed;
	bStatus.velocityY = bStatus.bulletSpeed;

	if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.height / 2, bStatus.width, bStatus.height);
	else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.height / 2, bStatus.width, bStatus.height);

	bStatus.isFire = true;

	bStatus.soundName = "SFX_DeathBall";
	SOUNDMANAGER->play(bStatus.soundName, 0.3f);

	return S_OK;
}

void DeathBall::update(void)
{
	bStatus.shape->play(bStatus.animSpeed);
	
	// 옆 벽에 닿으면 위로 상승
	if(bStatus.pos.x - fireStartPointX <= -240.0f * SCALE_FACTOR && !bStatus.rightDirect)
	{
		bStatus.pos.y -= bStatus.velocityY;
	}
	
	else if (bStatus.pos.x - fireStartPointX >= 240.0f * SCALE_FACTOR && bStatus.rightDirect)
	{
		bStatus.pos.y -= bStatus.velocityY;
	}

	// 옆 벽이 아니면 내려갔다가 반대쪽으로 이동
	else
	{
		// 하강 이동
		if (bStatus.pos.y - fireStartPointY >= 0.0f && bStatus.pos.y - fireStartPointY <= 75.0f * SCALE_FACTOR)
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

	if (bStatus.hitBox.left > WINSIZE_X + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.top > WINSIZE_Y + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.bottom < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;

	if(bStatus.isFire == false) SOUNDMANAGER->stop("SFX_DeathBall");
}

HRESULT DeathBubble::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	bStatus.shape = new GImage;
	bStatus.shape = IMAGEMANAGER->findImage("SFX_DeathBubble")->cloneImage();
	bStatus.demage = 2;

	bStatus.type = BulletSize::Large;
	bStatus.bType = type;
	bStatus.faction = BulletFaction::Player;

	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;

	bStatus.pos.x = x;
	bStatus.pos.y = y;

	bStatus.velocityX = velocityX * bStatus.bulletSpeed;
	bStatus.velocityY = velocityY * bStatus.bulletSpeed;

	if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.height / 2, bStatus.width, bStatus.height);
	else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.height / 2, bStatus.width, bStatus.height);

	bStatus.isFire = true;

	return S_OK;
}

void DeathBubble::update(void)
{
	bStatus.shape->play(bStatus.animSpeed);

	

	bStatus.pos.x += bStatus.velocityX;
	bStatus.pos.y += bStatus.velocityY;
	bStatus.animSpeed = 0.06f;

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.top > WINSIZE_Y + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.bottom < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;

}

HRESULT DeathRing::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	bStatus.shape = new GImage;
	bStatus.shape = IMAGEMANAGER->findImage("SFX_DeathRing")->cloneImage();
	bStatus.demage = 2;

	bStatus.type = BulletSize::Large;
	bStatus.bType = type;
	bStatus.faction = BulletFaction::Player;

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

	return S_OK;
}

void DeathRing::update(void)
{
	bStatus.shape->play(bStatus.animSpeed);

	bStatus.pos.x += bStatus.velocityX;
	bStatus.pos.y += bStatus.velocityY;
	bStatus.animSpeed = 0.06f;

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.top > WINSIZE_Y + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.bottom < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
}

/*
HRESULT DeathRing::init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY)
{
	bStatus.shape = new GImage;
	bStatus.shape = IMAGEMANAGER->findImage("SFX_DeathRing")->cloneImage();
	bStatus.demage = 2;

	bStatus.type = BulletSize::Large;
	bStatus.bType = type;
	bStatus.faction = BulletFaction::Player;

	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;

	bStatus.pos.x = x;
	bStatus.pos.y = y;

	bStatus.velocityX = velocityX * bStatus.bulletSpeed;
	bStatus.velocityY = velocityY * bStatus.bulletSpeed;

	bStatus.hitBox = RectMakeCenter(x, y - bStatus.height / 2, bStatus.width, bStatus.height);
	
	bStatus.isFire = true;

	return S_OK;
}

void DeathRing::update(void)
{
	bStatus.shape->play(bStatus.animSpeed);

	bStatus.pos.x += bStatus.velocityX;
	bStatus.pos.y += bStatus.velocityY;

	cout << "X :" << bStatus.pos.x / 3 << endl;
	cout << "Y :" << bStatus.pos.y / 3 << endl;

	cout << "LEFT :" << bStatus.hitBox.left << endl;
	cout << "TOP :" << bStatus.hitBox.top << endl;

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.height / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.top > WINSIZE_Y + 30 * SCALE_FACTOR) bStatus.isFire = false;
	else if (bStatus.hitBox.bottom < 0 - 30 * SCALE_FACTOR) bStatus.isFire = false;

}
*/