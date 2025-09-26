#include "Stdafx.h"
#include "MainGame.h"

HINSTANCE _hInstance;

HWND _hWnd;

POINT _ptMouse = { 0, 0 };

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

MainGame* _mg;

void setWindowSize(int x, int y, int width, int height);


int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR _lpszCmdParam,
    int     nCmdShow)
{
    _mg = new MainGame();

    _hInstance = hInstance;

    WNDCLASS wndClass;

    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = (WNDPROC)WndProc;
    wndClass.lpszClassName = WINNAME;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

#ifdef FULLSCREEN           // 영상찍을떄 쓰고 만들때는 값을 봐야해서 창모드로 만들어라

    // 디바이스 모드 구조체 (화면 디스플레이 관련 구조체)
    DEVMODE dm;

    // 메모리가 남아있을수 있으니 한번 메모리를 비우고 시작
    ZeroMemory(&dm, sizeof(DEVMODE));

    dm.dmSize = sizeof(DEVMODE);
    dm.dmBitsPerPel = 32;  // 32비트 트루컬러를 의미 WinAPI32 -> 32비트라서
    dm.dmPanningWidth = 1980;//가로값
    dm.dmPelsHeight = 1020;//세로값
    dm.dmDisplayFrequency = 60; // 주사율

    // 필드 설정
    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

    // 종료되면 디스플레이 세팅을 원래대로 돌린다
    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        ChangeDisplaySettings(&dm, 0);
    }

    _hWnd = CreateWindow
    (
        WINNAME,
        WINNAME,
        WINSTYLE,
        WINSTART_X,
        WINSTART_Y,
        WINSIZE_X,
        WINSIZE_Y,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL
    );


#else

    _hWnd = CreateWindow
    (
        WINNAME,
        WINNAME,
        WINSTYLE,
        WINSTART_X,
        WINSTART_Y,
        WINSIZE_X,
        WINSIZE_Y,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL
    );

#endif
    setWindowSize(WINSTART_X, WINSTART_Y, WINSIZE_X, WINSIZE_Y);

    ShowWindow(_hWnd, nCmdShow);

    if (FAILED(_mg->init()))
    {
        return 0;
    }

    MSG message;

    while (true)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT) break;

            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        else
        {
            TIMEMANAGER->update(60.0f);
            SOUNDMANAGER->update();
        }
    }

    _mg->release();
    SAFE_DELETE(_mg);

    UnregisterClass(WINNAME, hInstance);

    return (int)message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    return _mg->MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
    RECT rc = { 0, 0, width, height };

    AdjustWindowRect(&rc, WINSTYLE, false);

    SetWindowPos(_hWnd, NULL, x, y,
        (rc.right - rc.left),
        (rc.bottom - rc.top),
        SWP_NOZORDER | SWP_NOMOVE);
}

