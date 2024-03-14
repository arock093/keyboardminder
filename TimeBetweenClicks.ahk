#NoEnv
SendMode Input
SetCapsLockState, AlwaysOff

x := 0
y := 1
return

LButton::return

CapsLock & LButton::
  if (y = 1)
  {
    SetTimer, ResetCount, 30000
    y = 0
  }
  if (x < 4) {
    Click down
    KeyWait, LButton
    Click up
    x += 1
    Sleep 1000
  }
  else if (x >= 4) {
    Click down
    KeyWait, LButton
    Click up
    x += 1
    Sleep 3000
  }
return

ResetCount:
  x = 0
  y = 1

