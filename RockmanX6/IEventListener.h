#pragma once
#include "Event.h"

class IEventListener
{
private:
public:
	virtual ~IEventListener() = default;
	virtual void onEvent(const Event& event) = 0;
};

