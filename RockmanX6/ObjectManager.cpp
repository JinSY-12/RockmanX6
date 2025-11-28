#include "Stdafx.h"
#include "ObjectManager.h"


HRESULT ObjectManager::init(void)
{

	return S_OK;
}

void ObjectManager::release(void)
{
}

void ObjectManager::update(void)
{
	for (auto object = _vObject.begin(); object != _vObject.end();)
	{
		if ((*object)->getIsDead())
		{
			SOUNDMANAGER->play("SFX_SmallExplosion", 0.5f);
			EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb, (*object)->getObjectPos().x, (*object)->getObjectPos().y, (*object)->getObjectWidth(), (*object)->getObjectHeight(), 0);
			object = _vObject.erase(object);
		}

		else
		{
			(*object)->update();
			++object;
		}
	}

}

void ObjectManager::render(void)
{
	for (auto object = _vObject.begin(); object != _vObject.end(); ++object)
	{
		(*object)->render(getMemDC());
	}
}

void ObjectManager::spawnObject(ObjectType oType, int x, int y)
{
	switch(oType)
	{
	case ObjectType::Block:
		_object = new Block;
		_object->init(x, y);
		_object->settingBulletManager(_bManager);
		_object->settingPlayer(_player);
		_vObject.push_back(_object);
		break;
	}
}



