#pragma once
#include "BulletType.h"
#include "ObjectType.h"
#include "CombatEntity.h"

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
		RECT subRect;

		int width;
		int height;
	};

public:

	ObjectPos oPos;
	ObjectState oStatus;

	bool animPlay;
	bool isUsed;

	// int oLocationX;
	// int oLocationY;

public:
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	void setObjectHitbox(void);
	//virtual void setObjectHitbox(void);
			
	// getter,setter
	inline RECT getObjectRect(void) { return oStatus.worldRect; }
	inline RECT getObjectHitbox(void) { return oStatus.oHitBox; }
	inline ObjectPos getObjectPos(void) { return oPos; }
	inline bool getOjbectIsUsed(void) { return isUsed; }


	inline int getObjectWidth(void) { return oStatus.width; }
	inline int getObjectHeight(void) { return oStatus.height; }

	ObjectType getObjectType(void) { return oStatus.oType; }

	inline void reduceHp(int damage) { status.hp -= damage; }
	
	inline bool getIsDead(void) { return status.dead; }
	void checkDead(void);

	virtual void animOncePlay(bool play);
};

