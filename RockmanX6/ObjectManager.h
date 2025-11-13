#pragma once
#include "GameNode.h"
#include "ObjectType.h"
#include "ObjectBase.h"
#include "Block.h"

class Player;
class Bullet;
class BulletManager;

class ObjectManager : public GameNode
{
private:
	
	typedef vector<ObjectBase*> vObjectBase;
	typedef vector<ObjectBase*>::iterator viObjectBase;
	
private:

	vObjectBase _vObject;
	viObjectBase _viObject;
	
	Player* _player;
	ObjectBase* _object;
	BulletManager* _bManager;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void spawnObject(ObjectType oType, int x, int y);

	void setttingPlayer(Player* player) { _player = player; }
	void setttingBulletManager(BulletManager* bManager) { _bManager = bManager; }
};

