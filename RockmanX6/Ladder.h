#pragma once
#include "ObjectBase.h"

class Ladder : public ObjectBase
{
private:


public:
	HRESULT init(int x, int y) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;
};

