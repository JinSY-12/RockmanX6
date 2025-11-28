#include "Stdafx.h"
#include "EventManager.h"

void EventManager::addListener(IEventListener* listener)
{
	_vListeners.push_back(listener);
}

void EventManager::dispatchEvents(const Event& event)
{
	for (auto listener : _vListeners)
	{
		listener->onEvent(event);
	}
}
