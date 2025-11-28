#include "Stdafx.h"
#include "CombatEntity.h"

void CombatEntity::reduceHp(int damage)
{
	cout << status.hp << endl;
	status.hp -= damage;
	status.overpower = true;
}
