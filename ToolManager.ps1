Add-Type -AssemblyName System.Windows.Forms

<#
This script is to demonstrate how the tool(s) can be managed
Mode 1 - Have tool(s) running all the time and temporarly disable
If using mode 1, add tools to startup folder
Mode 2 - Run tool(s) for a certain amount of time
#>

$mode = 1

if ($mode -eq 1) 
{
  $duration = Read-Host "How long to pause tool for? In minutes"
  [System.Windows.Forms.SendKeys]::SendWait("^{0}")
  Start-Sleep 1
  Get-Process DisableTaskbar-x64 | Stop-Process
  Start-Sleep ([int]$duration * 60)
  Start-Process DisableTaskbar-x64.exe
}
elseif ($mode -eq 2) 
{
  $duration = Read-Host "How long to run tool for? In minutes"
  Start-Process DisableTaskbar-x64.exe
  Start-Sleep ([int]$duration * 60)
  [System.Windows.Forms.SendKeys]::SendWait("^{0}")
  Get-Process DisableTaskbar-x64 | Stop-Process
}

<#
Potential Ideas
- Pause/Run tool(s) on schedule
- Pause/Run tool(s) when certain app(s) open
#>

 