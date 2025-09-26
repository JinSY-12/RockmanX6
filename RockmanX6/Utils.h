#pragma once

// 디그리 1도의 라디안
#define DEG_TO_RAD		0.017453f

// 파이
#define PI				3.141592653f
// 2파이
#define PI_2			6.28318306f

#define FLOAT_EPSILON		0.001f

// 실수 -> 정수
#define FLOAT_TO_INT(f1)	static_cast<int>(f1 + FLOAT_EPSILON)

// 두 실수가 같은지 비교
#define FLOAT_EQUAL(f1, f2)	(fabs(f1 - f2) <= FLOAT_EPSILON)

namespace MY_UTIL
{
	// 두 점 사이의 거리
	float getDistance(float startX, float startY, float endX, float endY);

	// 스타트 -엔드 각을 라디안으로
	float getAngle(float startX, float startY, float endX, float endY);
}
