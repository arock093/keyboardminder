# Keyboardminder

This project is the first of many I am working on with each one building on top of each other and increasing in complexity. I usually prototype in PowerShell, Python and AutoHotkey, then write a C++ version once the project is more established.

## Financial Transparency Statement

You can read the financial transparency statement [here](https://github.com/arock093/keyboardminder/blob/main/TRANSPARENCY_STATEMENT.md).

If this project is successfully funded I will be able to release more open source projects.

Please support my work.

[GitHub Sponsors Profile](https://github.com/sponsors/arock093)

## Purpose

This project is the first in a series with the goal of creating a workflow you can use on your computer to get your work done as efficiently as possible. 

The purpose of this specific project is provide a set of tools to help learn to solely use the keyboard for more tasks because it should be more efficient than using a mouse or track pad in a lot of cases. It also lays the groundwork for future projects that help you use the keyboard more efficiently.

While I highly recommend incorporating this workflow into your routine for optimal efficiency in completing tasks, I also encourage you not to limit yourself exclusively to this method. Exploring different workflows can be a rewarding experience, offering valuable insights and potentially contributing to further improvements in this particular workflow. Feel free to experiment and find what works best for you in various contexts, whether at work or during more exploratory computer activities. This project aims to provide a reliable and efficient set of tools for your everyday tasks, but the beauty of technology lies in its diversity, allowing you to adapt and explore as needed.

## Background

I figured out the best way to learn to use the keyboard more is by intentionally using the mouse less. 
This may seem obvious but it can be trickier than you would think to get out of the habit of using the mouse or track pad.

Originally what I tried was placing my mouse further away from myself. What I found was there are some tasks that sometimes require using a mouse and others that require it to be used almost the entire time. This led to me moving the mouse closer for the tasks that required it and this ultimately led to using the mouse more overall again which wasn't my goal. 

What I ended up doing was using AutoHotkey to write some scripts to help. These scripts help limit mouse usage so that you remember to use the keyboard more.

## Description

There are three tools. All three have AutoHotkey script versions and one also has an early C++ version.
### DisableTaskbar
This script disables the task bar so that it is easier to get into the habit of using the keyboard to switch windows.
The task bar can be temporary re-enabled with a shortcut. The default shortcut is Ctrl + 0 and the amount of time before the task bar is hidden again is 10 seconds.
I recommend having this script running most of time as part of the workflow. I find this one extremely useful.

In the script there is a extra sleep that is commented out. Enabling this line can help when your having a difficult time getting out of the habit of using the task bar to switch windows. Another tip is to occasionally change the shortcut if find yourself using it more than you want to.

There is also an early C++ version of this tool. It still needs to be tested more.

### RequireCapsLock
This script makes it so that to use the mouse buttons and mouse wheel, caps lock must be held.
This also helps you be more mindful about mouse usage.
I also recommend having this script running a majority of the time.
### TimeBetweenClicks
You can use this script when your really trying to be more aware and limit mouse usage such as when your learning a new shortcut.
It adds wait times between mouse clicks. After a certain amount of clicks, the wait time is increased. It is reset after a certain amount of time. 
It also requires capslock to be held.
It can be a little frustrating sometimes but it can help you persist in learning more advanced keyboard shortcuts.

#### Mac Version
There are a couple decent solutions for MacOS.
The first is to make the dock as small as possible. You can also move it to the side of your screen.
The second is to set the autohide delay to around 8 seconds or even a longer amount of time.
For when you do still need to access the dock quickly, you can get to it from Mission Control which can also be disabled if desired.
This command will set the autohide delay time in seconds.\
```defaults write com.apple.dock autohide-delay -float 8; killall Dock```\
This command will restore to the default behavior.\
```defaults delete com.apple.dock autohide-delay; killall Dock```\
I may also add a Script to temporary show the dock again using a keyboard shortcut.

Acknowledgement\
The Mac solutions were inspired by a post on Stack Exchange. You can view the original solutions [here](https://apple.stackexchange.com/questions/59556/is-there-a-way-to-completely-disable-dock).

## Installing
First install AutoHotkey to be able to run the scripts.

**Warning**: While AutoHotkey itself should be safe, installing it does increase your attack surface area. The biggest thing is make sure you know what the scripts you run do.

https://www.autohotkey.com/

Next simply download the repository and run the scripts.

## Contributions
Contributions are more than welcome. Open an issue and submit a pull request if you have one. Please be respectful. 

## Roadmap
My goal with this project and future ones is to keep them as simple as possible. I don’t want to add unnecessary features but am more than willing to explore variations to the scripts. If there are any ideas for useful variations to the scripts, a settings panel can be added to the scripts.

Here is a sneek peak from the next project in the series written in C++ which adds some useful keyboard shortcuts.
```
BOOL IsAltTabWindow(HWND hwnd) {...
if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {...
if (hwnd != 0) {
...
else {
  startApplication(appToStart);
}
```

## Funding

If this project is of any use to you or you interested in my future projects, you can help fund my work.

[GitHub Sponsors Profile](https://github.com/sponsors/arock093)

On my sponsors profile you can read more about the overall mission with my projects.

Please read the financial transparency statement [here](https://github.com/arock093/keyboardminder/blob/main/TRANSPARENCY_STATEMENT.md) if you haven't already.

## Contact

You can contact me by email regarding feedback or questions on the project.

xck5s3zyu@mozmail.com
