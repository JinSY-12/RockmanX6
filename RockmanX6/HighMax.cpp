#include "Stdafx.h"
#include "HighMax.h"
#include "BulletManager.h"
#include "Player.h"

HRESULT HighMax::init(int x, int y)
{
	status.type = CombatEntityType::Enemy;
	btype = BossType::Intro;

	status.maxHp = 4;
	status.hp = status.maxHp;
	status.physicalDamage = 2;

	bStatus.bImage = new GImage;
	bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();

	status.width = bStatus.bImage->getFrameWidth();
	status.height = bStatus.bImage->getFrameHeight();

	bStatus.overPower = false;
	status.lookRight = false;

	bStatus.bHitBox = bStatus.bWorldRect = RectMakeCenter(x + status.width /2, y - status.height / 2, status.width, status.height);
	
	pos.x = x;
	pos.y = y;

	animSpeed = 0.1f;

	return S_OK;
}

HRESULT HighMax::init(int x, int y, int locationX, int locationY)
{
	status.type = CombatEntityType::Enemy;
	btype = BossType::Intro;

	status.maxHp = 4;
	status.hp = status.maxHp;
	status.physicalDamage = 2;

	bStatus.bImage = new GImage;
	bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();

	status.width = bStatus.bImage->getFrameWidth();
	status.height = bStatus.bImage->getFrameHeight();

	bStatus.overPower = false;

	//bStatus.bHitBox = RectMakeCenter();
	//bStatus.bWorldRect;

	pos.x = x;
	pos.y = y;



	return S_OK;
}

HRESULT HighMax::init(int x, int y, int width, int top, int height)
{
	return S_OK;
}

void HighMax::update(void)
{
	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		cout << "등장" << endl;
	}

	if (KEYMANAGER->isOnceKeyDown('N'))
	{
		cout << "시즈샷" << endl;
		siegeShoot();
	}

	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		cout << "데스볼" << endl;
		deathBall();
	}

	setBossHitbox();
	bStatus.bImage->play(animSpeed);
}

void HighMax::bossAppearance(void)
{

}

void HighMax::siegeShoot(void)
{
	// 불렛 만들기
	ShootEvent shootEvent;
	shootEvent.bType = BulletType::SiegeShoot;
	shootEvent.x = pos.x;
	shootEvent.y = pos.y - status.height;
	shootEvent.direct = status.lookRight;
	shootEvent.velocityX = normalize(getDiffPlayer(shootEvent.x, shootEvent.y)).x;
	shootEvent.velocityY = normalize(getDiffPlayer(shootEvent.x, shootEvent.y)).y;

	EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });
}

void HighMax::deathBall(void)
{
	ShootEvent shootEvent;
	shootEvent.bType = BulletType::DeathBall1;
	shootEvent.x = pos.x + status.width / 2;
	shootEvent.y = pos.y - status.height / 2;
	shootEvent.direct = status.lookRight;
	shootEvent.velocityX = normalize(getDiffPlayer(shootEvent.x, shootEvent.y)).x;
	shootEvent.velocityY = normalize(getDiffPlayer(shootEvent.x, shootEvent.y)).y;

	EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });
}

void HighMax::patternShoot(void)
{

}

ShootEvent HighMax::makeShootEvent(BulletType bType)
{
	ShootEvent shootEvent;
	shootEvent.bType = bType;
	shootEvent.x = pos.x;
	shootEvent.y = pos.y - status.height;
	shootEvent.direct = status.lookRight;
	shootEvent.velocityX = normalize(getDiffPlayer(pos.x, shootEvent.y)).x;
	shootEvent.velocityY = normalize(getDiffPlayer(pos.x, shootEvent.y)).y;

	EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });

	return shootEvent;
}

