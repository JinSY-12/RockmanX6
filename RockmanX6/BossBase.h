#pragma once
#include "BulletType.h"
#include "CombatEntity.h"
#include "ProgressBar.h"

#define BOSS_PATTERN_MAXLIST 5

class Player;

class BossBase : public CombatEntity
{

protected:
	enum class BossState
	{
		Apperance,
		Idle,
		Move,
		Return,
		Dodge,
		Attack,
		Dead,
		Damaged
	};

	enum class AnimDirection
	{
		Forward,
		Backward
	};

	struct FirePointOffset
	{
		int x;
		int y;
	};

	struct BossStatus
	{
		GImage* bImage;
		GImage* effectImage;

		RECT bHitBox;
		RECT effectRect;

		int originX;
		int originY;

		int offsetX;
		int offsetY;

		int effectOffsetX;
		int effectOffsetY;

		bool effectOn;
		bool effectCollisionOn;
		bool effectOnTop;
		bool doCutScene;

		bool movable;

		float invincibleTimer = 0.0f;
		float invincibleMaxTime;

		bool invincible = false;
	};

	struct BossTimer
	{
		float timer = 0.0f;

		bool update(float time)
		{
			timer += 0.1f;

			if (timer >= time)
			{
				timer = 0.0f;
				return true;
			}
			else
				return false;
		}

		void reset(void)
		{
			timer = 0.0f;
		}

		// 이펙트 전용
		int count = 0;
		float effectSpeed = 5.0;
	};

	struct SoundPlayOnce
	{
		bool playDone;

		void play(string name, float volume)
		{
			if (!playDone)
			{
				SOUNDMANAGER->play(name, volume);
				playDone = true;
			}
		}

		void playReset(void)
		{
			playDone = false;
		}
	};

protected:
	Player* player;
	ShootEvent shootEvent;
	BossStatus bStatus;
	BossState bState;

	BossType btype;
	AnimDirection animDir;

	ProgressBar hpBar;
	Vector2 diff;

	BossTimer patternTimer;
	BossTimer effectTimer;

	SoundPlayOnce soundSupport;

	string BossBGM;
	bool musciStart;
	bool gameStart;

	float animSpeed;

	bool isPattern;
	bool appearanceDone;
	int parrternList[BOSS_PATTERN_MAXLIST];

	float patternTime;
	float pattenrCoolDown;
	bool phase2; // 2페이즈로 패턴 타이머와 애니메이션 속도 빠르게
	float timer;

	// 데미지 누적 변수
	int stockDamage;
	int knockOutCount;

	int bossAlpha;
	bool bossClear;
	// test
	bool attCycle;	

public:
	virtual void render(HDC hdc);

	// get/set 관련
	RECT getBossHitBox() { return bStatus.bHitBox; }
	RECT getBossSubHitBox() { return bStatus.effectRect; }

	bool getSubRectAttOn() { return bStatus.effectCollisionOn; }
	BossType getBossType() { return btype; }

	// 패턴 관련
	virtual void bossAppearance();
	virtual void setBossHitbox(void);
	virtual void offsetFix(void);

	Vector2 getDiffPlayer(int firePointX, int firePointY);

	inline void settingPlayer(Player* p) { player = p; }
	void makeShootEvent(BulletType bType);

	virtual void changeAnim(BossState bossState);
	virtual void readyPattern(void);

	// 상태 관련
	inline void reduceHp(int damage)
	{
		status.hp -= damage;
		stockDamage += damage;

		if (status.hp > 0)
		{
			if (damage < 4) bStatus.invincibleMaxTime = 8.0f;
			else bStatus.invincibleMaxTime = 15.0f;
		}
		
		else
		{
			status.hp = 0;
			bossAlpha = 255;
			bState = BossState::Dead;
		}
		
		status.overpower = true;
	}

	void deadAnim(void);

	void bossInvincibleTimerUpdate(void);

	void battleStart(void);
};

