#pragma once
#include "SingletonBase.h"
#include "BossType.h"
#include "EffectType.h"

class EffectManager : public SingletonBase<EffectManager>
{
private:
	struct Effect
	{
		GImage* image;
		int x;
		int y;
		bool direct;

		int startX;
		int startY;

		int velocityY;

		int gravity;
	};

	typedef	vector<Effect> vEffect;
	typedef vector<Effect>::iterator viEffect;

	typedef	vector<Effect> vFragments;
	typedef vector<Effect>::iterator viFragments;

private:
	Effect effect;

	vEffect _vEffect;
	viEffect _viEffect;

	vFragments _vFragments;
	viFragments _viFragments;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void spawnEffect(EffectType eType, int x, int y, bool direct);
	void SpawnFragments(EnemyType type, int x, int y);

	void moveFragments();
};

