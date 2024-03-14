#NoEnv
SendMode Input
SetCapsLockState, AlwaysOff

LButton::return

CapsLock & LButton::
  if (GetKeyState("LButton", "P")) {
    Click down
    KeyWait, LButton
    Click up
  } else {
    Send, {LButton}
  }
  return

MButton::return

CapsLock & MButton::Send, {MButton}

Wheelup::return

CapsLock & Wheelup::Wheelup

Wheeldown::return

CapsLock & Wheeldown::Wheeldown

