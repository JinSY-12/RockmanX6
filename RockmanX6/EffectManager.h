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

		int offsetX;
		int offsetY;

		int gravity;
		int velocityY;
		
		float aniSpeed;

		int alpha;
	};

	struct AfterImage
	{
		GImage* image;

		GImage* blue;
		GImage* skyBlue;
		GImage* lightBlue;

		string imageKey;

		int x;
		int y;
		
		int frameX;
		int frameY;

		bool direct;

		// 타이머 - MaxTime를 늘리면 잔상이 오래간다!
		float afterImageTimer;
		float afterImageMaxTime;
	};

	typedef	vector<Effect> vEffect;
	typedef vector<Effect>::iterator viEffect;

	typedef	vector<Effect> vFragments;
	typedef vector<Effect>::iterator viFragments;
	
	typedef deque<AfterImage> dqAfterImages;

private:
	Effect effect;

	vEffect _vEffect;
	viEffect _viEffect;

	vFragments _vFragments;
	viFragments _viFragments;

	dqAfterImages _dqAfterImages;


public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(HDC hdc);
	void afterImageRender(HDC hdc);
	
	void spawnEffect(EffectType eType, int x, int y, int width, int height, bool direct);
	void SpawnFragments(EnemyType type, int x, int y);

	void moveFragments();

	void addDashAfterImage(int x, int y, int frameX, int frameY, bool dir, string imageKey);
	void deleteDashAfterImage(void);
	
	void setAfterImageColor(GImage* img, COLORREF color);
};

