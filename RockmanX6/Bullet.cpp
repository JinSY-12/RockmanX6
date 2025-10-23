#include "Stdafx.h"
#include "Bullet.h"

HRESULT Bullet::init(void)
{

	return S_OK;
}

HRESULT Bullet::init(BulletType type, int x, int y, bool isRight)
{
	return S_OK;
}

HRESULT Bullet::init(EnemyBulletType type, int x, int y, bool isRight)
{
	return S_OK;
}

void Bullet::release(void)
{
	// Do Nothing!
}


void Bullet::update(void)
{
}

void Bullet::render(void)
{
}


HRESULT Burster::init(void)
{
	return S_OK;
}

HRESULT Burster::init(BulletType type, int x, int y, bool isRight)
{
	switch (type)
	{
	case BulletType::Buster:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster1")->cloneImage();
		bStatus.demage = 5;
		bStatus.bType = BulletType::Buster;
		break;

	case BulletType::ChargeBurst1:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster2")->cloneImage();
		bStatus.demage = 2;
		bStatus.bType = BulletType::ChargeBurst1;
		break;

	case BulletType::ChargeBurst2:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster3")->cloneImage();
		bStatus.demage = 3;
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
	bStatus.speed = direction * 20.0f;
	bStatus.isFire = true;

	return S_OK;
}

void Burster::release(void)
{
}

void Burster::update(void)
{
	bStatus.shape->play(0.05f);

	bStatus.pos.x += bStatus.speed;

	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	
	///////////////////////
	// 여기에 bStatus.pos.y += bStatus.speed; 추가하면 대각선 이동 가능
	///////////////////////

	bStatus.hitBox.top = bStatus.pos.y - bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;

}

void Burster::render(void)
{
	bStatus.shape->frameRender(getMemDC(), bStatus.hitBox.left, bStatus.hitBox.top, bStatus.shape->getFrameX(), bStatus.rightDirect);
	
	if (UIMANAGER->getIsDebugMode() == true)
		DrawRectMakeColor(getMemDC(), bStatus.hitBox, RGB(0, 255, 0), 2);
}

HRESULT JunkBullet::init(void)
{
	return S_OK;
}

HRESULT JunkBullet::init(EnemyBulletType type, int x, int y, bool isRight)
{
	switch (type)
	{
	case EnemyBulletType::JunkBullet:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("SFX_JunkBullet")->cloneImage();
		bStatus.demage = 2;
		break;
	}

	bStatus.type = BulletSize::Small;

	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;
	
	bStatus.pos.x = x;
	bStatus.pos.y = y;

	if (isRight) bStatus.hitBox = RectMakeCenter(x, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());
	else bStatus.hitBox = RectMakeCenter(x - bStatus.width + 4 * SCALE_FACTOR, y - bStatus.shape->getFrameHeight() / 2, bStatus.shape->getFrameWidth(), bStatus.shape->getFrameHeight());

	int direction = isRight ? 1 : -1;
	bStatus.speed = direction * 5.0f;
	bStatus.isFire = true;

	return S_OK;
}

void JunkBullet::release(void)
{
}

void JunkBullet::update(void)
{
	bStatus.shape->play(0.05f);

	bStatus.pos.x += bStatus.speed;
	
	bStatus.hitBox.left = bStatus.pos.x - bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	bStatus.hitBox.right = bStatus.pos.x + bStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	bStatus.hitBox.top = bStatus.pos.y - bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;
	bStatus.hitBox.bottom = bStatus.pos.y + bStatus.shape->getFrameHeight() / 2 - CAMERAMANAGER->getCameraPos().y;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;
}

void JunkBullet::render(void)
{
	bStatus.shape->frameRender(getMemDC(), bStatus.hitBox.left, bStatus.hitBox.top, bStatus.shape->getFrameX(), bStatus.rightDirect);

	if (UIMANAGER->getIsDebugMode() == true)
		DrawRectMakeColor(getMemDC(), bStatus.hitBox, RGB(0, 255, 0), 2);
}
