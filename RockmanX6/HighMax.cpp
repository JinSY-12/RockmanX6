#include "Stdafx.h"
#include "HighMax.h"

HRESULT HighMax::init(int x, int y)
{
	return S_OK;
}

HRESULT HighMax::init(int x, int y, int locationX, int locationY)
{
	return S_OK;
}

HRESULT HighMax::init(int x, int y, int width, int top, int height)
{
	return S_OK;
}

void HighMax::bossAppearance(void)
{

}

void HighMax::patternShoot(void)
{

}

ShootEvent HighMax::makeShootEvent(BulletType bType)
{
	ShootEvent shootEvent;
	shootEvent.bType = bType;
	shootEvent.x = pos.x;
	shootEvent.y = pos.y - status.hitBoxHeight / 2;
	shootEvent.direct = status.lookRight;

	return shootEvent;
}
