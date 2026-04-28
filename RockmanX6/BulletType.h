#pragma once

enum class BulletFaction
{
	Player,
	Enemy,
	Object
};

// 플레이어 총알 종류
enum class BulletType
{
	Burster,
	ChargeBurst1,
	ChargeBurst2,
	FalconBurst2,
	JunkBullet,
	SiegeShoot
};

// 경직 관련
enum class BulletSize
{
	Large,
	Small
};