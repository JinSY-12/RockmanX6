#include "Stdafx.h"
#include "Player.h"


HRESULT Player::init(void)
{
	// Do Nothing!!
	return S_OK;
}

HRESULT Player::init(int x, int y)
{
	return S_OK;
}

void Player::release(void)
{
	// Do Nothing!!
}

void Player::update(void)
{
	// Do Nothing!!
}

void Player::render(void)
{
	// Do Nothing!!
}

void Player::move(bool direction)
{
	// 우로 이동
	if (direction == false) CAMERAMANAGER->setPos(4, 0);

	// 좌로 이동
	else if(direction == true) CAMERAMANAGER->setPos(-4, 0);
}

void Player::jump(void)
{
	CAMERAMANAGER->setPos(0, 4);
}

void Player::dash(void)
{

}

void Player::attack(void)
{


}

void Player::spawn(int x, int y)
{
}
