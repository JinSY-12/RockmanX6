#pragma once
#include "ObjectBase.h"

class BossGate : public ObjectBase
{
private:
	enum DoorState
	{
		Opening,
		Opened,
		Closing,
		Closed
	}state;


	int left;
	int right;
	int top;
	int bottom;

	bool openClose;

	float closeTimer;

public:
	HRESULT init(int x, int y, int locationX, int locationY) override;

	void release(void) override;
	void update(void) override;
	void render(HDC hdc) override;

	void animOncePlay(bool play) override;
};

