#include "Stdafx.h"
#include "X.h"

HRESULT X::init(void)
{

	return S_OK;
}

HRESULT X::init(int x, int y)
{

	width = 20;
	height = 60;

	spawn(x, y);
		

	return S_OK;
}

void X::release(void)
{

}

void X::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		stat.lookRight = true;
		move(stat.lookRight);
	}

	else if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		stat.lookRight = false;
		move(stat.lookRight);
	}

	if (KEYMANAGER->isStayKeyDown('X'))
	{
		if (stat.isOnGround) Player::jump();
		else if (!stat.isOnGround) jump();
	}

	if (KEYMANAGER->isStayKeyDown('C'))
	{
		stat.isAtt = true;
		attack();
	}
}

void X::render(void)
{

}

void X::jump(void)
{
	CAMERAMANAGER->setPos(4, 0);
}

void X::attack(void)
{

}

void X::spawn(int x, int y)
{
	
	stat.hitBox = RectMakeCenter(x, y, width, height);
}
