SetCapsLockState "AlwaysOff"

global x := 0
global y := 1
return

LButton::return

CapsLock & LButton::
{
  global x, y
  if (y = 1)
  {
    SetTimer ResetCount, 30000
    y := 0
  }
  if (x < 4) {
    Click "Down"
    KeyWait "LButton"
    Click "Up"
    x += 1
    Sleep 1000
  }
  else if (x >= 4) {
    Click "Down"
    KeyWait "LButton"
    Click "Up"
    x += 1
    Sleep 3000
  }
  return
}

ResetCount() {
  global x, y
  x := 0
  y := 1
}
