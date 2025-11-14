#pragma once
#include "GameNode.h"
#include "BulletType.h"
#include "ObjectType.h"

class BulletManager;
class Player;

class ObjectBase : public GameNode
{
private:

public:
	struct ObjectPos
	{
		int x;
		int y;
	};

	struct ObjectState
	{
		ObjectType oType;
		GImage* oImage;
		GImage* oPixel;

		RECT oHitBox;
		RECT worldRect;

		int hp;
		int maxHp;

		int width;
		int height;

		bool dead;
	};

public:

	ObjectPos oPos;
	ObjectState oStatus;

	BulletManager* oBManager;
	Player* oPlayer;

public:

	virtual void render(void);

	virtual void setObjectHitbox(void);

	// 공격, 피격 체크
	inline void settingBulletManager(BulletManager* bullet) { oBManager = bullet; }
	inline void settingPlayer(Player* p) { oPlayer = p; }

	virtual void checkPlayerAttCollision(void);
	virtual void checkWallCollision(void);

	// getter,setter
	inline RECT getObjectRect(void) { return oStatus.worldRect; }
	inline RECT getObjectHitbox(void) { return oStatus.oHitBox; }

	inline bool getIsDead(void) { return oStatus.dead; }
	void checkDead(void);
	
};

