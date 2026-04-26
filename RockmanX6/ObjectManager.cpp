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
	for (auto object = _vObject.begin(); object != _vObject.end();++object)
	{
		(*object)->update();
	}

	if (_vObject.size() > 0)
	{
		checkObjectDead();
	}
}

void ObjectManager::render(void)
{
	for (auto object = _vObject.begin(); object != _vObject.end(); ++object)
	{
		(*object)->render(getMemDC());
	}
}

void ObjectManager::spawnObject(ObjectType oType, int x, int y, int width, int top, int height)
{
	switch(oType)
	{
	case ObjectType::Block:
		_object = new Block;
		_object->init(x, y);
		_vObject.push_back(_object);
		break;
	case ObjectType::BossGate:
		_object = new BossGate;
		_object->init(x, y, width, top, height);
		_vObject.push_back(_object);
		break;
	case ObjectType::Ladder:
		_object = new Ladder;
		_object->init(x, y);
		_vObject.push_back(_object);
		break;
	}
}

void ObjectManager::checkObjectDead(void)
{
	for (auto object = _vObject.begin(); object != _vObject.end();)
	{
		if ((*object)->getIsDead())
		{
			playObjectExplodeEffect((*object)->getObjectType(), (*object)->getPos().x, (*object)->getPos().y,
				(*object)->getObjectWidth(), (*object)->getObjectHeight(), 0);
			SOUNDMANAGER->play("SFX_SmallExplosion", 0.5f);

			object = _vObject.erase(object);
		}

		else ++object;
	}
}

void ObjectManager::playObjectExplodeEffect(ObjectType oType, int x, int y, int width, int height, int look)
{
	switch (oType)
	{
	case ObjectType::Block:
		EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb, x, y, width, height, look);
		break;
	default:
		break;
	}

}



