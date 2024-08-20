#include <windows.h>
#include <dwmapi.h>
#include <iostream>
#pragma comment(lib, "dwmapi.lib")
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <vector>
#include <map>
#include <conio.h>

DWORD ParseModifiers(const std::string& modifiers_str)
{
    std::map<std::string, DWORD> modifier_map =
    {
        {"MOD_ALT", MOD_ALT},
        {"MOD_CONTROL", MOD_CONTROL},
        {"MOD_SHIFT", MOD_SHIFT},
        {"MOD_WIN", MOD_WIN}
    };

    DWORD modifiers = 0;
    size_t start = 0;
    size_t end = 0;

    while ((end = modifiers_str.find('|', start)) != std::string::npos)
    {
        std::string mod = modifiers_str.substr(start, end - start);
        modifiers |= modifier_map[mod];
        start = end + 1;
    }

    std::string mod = modifiers_str.substr(start);
    modifiers |= modifier_map[mod];

    return modifiers;
}

void HandleConfigError(HWND& thisHandle, std::string key, std::string value)
{
    ShowWindow(thisHandle, SW_SHOW);
    std::cerr << "Error: " << key << "=" << value << " in config is incorrect.\n";
    _getch();
    exit(0);
}

BOOL MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    int* count = reinterpret_cast<int*>(dwData);
    (*count)++;
    return TRUE;
}

int main()
{

    HWND thisHandle = GetConsoleWindow();

    bool visible = true;
    while (visible)
    {
        ShowWindow(thisHandle, SW_HIDE);
        visible = IsWindowVisible(thisHandle);
    }

    std::string delimiter = "=";
    std::string modifiersStr = "MOD_CONTROL";
    std::string shortcutKey = "0x30";
    int delayAmount = 10;
    std::string line = "False";
    bool extraDelayEnabled = false;

    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    std::wstring directoryPath = std::wstring(buffer).substr(0, pos);
    std::filesystem::path exePath = directoryPath;
    std::filesystem::path filePath = exePath / "DisableTaskbarConfig.txt";
    if (std::filesystem::exists(filePath))
    {

        int pos;
        std::ifstream ConfigFile(filePath);

        getline(ConfigFile, modifiersStr);
        pos = modifiersStr.find(delimiter);
        modifiersStr = modifiersStr.substr(pos + 1, modifiersStr.length());
        DWORD modifiers = 0;
        size_t start = 0;
        size_t end = 0;
        while ((end = modifiersStr.find('|', start)) != std::string::npos)
        {
            std::string mod = modifiersStr.substr(start, end - start);
            if (mod != "MOD_ALT" && mod != "MOD_CONTROL" && mod != "MOD_SHIFT" && mod != "MOD_WIN")
                HandleConfigError(thisHandle, "Modifiers", modifiersStr);
            start = end + 1;
        }
        std::string mod = modifiersStr.substr(start);
        if (mod != "MOD_ALT" && mod != "MOD_CONTROL" && mod != "MOD_SHIFT" && mod != "MOD_WIN")
            HandleConfigError(thisHandle, "Modifiers", modifiersStr);

        getline(ConfigFile, shortcutKey);
        pos = shortcutKey.find(delimiter);
        shortcutKey = shortcutKey.substr(pos + 1, shortcutKey.length());
        if (!(shortcutKey.find("0x") != std::string::npos) || shortcutKey.length() != 4)
            HandleConfigError(thisHandle, "ShortcutKey", shortcutKey);

        getline(ConfigFile, line);
        pos = line.find(delimiter);
        line = line.substr(pos + 1, line.length());
        std::stringstream ss(line);
        ss >> delayAmount;
        if (ss.fail())
        {
            HandleConfigError(thisHandle, "DelayAmount", line);
        }

        getline(ConfigFile, line);
        pos = line.find(delimiter);
        line = line.substr(pos + 1, line.length());
        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) { return std::tolower(c); });
        if (line == "true")
            extraDelayEnabled = true;
        else if (line == "false")
            extraDelayEnabled = false;
        else
            HandleConfigError(thisHandle, "ExtraDelay", line);

        ConfigFile.close();
    }
    else
    {
        std::wcout << L"File does not exist: " << filePath << std::endl;
        std::ofstream file(filePath);
        if (file)
        {
            std::cout << "File created successfully." << std::endl;
        }
        else
        {
            std::cerr << "Failed to create the file." << std::endl;
            return 1;
        }
        file.close();
        std::ofstream ConfigFile(filePath, std::ios::app);
        if (ConfigFile)
        {
            ConfigFile << "Modifiers=MOD_CONTROL" << std::endl;
            ConfigFile << "ShortcutKey=0x30" << std::endl;
            ConfigFile << "DelayAmount=10" << std::endl;
            ConfigFile << "ExtraDelay=False" << std::endl;
            std::cout << "Data written to the file successfully." << std::endl;
        }
        else
        {
            std::cerr << "Failed to open the file for writing." << std::endl;
        }
        ConfigFile.close();
    }

    unsigned long ul = stoul(shortcutKey, nullptr, 0);
    DWORD modifiers = ParseModifiers(modifiersStr);

    if (modifiers == 0 || !RegisterHotKey(NULL, 1, modifiers, ul))
    {
        ShowWindow(thisHandle, SW_SHOW);
        std::cout << "Failed to register hot key 1. Your config file may be incorrect." << std::endl;
    }

    int monitorCount = 0;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorCount));

    HWND taskbar = FindWindow(L"Shell_TrayWnd", NULL);;
    std::vector<HWND> taskbars;

    if (monitorCount == 1)
    {
        if (!taskbar)
        {
            MessageBox(NULL, L"Failed to find taskbar", L"Error", MB_OK);
            return -1;
        }
        LONG_PTR style = GetWindowLongPtr(taskbar, GWL_EXSTYLE);

        if (!(style & WS_EX_LAYERED))
        {
            SetWindowLongPtr(taskbar, GWL_EXSTYLE, style | WS_EX_LAYERED);
        }

        BYTE alpha = 0;
        SetLayeredWindowAttributes(taskbar, 0, alpha, LWA_ALPHA);
    }
    else if (monitorCount > 1)
    {
        HWND primaryTaskbar = FindWindow(L"Shell_TrayWnd", NULL);
        if (primaryTaskbar)
        {
            taskbars.push_back(primaryTaskbar);
        }

        HWND secondaryTaskbar = NULL;
        do
        {
            secondaryTaskbar = FindWindowEx(NULL, secondaryTaskbar, L"Shell_SecondaryTrayWnd", NULL);
            if (secondaryTaskbar)
            {
                taskbars.push_back(secondaryTaskbar);
            }
        } while (secondaryTaskbar != NULL);

        for (HWND hwnd : taskbars)
        {
            LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

            if (!(style & WS_EX_LAYERED))
            {
                SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
            }

            BYTE alpha = 0;
            SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
        }
    }

    MSG msg = { 0 };

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            if (msg.message == WM_HOTKEY)
            {
                if (msg.wParam == 1)
                {
                    if (extraDelayEnabled)
                        Sleep(5000);
                    if (monitorCount > 1)
                    {
                        for (HWND hwnd : taskbars)
                            SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
                        Sleep(delayAmount * 1000);
                        for (HWND hwnd : taskbars)
                            SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
                    }
                    else
                    {
                        SetLayeredWindowAttributes(taskbar, 0, 255, LWA_ALPHA);
                        Sleep(delayAmount * 1000);
                        SetLayeredWindowAttributes(taskbar, 0, 0, LWA_ALPHA);
                    }
                }
            }
        }
        Sleep(500);
    }
}
