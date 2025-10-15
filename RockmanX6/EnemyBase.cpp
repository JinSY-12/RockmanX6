#include "Stdafx.h"
#include "EnemyBase.h"

HRESULT EnemyBase::init(void)
{
	return S_OK;
}

HRESULT EnemyBase::init(int x, int y)
{
	return S_OK;
}

void EnemyBase::release(void)
{
}

void EnemyBase::update(void)
{
}

void EnemyBase::render(void)
{
}

void EnemyBase::attack(void)
{
}

void EnemyBase::pattern(void)
{
	if (!eStatus.isAtt)
	{
		patternTimer += 0.1f;

		if (patternTimer >= maxPatternTime)	attackAble = true;
	}
}
