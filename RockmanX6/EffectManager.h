#pragma once
#include "SingletonBase.h"
#include "Effect.h"

class EffectManager : public SingletonBase<EffectManager>
{
private:

	Effect* effect;

	typedef	vector<Effect*> vEffect;
	typedef vector<Effect*>::iterator viEffect;
private:

	vEffect _vEffect;
	viEffect _viEffect;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(HDC hdc);

	void spawnEffect();
};

