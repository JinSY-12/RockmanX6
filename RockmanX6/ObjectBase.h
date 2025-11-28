#pragma once
#include "GameNode.h"
#include "BulletType.h"
#include "ObjectType.h"
#include "CombatEntity.h"

class BulletManager;
class Player;

class ObjectBase : public CombatEntity
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

		int width;
		int height;
	};

public:

	ObjectPos oPos;
	ObjectState oStatus;

	BulletManager* oBManager;
	Player* oPlayer;


public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void setObjectHitbox(void);

	// 공격, 피격 체크
	inline void settingBulletManager(BulletManager* bullet) { oBManager = bullet; }
	inline void settingPlayer(Player* p) { oPlayer = p; }

	virtual void checkPlayerAttCollision(void);
	virtual void checkBulletCollision(void);
	virtual void checkWallCollision(void);

	// getter,setter
	inline RECT getObjectRect(void) { return oStatus.worldRect; }
	inline RECT getObjectHitbox(void) { return oStatus.oHitBox; }
	inline ObjectPos getObjectPos(void) { return oPos; }

	inline int getObjectWidth(void) { return oStatus.width; }
	inline int getObjectHeight(void) { return oStatus.height; }

	inline bool getIsDead(void) { return status.dead; }
	void checkDead(void);
	
};

