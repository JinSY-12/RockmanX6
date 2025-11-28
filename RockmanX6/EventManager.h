#pragma once
#include "SingletonBase.h"
#include "IEventListener.h"
#include "Event.h"
// #include "BulletManager.h"

class EventManager : public SingletonBase<EventManager>
{
private:
	vector<IEventListener*> _vListeners;

public:
	void addListener(IEventListener* listener);
	void dispatchEvents(const Event& event);
};

