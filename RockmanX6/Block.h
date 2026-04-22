#pragma once
#include "ObjectBase.h"

class Block : public ObjectBase
{
private:

public:
	HRESULT init(void) override;
	HRESULT init(int x, int y) override;
	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;

	// void checkPlayerAttCollision(void) override;
};

