#pragma once
#include "BulletType.h"
#include "EffectType.h"
#include "CombatEntity.h"
#include <string>

enum class EventType
{
	ShootBulltet,
	BulletHitEnemy,
	BulletDeflected,
	BulletErase,
	PlayEffectSound,
	PlayVoiceSound,
	StopSound,
	Damage
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
	bool direct;
};

struct DamageEvent
{
	CombatEntity* attacker;
	CombatEntity* target;
	int damage;
	BulletType bType;
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