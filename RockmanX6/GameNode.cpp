#include "Stdafx.h"
#include "GameNode.h"



HRESULT GameNode::init(void)
{
    return S_OK;
}

HRESULT GameNode::init(int num)
{
    return S_OK;
}

HRESULT GameNode::init(int x, int y)
{
    return S_OK;
}

HRESULT GameNode::init(PlayerType pType, BossType bType)
{
    return S_OK;
}

HRESULT GameNode::init(bool managerInit)
{
    _hdc = GetDC(_hWnd);
    _managerInit = managerInit;

    if (managerInit)
    {
        setlocale(LC_ALL, "korea");

        SetTimer(_hWnd, 1, 10, NULL);

        RND->init();

        KEYMANAGER->init();

        IMAGEMANAGER->init();

        SOUNDMANAGER->init();

        TIMEMANAGER->init();

        // UIMANAGER->init();

        SCENEMANAGER->init();

        // CAMERAMANAGER->init();

        TIMEMANAGER->init();

        TEXTMANAGER->init();
        
    }

    return S_OK;
}


void GameNode::release(void)
{

    if (_managerInit)
    {
        KillTimer(_hWnd, 1);

        RND->releaseSingleton();

        KEYMANAGER->releaseSingleton();

        IMAGEMANAGER->release();
        IMAGEMANAGER->releaseSingleton();

        SOUNDMANAGER->release();
        SOUNDMANAGER->releaseSingleton();

        TIMEMANAGER->release();
        TIMEMANAGER->releaseSingleton();

        CAMERAMANAGER->release();
        CAMERAMANAGER->releaseSingleton();

        SCENEMANAGER->release();
        SCENEMANAGER->releaseSingleton();

        UIMANAGER->releaseSingleton();

        TEXTMANAGER->release();
        TEXTMANAGER->releaseSingleton();
    }
    ReleaseDC(_hWnd, _hdc);

}

void GameNode::update(void)
{
    InvalidateRect(_hWnd, NULL, false);
}

void GameNode::render(void)
{
    // ! Do Nothing
}

void GameNode::alphaRender(void)
{
    // ! Do Nothing
}

void GameNode::objectRender(void)
{
    // ! Do Nothing
}



// 메인 프로시저
LRESULT GameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (iMessage)
    {

    case WM_TIMER:
        this->update();
        break;

    case WM_CREATE:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        this->render();
        EndPaint(hWnd, &ps);
        break;

    case WM_MOUSEMOVE:
        _ptMouse.x = LOWORD(lParam);
        _ptMouse.y = HIWORD(lParam);
        break;

    case WM_LBUTTONDOWN:
        break;

    case WM_LBUTTONUP:
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            // PostMessage(hWnd, WM_DESTROY, 0, 0);
            //break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}