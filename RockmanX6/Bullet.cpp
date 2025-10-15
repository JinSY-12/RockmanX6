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
		bStatus.demage = 1;
		break;
	case BulletType::ChargeBurst1:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster2")->cloneImage();
		bStatus.demage = 2;
		break;
	case BulletType::ChargeBurst2:
		bStatus.shape = new GImage;
		bStatus.shape = IMAGEMANAGER->findImage("X_Burster3")->cloneImage();
		bStatus.demage = 3;
		break;
	}
	
	bStatus.width = bStatus.shape->getFrameWidth();
	bStatus.height = bStatus.shape->getFrameHeight();

	bStatus.rightDirect = isRight;
	

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

	bStatus.hitBox.left += bStatus.speed;
	bStatus.hitBox.right += bStatus.speed;

	if (bStatus.hitBox.left > WINSIZE_X) bStatus.isFire = false;
	else if (bStatus.hitBox.right < 0) bStatus.isFire = false;
}

void Burster::render(void)
{
	bStatus.shape->frameRender(getMemDC(), bStatus.hitBox.left, bStatus.hitBox.top, bStatus.shape->getFrameX(), bStatus.rightDirect);
	
	if (UIMANAGER->getIsDebugMode() == true)
		DrawRectMakeColor(getMemDC(), bStatus.hitBox, RGB(0, 255, 0), 2);
}
