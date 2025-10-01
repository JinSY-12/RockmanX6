#pragma once
// CommonMacroFunction


inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

inline void DrawRectMake(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2 , y - height / 2 , x + width / 2 , y + height / 2 };
	return rc;
}

inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);

}

inline void DrawRectMakeColor(HDC hdc, RECT rc, COLORREF color, int penWidth)
{
	if (!hdc) return;

	HPEN myPen, oldPen;

	myPen = CreatePen(PS_SOLID, penWidth, color);
	oldPen = (HPEN)SelectObject(hdc, myPen);

	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(myPen);
}

inline void DrawTextColor(HDC hdc, const std::string& text, int x, int y, COLORREF color)
{
	if (!hdc) return;

	// 글자 색상 설정
	SetTextColor(hdc, color);

	// 배경 투명
	SetBkMode(hdc, TRANSPARENT);

	// 글자 위치 출력
	TextOutA(hdc, x, y, text.c_str(), (int)text.length());
}