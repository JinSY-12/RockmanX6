#pragma once
#include "GameNode.h"
#include "EffectType.h"
#include "BulletType.h"

class BulletManager;

class Player : public GameNode
{
private:


public:

#pragma region PlayerType

	enum class CharacterType
	{
		X,
		Zero
	};

	enum class ArmorType
	{
		Normal,
		Falcon,
		Shadow,
		Ultimate,
		BlakcZero
	};

#pragma endregion

#pragma region PlayerStruct
	struct CharcterPos
	{
		int x;
		int y;
	};

	struct Anim
	{
		int x;
		int y;
	};

	struct Offset
	{
		int x;
		int y;
	};

	struct BursterPos
	{
		int x;
		int y;
	};

	enum class SholderState
	{
		None,
		Burst,
		LargeBurst,
		Hold
	};

	// ĳ���� ���°�
	enum class CharacterState
	{
		Idle,
		Walk,
		JumpUp,
		FallingDown,
		Dash,
		DashEnd,
		Crouch,
		Land,
		Climb,
		Warp,
		WallSlide,
		WallKick,
		Dead,
		OverPower
	};

	struct PlayerPalette
	{
		COLORREF base;
		COLORREF afterShadow1;
		COLORREF afterShadow2;
		COLORREF afterShadow3;
		COLORREF charged1;
		COLORREF charged2;
		COLORREF burst;
		COLORREF lowDamaged;
		COLORREF highDamaged;
	};
	
	// ĳ���� ���� ������
	struct PlayerStatus
	{
		// ���� �� �̹���
		GImage* player;
		RECT hitBox;
		RECT floorCheck;

		string charName;
		int firePointX;
		int firePointY;

		// ����
		float hp;
		float maxHp;
		float mp;
		float maxMp;

		// �̵� ����
		float moveSpeed;
		float dashSpeed;
		float jumpPower;
		float velocityX;
		
		// ���°�
		bool invincible;
		bool lookRight;
		bool isOnGround;
		bool isOnTop;
		bool Dead;

		bool touchLeft;
		bool touchRight;
		
		// ���� ����
		float velocityY;
		float maxFallSpeed;
		bool isJumpUp;
		bool isWallKick;
		bool isWallSlide;
		bool wallKickRight;

		// ��� ����
		bool isDash;
		bool isJumpDash;

		// ���� ����
		bool movable;
		float invincibleTimer;
		float invincibleMaxTime;
	};

	struct Progress
	{
		int life;
		int limitLife = 9;
		// �Ƹ� ��Ʈ���� ������ ���, ����, ��, ǲ
		bitset<4> isArmd[2]; // �ƸӴ� �ϳ��� �����Ұ����� �迭 ũ�� 2�� �ּ� �ΰ� ����� ô!

		// ���� ��Ʈ�� ������ ȭ�� ���� ���������� �ð� ����
		bitset<4> clearBoss;
		float gravityAccel;
	};

#pragma endregion

	// ĳ���� �ൿ �� ����
	PlayerStatus pStatus;
	CharacterState previousState;
	CharacterState currentState;
	SholderState attState;

	Progress progress;

	PlayerPalette playerColor[15];

	// ĳ���� ���� �� ��ǥ
	int hitBoxWidth;
	int hitBoxHeight;

	CharcterPos hitBoxCenter;
	CharcterPos charPos;
	Anim animBaseline;
	BursterPos busterPos;

	// �÷��̾� ���°�
	CharacterType charType;
	ArmorType armorType;

	bool inputEnabled;
	bool isMoving;

	// �ִϸ��̼� ����
	Offset animOffset;

	string previousAnim;
	string currentAnim;

	int animDir;
	float animSpeed;
	float delayTimer;
	bool attChange;

	float attackTimer;
	const float attackDuration = 0.4f;
	bool attCheckOnce;

	float now;
	float lastShootTime;
	float shotCoolDown;

	bool burstloop;

	// ���� ����
	string soundResult;

	BulletManager* bManager;

	// Ű �Է� ����
	bool multiInput;
	bool pressLeft;
	bool pressRight;
	bool lastKeyIsRight;

	// ���� ����
	float wallkickTimer;
	float wallkickMaxTime;

	// ��� ����
	float dashTimer;
	float maxDashTime;
	float dashSpeed;
	bool pressDash;
	bool aniDash;

	GImage* afterImage;

	int colorType;
	int colorTimer;

	BulletType bulletType;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	// ĳ���� ���� ����
	virtual void move(bool direction);
	virtual void jump(void);
	virtual void dash(bool direction);
	virtual void attack(void);

	void wallSlide(void);
	void wallDrop(void);
	void wallKick(void);
	
	// ĳ���� ���� ���°�
	void applyForce(void);
	void sfxPlay(void);
	void setBulletManager(BulletManager* manager) { bManager = manager; };

	// ĳ���� ����
	virtual void spawn(int x, int y);

	// ���°�
	string printBodyState(void);
	string printAttState(void);

	// �ִϸ��̼� ����
	virtual void setHitBox(void);
	virtual void currentAnimChange(void);

	// ���� ����
	inline void setStageGravity(float gravityPower) { progress.gravityAccel = gravityPower; }

	// settter/getter
	// ��ǥ �� ����
	RECT getPlayerRect(void) { return pStatus.hitBox; }
	inline int getPlayerCenter(void) { return charPos.x; }
	inline int getPlayerTop(void) { return charPos.y - hitBoxHeight; }
	inline int getPlayerBottom(void) { return charPos.y; }
	inline int getPlayerLeft(void) { return charPos.x - hitBoxWidth / 2; }
	inline int getPlayerRight(void) { return charPos.x + hitBoxWidth /2; }
	
	inline bool getPlayerSight(void) { return pStatus.lookRight; }
	inline bool getOverPower(void) { return pStatus.invincible; }

	// ���°�
	inline void setLeftCollision(bool left, int leftline)
	{
		pStatus.touchLeft = left;

		if (pStatus.touchLeft == true)
		{
			wallkickTimer = 0.0f;
			charPos.x = leftline + hitBoxWidth / 2;

			int left = leftline - CAMERAMANAGER->getCameraPos().x;
			pStatus.hitBox.left = left;
			pStatus.hitBox.right = pStatus.hitBox.left + hitBoxWidth;

			pStatus.velocityX = 0.0f;

			pStatus.isWallKick = false;
			pStatus.isJumpDash = false;
		}
	}
	inline void setRightCollision(bool right, int rightline)
	{
		pStatus.touchRight = right;

		if (pStatus.touchRight == true)
		{
			wallkickTimer = 0.0f;

			charPos.x = rightline - hitBoxWidth / 2;

			int right = rightline - CAMERAMANAGER->getCameraPos().x;
			pStatus.hitBox.right = right;
			pStatus.hitBox.left = pStatus.hitBox.right - hitBoxWidth;
			
			pStatus.velocityX = 0.0f;

			pStatus.isWallKick = false;
			pStatus.isJumpDash = false;
		}
	}
	inline void setIsOnGround(bool OnGround, int topline)
	{
		pStatus.isOnGround = OnGround;
		
		if (pStatus.isOnGround == true)
		{
			charPos.y = topline - 2;

			int bottom = topline - CAMERAMANAGER->getCameraPos().y;
			pStatus.hitBox.bottom = bottom - 2;
			pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;

			pStatus.velocityY = 0.0f;

			if (pStatus.isJumpDash)
			{
				pStatus.isDash = false;
				pStatus.isJumpDash = false;
			}
		}
	}
	inline void setTopCollision(bool top, int bottomline)
	{
		pStatus.isOnTop = top;

		if (top == true)
		{
			charPos.y = bottomline + 3  + hitBoxHeight;

			int top = bottomline - CAMERAMANAGER->getCameraPos().y;
			pStatus.hitBox.top = top + 3 ;
			pStatus.hitBox.bottom = pStatus.hitBox.top + hitBoxHeight;

			pStatus.velocityY = 0.0f;
			pStatus.velocityX = 0.0f;

			wallkickTimer = wallkickMaxTime;
		}
	}
	void setOverPower(bool op, BulletSize bullet);

	Progress getProgress(void) { return progress; }

	CharcterPos& getCharPos(void) { return charPos; }

	// ���� ����
	inline void reduceHp(int damage, BulletSize size) {

		pStatus.hp -= damage;

		if (pStatus.hp > 0)
		{
			int random = RND->getInt(2);
			if (random == 0) SOUNDMANAGER->play("Voice_X_Damaged1");
			else SOUNDMANAGER->play("Voice_X_Damaged2");
			
			currentState = CharacterState::OverPower;
			pStatus.movable = false;
			pStatus.invincible = true;
			animSpeed = 0.06f;

			switch (size)
			{
			case BulletSize::Small:
				// �Ұ���
				changeAnimation(pStatus.charName + "SmallDamaged", 0);

				if(pStatus.lookRight) pStatus.velocityX = -3.0f;
				else pStatus.velocityX = 3.0f;

				pStatus.velocityY = 0.0f;

				break;
			case BulletSize::Large:
				// �����
				changeAnimation(pStatus.charName + "LargeDamaged", 0);

				if (pStatus.lookRight) pStatus.velocityX = -2.0f;
				else pStatus.velocityX = 2.0f;

				pStatus.velocityY = 0.0f;

				break;
			}
		}	

		else 
		{
			currentState = CharacterState::Dead;
			SOUNDMANAGER->play("Voice_" + pStatus.charName + "Dead");
			pStatus.Dead = true;
			pStatus.invincible = true;
			pStatus.movable = false;
		}
	}

	inline void reduceMp(int damage) { pStatus.mp -= damage; }
	inline float lerp(float start, float end, float time)
	{
		return start + (end - start) * time;
	}

	inline void isDead(void)
	{
		if (pStatus.hp <= 0)
		{
			pStatus.hp = 0;
			pStatus.invincible = true;
			pStatus.Dead = false;
		}
	}

	inline void invincibleTimerUpdate()
	{
		if (pStatus.invincible && !pStatus.Dead)
		{
			pStatus.invincibleTimer += 0.1f;
			if (pStatus.invincibleTimer >= pStatus.invincibleMaxTime)
			{
				pStatus.invincible = false;
				pStatus.invincibleTimer = 0.0f;
			}
		}
	}

	virtual void colorSetting(void);
	virtual void colorChange(void);

	void changeAnimation(const string& animName, int frame);
};

