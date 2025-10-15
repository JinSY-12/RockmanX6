#pragma once

class Effect
{
private:
	GImage* mEffcect;

public:
	
	HRESULT init(void);
	void update(void);
	void render(HDC hdc);
};

