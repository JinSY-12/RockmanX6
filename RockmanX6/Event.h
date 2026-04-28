#pragma once
#include "BulletType.h"
#include "EffectType.h"
#include "CombatEntity.h"
#include <string>

class Bullet;

enum class EventType
{
	ShootBulltet,
	BulletHitEnemy,
	BulletErase,
	PlayEffectSound,
	PlayVoiceSound,
	StopSound,
	TouchDamage,
	BulletDamage,
};

enum class EntityType
{
	Player,
	Enemy,
	Object,
	Bullet
};

struct Event
{
	EventType eType;
	void* data;
};

struct ShootEvent
{
	BulletType bType;
	int x;
	int y;
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	bool direct;
};

enum class DamageType
{
	Burster,
	ChargeBurst1,
	ChargeBurst2,
	FalconBurst2,
	Saber,
	Touch
};

struct DamageEvent
{
	CombatEntity* attacker;
	CombatEntity* target;
	int damage;
	DamageType dType;
	BulletType bType;
	Bullet* bullet;
	bool targetDead;
};

struct DestroyEvent
{
	EntityType etType;
	int etId;
};

struct SpanwEffectEvent
{
	EffectType type;
	int x;
	int y;
	bool direction;
};