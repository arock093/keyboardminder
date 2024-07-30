SetCapsLockState "AlwaysOff"

LButton::return

CapsLock & LButton::
{
  if (GetKeyState("LButton", "P")) {
    Click "Down"
    KeyWait "LButton"
    Click "Up"
  } else {
    Send "{LButton}"
  }
  return
}

MButton::return

CapsLock & MButton::Send "{MButton}"

Wheelup::return

CapsLock & Wheelup::Wheelup

Wheeldown::return

CapsLock & Wheeldown::Wheeldown
