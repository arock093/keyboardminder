#include <windows.h>
#include <dwmapi.h>
#include <iostream>
#pragma comment(lib, "dwmapi.lib")

int main()
{

    if (!RegisterHotKey(NULL, 1, MOD_CONTROL, 0x49))
    {
        std::cout << "Failed to register hot key 1" << std::endl;
    }

    MSG msg = { 0 };

    HWND taskbar = FindWindow(L"Shell_TrayWnd", NULL);
    if (!taskbar) {
        MessageBox(NULL, L"Failed to find taskbar", L"Error", MB_OK);
        return -1;
    }

    LONG_PTR style = GetWindowLongPtr(taskbar, GWL_EXSTYLE);

    if (!(style & WS_EX_LAYERED)) {
        SetWindowLongPtr(taskbar, GWL_EXSTYLE, style | WS_EX_LAYERED);
    }

    BYTE alpha = 0;
    SetLayeredWindowAttributes(taskbar, 0, alpha, LWA_ALPHA);

    while (true) {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            if (msg.message == WM_HOTKEY)
            {
                if (msg.wParam == 1) {
                    //Sleep(5000);
                    SetLayeredWindowAttributes(taskbar, 0, 255, LWA_ALPHA);
                    Sleep(10000);
                    SetLayeredWindowAttributes(taskbar, 0, 0, LWA_ALPHA);
                }
            }
        }
        Sleep(500);
    }
}
