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

void ObjectManager::spawnObject(ObjectType oType, int x, int y, int width, int top, int height)
{
	switch(oType)
	{
	case ObjectType::Block:
		_object = new Block;
		_object->init(x, y);
		_vObject.push_back(_object);
		cout << _vObject.size() << endl;
		break;
	case ObjectType::BossGate:
		_object = new BossGate;
		_object->init(x, y, width, top, height);
		// _object->init(x, y, (x + 14) * SCALE_FACTOR, 240 * SCALE_FACTOR);
		_vObject.push_back(_object);
		break;
	}
}



