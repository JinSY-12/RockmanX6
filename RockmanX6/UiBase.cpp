#include "Stdafx.h"
#include "UiBase.h"

HRESULT UiBase::init(UiType uType)
{
	return S_OK;
}

HRESULT UiBase::init(UiType uType, int sceneNum)
{
	return S_OK;
}

HRESULT UiBase::init(int x, int y)
{
	return S_OK;
}

HRESULT UiBase::init(PlayerType pType)
{
	return S_OK;
}

HRESULT UiBase::init(BossType bType)
{
	return S_OK;
}

HRESULT UiBase::init(PlayerType pType, BossType bType)
{
	return S_OK;
}

void UiBase::release()
{

}

void UiBase::update()
{
	// Do Nothing
}


void UiBase::render(HDC hdc)
{
	// if(isUiMode) 
}

