#NoEnv

WinSet, Transparent, 0, ahk_class Shell_TrayWnd
return

^0::
    ;Sleep, 5000
    WinSet, Transparent, 255, ahk_class Shell_TrayWnd
    WinSet, TransColor, OFF, ahk_class Shell_TrayWnd
    WinSet, Transparent, OFF, ahk_class Shell_TrayWnd
    WinSet, Redraw,, ahk_class Shell_TrayWnd
    Sleep, 10000
    WinSet, Transparent, 0, ahk_class Shell_TrayWnd
return
