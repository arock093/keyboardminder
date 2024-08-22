#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <sstream>
#include <vector>
#include <filesystem>

#undef max

const std::wstring processNames[3] = { L"DisableTaskbar-x64.exe", L"RequireCapsLock.exe", L"TimeBetweenClicks.exe" };

std::wstring GetProcessName(DWORD processID) {
    TCHAR processName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));
        }
    }

    CloseHandle(hProcess);

    return processName;
}

bool IsProcessRunning(const std::wstring& processNameToCheck) {
    DWORD processes[1024], cbNeeded, processCount;

    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
        std::cerr << "Failed to enumerate processes." << std::endl;
        return false;
    }

    processCount = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < processCount; ++i) {
        if (processes[i] != 0) {
            std::wstring processName = GetProcessName(processes[i]);
            if (processName == processNameToCheck) {
                return true;
            }
        }
    }

    return false;
}

DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &processEntry)) {
        do {
            if (processName == processEntry.szExeFile) {
                pid = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return pid;
}

int StartProcess(std::wstring processName)
{
    if (processName == L"DisableTaskbar" || processName == L"DisableTaskbar.exe") {
        processName = L"DisableTaskbar-x64.exe";
    }
    else if (processName == L"RequireCapsLock")
        processName = L"RequireCapsLock.exe";
    else if (processName == L"TimeBetweenClicks")
        processName = L"TimeBetweenClicks.exe";

    if (!std::filesystem::exists(processName))
    {
        std::wcout << processName + L" not found. Tool needs to be in same directory as ToolManager.exe" << std::endl;
        return 1;
    }

    if (processName != L"DisableTaskbar-x64.exe" && processName != L"RequireCapsLock.exe" && processName != L"TimeBetweenClicks.exe")
    {
        std::wcout << processName + L" is not a valid tool name." << std::endl;
        return 1;
    }
    
    if (!IsProcessRunning(processName))
    {
        if (processName == L"DisableTaskbar-x64.exe")
        {

            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if (!CreateProcess(processName.c_str(),
                NULL,
                NULL,
                NULL,
                FALSE,
                CREATE_NEW_CONSOLE,
                NULL,
                NULL,
                &si,
                &pi)
                ) {
                std::cerr << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
                return 1;
            }

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else
            ShellExecute(NULL, L"open", processName.c_str(), NULL, NULL, SW_HIDE);
    }
    else
        std::wcout << processName + L" is already running." << '\n';

    return 0;
}

int StopProcess(std::wstring processName)
{
    if (processName == L"DisableTaskbar" || processName == L"DisableTaskbar.exe") {
        processName = L"DisableTaskbar-x64.exe";
    }
    else if (processName == L"RequireCapsLock")
        processName = L"RequireCapsLock.exe";
    else if (processName == L"TimeBetweenClicks")
        processName = L"TimeBetweenClicks.exe";

    if (processName != L"DisableTaskbar-x64.exe" && processName != L"RequireCapsLock.exe" && processName != L"TimeBetweenClicks.exe")
    {
        std::wcout << processName + L" is not a valid tool name." << std::endl;
        return 1;
    }

    if (processName == L"DisableTaskbar-x64.exe")
    {
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
        SetLayeredWindowAttributes(taskbar, 0, 255, LWA_ALPHA);
    }

    DWORD pid = GetProcessIdByName(processName);
    if (pid == 0) {
        std::cerr << "Process not found." << std::endl;
        return 1;
    }

    HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (processHandle == NULL) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    if (!TerminateProcess(processHandle, 0)) {
        std::cerr << "Failed to terminate process. Error: " << GetLastError() << std::endl;
        CloseHandle(processHandle);
        return 1;
    }

    std::wcout << "Process terminated successfully." << std::endl;
    CloseHandle(processHandle);

    return 0;
}

int GetOption(int numOptions) {
    int option;
    std::cin >> option;
    if (numOptions == 2)
    {
        while (option != 1 && option != 2)
        {
            std::cout << "That is not an option. Please try again.\n";
            std::cin >> option;
        }
    }
    else if (numOptions == 3)
    {
        while (option != 1 && option != 2 && option != 3)
        {
            std::cout << "That is not an option. Please try again.\n";
            std::cin >> option;
        }
    }
    return option;
}

void SSPTools(std::wstring command, std::vector<std::wstring>& tools) {

    for (int i = 0; i < 3; i++)
    {
        if ((command == L"Start" && !IsProcessRunning(processNames[i])) || (command != L"Start" && IsProcessRunning(processNames[i])))
        {
            std::string sOption = "";
            std::wcout << command + L" " + processNames[i] + L"?\nY / N\n";
            std::cin >> sOption;
            while (sOption != "Y" && sOption != "y" && sOption != "N" && sOption != "n")
            {
                std::cout << "That is not an option. Please try again.\n";
                std::cin >> sOption;
            }
            if (sOption == "Y" || sOption == "y")
            {
                //if (command == L"Start")
                //    StartProcess(processNames[i]);
                if (command == L"Stop" || command == L"Pause")
                    StopProcess(processNames[i]);

                tools.push_back(processNames[i]);
            }
            else if (sOption == "N" || sOption == "n")
            {
                continue;
            }
        }
    }
}

void StartTools() {

    std::vector<std::wstring> tools;
    std::cout << "1) Start all tools.\n2) Start some tools.\n";
    int option = GetOption(2);
    int duration = 0;
    std::cout << "How long do you want to run tools for? Enter an amount in minutes or nothing to run continuously: ";
    //std::cin >> duration;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string in;
    std::getline(std::cin, in);
    if (in == "\n" || in == "")
        duration = 0;
    else
    {
        std::istringstream ss(in);
        int x;
        if (!(ss >> x)) {
            std::cerr << "Invalid number: " << in << '\n';
        }
        else if (!ss.eof()) {
            std::cerr << "Trailing characters after number: " << in << '\n';
        }
        duration = x;
    }
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            StartProcess(processNames[i]);
        }
    }
    else if (option == 2)
    {
        SSPTools(L"Start", tools);
        for (int i = 0; i < tools.size(); i++)
        {
            StartProcess(tools[i]);
        }
    }
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    Sleep(duration * 60000);

    if (tools.empty() && duration != 0)
    {
        for (int i = 0; i < 3; i++)
        {
            StopProcess(processNames[i]);
        }
    }
    else if (!tools.empty() && duration != 0)
    {
        for (int i = 0; i < tools.size(); i++)
        {
            StopProcess(tools[i]);
        }
    }
}

void PauseTools() {

    std::vector<std::wstring> tools;
    std::cout << "1) Pause all tools.\n2) Pause some tools.\n";
    int option = GetOption(2);
    int duration = 0;
    std::cout << "How long do you want to pause tool(s) for? In minutes: ";
    std::cin >> duration;
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            StopProcess(processNames[i]);
        }
    }
    else if (option == 2)
    {
        SSPTools(L"Pause", tools);
    }

    ShowWindow(GetConsoleWindow(), SW_HIDE);
    Sleep(duration * 60000);
    if (tools.empty() && duration != 0)
    {
        for (int i = 0; i < 3; i++)
        {
            StartProcess(processNames[i]);
        }
    }
    else if (!tools.empty() && duration != 0)
    {
        for (int i = 0; i < tools.size(); i++)
        {
            StartProcess(tools[i]);
        }
    }
}

void StopTools() {

    std::vector<std::wstring> tools;
    std::cout << "1) Stop all tools.\n2) Stop some tools.\n";
    int option = GetOption(2);
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            StopProcess(processNames[i]);
        }
    }
    else if (option == 2)
    {
        SSPTools(L"Stop", tools);
    }
}

void RestartTools() {

    std::vector<std::wstring> tools;
    std::cout << "1) Restart all tools.\n2) Restart some tools.\n";
    int option = GetOption(2);
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            StopProcess(processNames[i]);
            Sleep(1000);
            StartProcess(processNames[i]);
        }
    }
    else if (option == 2)
    {
        SSPTools(L"Restart", tools);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        int i = 1;
        int duration = 0;
        int sleep = 0;
        std::string command = "";
        std::vector<std::wstring> tools;
        bool checkForToolNames = false;

        while (i < argc) {
            if (strcmp(argv[i], "start") == 0)
            {
                command = "start";
                checkForToolNames = true;
            }
            else if (strcmp(argv[i], "pause") == 0)
            {
                command = "pause";
                checkForToolNames = true;
            }
            else if (strcmp(argv[i], "stop") == 0)
            {
                command = "stop";
                checkForToolNames = true;
            }
            else if (strcmp(argv[i], "restart") == 0)
            {
                command = "restart";
                checkForToolNames = true;
            }
            else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--duration") == 0)
            {
                std::istringstream ss(argv[i + 1]);
                int x;
                if (!(ss >> x)) {
                    std::cerr << "Invalid number: " << argv[1 + 1] << '\n';
                }
                else if (!ss.eof()) {
                    std::cerr << "Trailing characters after number: " << argv[1 + 1] << '\n';
                }
                duration = x;
                i++;
            }
            else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            {
                std::cout <<
                    "ToolManager:\n" <<
                    "Usage: ToolManager COMMAND [Tool Name(s)]... [OPTIONS]\n" <<
                    "Description:\nUsed to manage keyboardminder tools.\n" <<
                    "Commands:\nstart Start tool(s). Duration option can be included to run tool(s) temporally.\n" <<
                    "pause Pause tool(s) temporally.\n" <<
                    "stop Stop tool(s).\n" <<
                    "restart Restart tool(s).\n" <<
                    "Options:\n-d, --duration The duration in minutes.\n" <<
                    "Examples:\nToolManager start\nToolManager start -d 10\nToolManager stop DisableTaskbar\nToolManager pause RequireCapsLock --duration 20\nToolManager restart DisableTaskbar\nIf no tools are listed after command, it will take effect on all of them.";
            }
            else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sleep") == 0)
            {
                std::istringstream ss(argv[i + 1]);
                int x;
                if (!(ss >> x)) {
                    std::cerr << "Invalid number: " << argv[1 + 1] << '\n';
                }
                else if (!ss.eof()) {
                    std::cerr << "Trailing characters after number: " << argv[1 + 1] << '\n';
                }
                sleep = x;
                i++;
            }
            if (checkForToolNames)
            {
                i++;
                while (i < argc && argv[i][0] != '-')
                {
                    std::string s = argv[i];
                    std::wstring ws(s.begin(), s.end());
                    tools.push_back(ws);
                    i++;
                }
                if (i != argc && argv[i][0] == '-')
                    i--;
                checkForToolNames = false;
            }
            i++;
        }

        if (sleep != 0)
            Sleep(sleep * 60000);

        if (tools.empty())
        {
            if (duration == 0)
            {
                if (command == "start")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        StartProcess(processNames[i]);
                    }
                }
                else if (command == "pause" || command == "stop")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        StopProcess(processNames[i]);
                    }
                }
                else if (command == "restart")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        StopProcess(processNames[i]);
                        Sleep(1000);
                        StartProcess(processNames[i]); 
                    }
                }
            }
            else if (duration != 0)
            {
                if (command == "start")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        StartProcess(processNames[i]);
                    }
                    std::wstring ws = L"stop -s " + std::to_wstring(duration);
                    ShellExecute(NULL, L"open", L"ToolManager.exe", ws.c_str(), NULL, SW_HIDE);
                }
                else if (command == "pause" || command == "stop")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        StopProcess(processNames[i]);
                    }
                    std::wstring ws = L"start -s " + std::to_wstring(duration);
                    ShellExecute(NULL, L"open", L"ToolManager.exe", ws.c_str(), NULL, SW_HIDE);
                }
            }
        }
        else if (!tools.empty())
        {
            if (duration == 0)
            {
                if (command == "start")
                {
                    for (int i = 0; i < tools.size(); i++)
                    {
                        StartProcess(tools[i]);
                    }
                }
                else if (command == "pause" || command == "stop")
                {
                    for (int i = 0; i < tools.size(); i++)
                    {
                        StopProcess(tools[i]);
                    }
                }
                else if (command == "restart")
                {
                    for (int i = 0; i < tools.size(); i++)
                    {
                        StopProcess(processNames[i]);
                        Sleep(1000);
                        StartProcess(processNames[i]);
                    }
                }
            }
            else if (duration != 0)
            {
                std::wstring toolsString;
                if (command == "start")
                {
                    
                    for (int i = 0; i < tools.size(); i++)
                    {
                        StartProcess(tools[i]);
                        toolsString += tools[i] + L" ";
                    }
                    std::wstring ws = L"stop " + toolsString + L"- s " + std::to_wstring(duration);
                    ShellExecute(NULL, L"open", L"ToolManager.exe", ws.c_str(), NULL, SW_HIDE);
                }
                else if (command == "pause")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        StopProcess(processNames[i]);
                        toolsString += tools[i];
                    }
                    std::wstring ws = L"start " + toolsString + L"- s " + std::to_wstring(duration);
                    ShellExecute(NULL, L"open", L"ToolManager.exe", ws.c_str(), NULL, SW_HIDE);
                }

            }
        }
    }
    else
    {
        int mode = 0;
        int count = 0;

        DWORD processes[1024], cbNeeded, processCount;

        if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
            std::cerr << "Failed to enumerate processes." << std::endl;
            return false;
        }

        processCount = cbNeeded / sizeof(DWORD);

        for (unsigned int i = 0; i < processCount; ++i) {
            if (processes[i] != 0) {
                std::wstring processName = GetProcessName(processes[i]);
                if (processName == processNames[0] || processName == processNames[1] || processName == processNames[2]) {
                    count++;
                }
            }
        }

        if (count > 0 && count < 3)
            mode = 1;
        else if (count == 3)
            mode = 2;

        int option;
        std::vector<std::wstring> tools;
        std::cout << "Type 1, 2 or 3 and press Enter to select an option\n";
        if (mode == 0)
        {   
            StartTools();
        }
        else if (mode == 1)
        {
            std::cout << "1) Start tool(s).\n2) Pause/Stop tool(s).\n3) Restart tool(s).\n";
            option = GetOption(3);
            if (option == 1)
            {
                StartTools();
            }
            else if (option == 2)
            {
                std::cout << "1) Pause tool(s).\n2) Stop tool(s).\n";
                option = GetOption(2);
                if (option == 1)
                {
                    PauseTools();
                }
                else if (option == 2)
                {
                    StopTools();
                }
            }
            else if (option == 3)
            {
                RestartTools();
            }
        }
        else if (mode == 2)
        {
            std::cout << "1) Pause tool(s).\n2) Stop tool(s).\n3) Restart tool(s).\n";
            option = GetOption(3);
            if (option == 1)
            {
                PauseTools();
            }
            else if (option == 2)
            {
                StopTools();
            }
            else if (option == 3)
            {
                RestartTools();
            }
        }
    }
}
