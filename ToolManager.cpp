#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>

#undef max

const std::wstring processNames[3] = {L"DisableTaskbar-x64.exe", L"RequireCapsLock.exe", L"TimeBetweenClicks.exe"};
bool toolEnabled[3] = { false,false,false };

std::wstring GetProcessName(DWORD processID)
{
    TCHAR processName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));
        }
    }

    CloseHandle(hProcess);

    return processName;
}

bool IsProcessRunning(const std::wstring& processNameToCheck)
{
    DWORD processes[1024], cbNeeded, processCount;

    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded))
    {
        std::cerr << "Failed to enumerate processes." << std::endl;
        return false;
    }

    processCount = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < processCount; ++i)
    {
        if (processes[i] != 0)
        {
            std::wstring processName = GetProcessName(processes[i]);
            if (processName == processNameToCheck)
            {        
                return true;
            }
        }
    }

    return false;
}

DWORD GetProcessIdByName(const std::wstring& processName)
{
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &processEntry))
    {
        do
        {
            if (processName == processEntry.szExeFile)
            {
                pid = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return pid;
}

bool checkProcessName(std::wstring& processName, std::string action = "Start")
{
    if (processName == L"DisableTaskbar" || processName == L"DisableTaskbar.exe") {
        processName = L"DisableTaskbar-x64.exe";
    }
    else if (processName == L"RequireCapsLock")
        processName = L"RequireCapsLock.exe";
    else if (processName == L"TimeBetweenClicks")
        processName = L"TimeBetweenClicks.exe";

    if (action == "Start" && !std::filesystem::exists(processName))
    {
        std::wcout << processName + L" not found. Tool needs to be in same directory as ToolManager.exe" << std::endl;
        return false;
    }

    if (processName != L"DisableTaskbar-x64.exe" && processName != L"RequireCapsLock.exe" && processName != L"TimeBetweenClicks.exe")
    {
        std::wcout << processName + L" is not a valid tool name." << std::endl;
        return false;
    }

    return true;
}

int StartProcess(std::wstring processName)
{
    if (checkProcessName(processName))
    {
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
    }

    return 0;
}

int StopProcess(std::wstring processName)
{
    if (checkProcessName(processName, "Stop"))
    {
        if (IsProcessRunning(processName)) {
            if (processName == L"DisableTaskbar-x64.exe")
            {
                HWND taskbar = FindWindow(L"Shell_TrayWnd", NULL);
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
                SetLayeredWindowAttributes(taskbar, 0, 255, LWA_ALPHA);
            }

            DWORD pid = GetProcessIdByName(processName);
            if (pid == 0)
            {
                std::cerr << "Process not found." << std::endl;
                return 1;
            }

            HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
            if (processHandle == NULL)
            {
                std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
                return 1;
            }

            if (!TerminateProcess(processHandle, 0))
            {
                std::cerr << "Failed to terminate process. Error: " << GetLastError() << std::endl;
                CloseHandle(processHandle);
                return 1;
            }

            std::wcout << "Process terminated successfully." << std::endl;
            CloseHandle(processHandle);
        }
        else
            std::wcout << processName << " is not running." << std::endl;
    }
    return 0;
}

int GetOption(int numOptions = 2)
{
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

void StartTools()
{

    std::vector<std::wstring> tools;
    std::cout << "1) Start all tools.\n2) Start some tools.\n";
    int option = GetOption();
    int duration = 0;
    std::cout << "How long do you want to run tools for? Enter an amount in minutes or nothing to run continuously: ";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string in;
    std::getline(std::cin, in);
    if (in == "\n" || in == "")
        duration = 0;
    else
    {
        std::istringstream ss(in);
        int x;
        if (!(ss >> x))
        {
            std::cerr << "Invalid number: " << in << '\n';
        }
        else if (!ss.eof())
        {
            std::cerr << "Trailing characters after number: " << in << '\n';
        }
        duration = x;
    }
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (toolEnabled[i])
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

void PauseTools()
{

    std::vector<std::wstring> tools;
    std::cout << "1) Pause all tools.\n2) Pause some tools.\n";
    int option = GetOption();
    int duration = 0;
    std::cout << "How long do you want to pause tool(s) for? In minutes: ";
    std::cin >> duration;
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (toolEnabled[i])
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
            if (toolEnabled[i])
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

void StopTools()
{

    std::vector<std::wstring> tools;
    std::cout << "1) Stop all tools.\n2) Stop some tools.\n";
    int option = GetOption();
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (toolEnabled[i])
                StopProcess(processNames[i]);
        }
    }
    else if (option == 2)
    {
        SSPTools(L"Stop", tools);
    }
}

void RestartTools()
{

    std::vector<std::wstring> tools;
    std::cout << "1) Restart all tools.\n2) Restart some tools.\n";
    int option = GetOption();
    if (option == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (toolEnabled[i])
            {
                StopProcess(processNames[i]);
                Sleep(1000);
                StartProcess(processNames[i]);
            }
        }
    }
    else if (option == 2)
    {
        SSPTools(L"Restart", tools);
    }
}

int main(int argc, char* argv[])
{

    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    std::wstring directoryPath = std::wstring(buffer).substr(0, pos);
    std::filesystem::path exePath = directoryPath;
    std::filesystem::path filePath = exePath / "ToolManagerConfig.txt";
    if (std::filesystem::exists(filePath))
    {
        int pos;
        std::ifstream ConfigFile(filePath);
        std::string line = "";
        while (getline(ConfigFile, line))
        {
            if (line == "DisableTaskbar" || line == "DisableTaskbar.exe")
                toolEnabled[0] = true;
            else if (line == "RequireCapsLock" || line == "RequireCapsLock.exe")
                toolEnabled[1] = true;
            else if (line == "TimeBetweenClicks" || line == "TimeBetweenClicks.exe")
                toolEnabled[2] = true;
            else
                std::cerr << line << " is not a valid tool name in config." << std::endl;
        }
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
            ConfigFile << "DisableTaskbar" << std::endl;
            ConfigFile << "RequireCapsLock" << std::endl;
            ConfigFile << "TimeBetweenClicks" << std::endl;
            std::cout << "Data written to the file successfully." << std::endl;
        }
        else
        {
            std::cerr << "Failed to open the file for writing." << std::endl;
        }
        ConfigFile.close();

        for (int i = 0; i < 3; i++)
        {
            toolEnabled[i] = true;
        }
    }

    if (argc > 1)
    {
        int i = 1;
        int duration = 0;
        int sleep = 0;
        std::string command = "";
        std::vector<std::wstring> tools;

        while (i < argc)
        {
            if (strcmp(argv[i], "start") == 0 || strcmp(argv[i], "pause") == 0 || strcmp(argv[i], "stop") == 0 || strcmp(argv[i], "restart") == 0)
            {
                command = argv[i];
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
            }
            else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--duration") == 0)
            {
                std::istringstream ss(argv[i + 1]);
                int x;
                if (!(ss >> x))
                {
                    std::cerr << "Invalid number: " << argv[1 + 1] << '\n';
                }
                else if (!ss.eof())
                {
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
                if (!(ss >> x))
                {
                    std::cerr << "Invalid number: " << argv[1 + 1] << '\n';
                }
                else if (!ss.eof())
                {
                    std::cerr << "Trailing characters after number: " << argv[1 + 1] << '\n';
                }
                sleep = x;
                i++;
            }
            i++;
        }

        if (sleep != 0)
            Sleep(sleep * 60000);

        std::vector<std::wstring> processes;

        if (tools.empty())
        {
            int enabledToolCount = 0;
            for (int i = 0; i < 3; i++)
                if (toolEnabled[i])
                    enabledToolCount++;
            if (enabledToolCount != 3)
                std::cout << enabledToolCount << " tools are enabled. Modify config to change." << std::endl;

            for (int i = 0; i < 3; ++i)
            {
                if (toolEnabled[i])
                {
                    processes.push_back(processNames[i]);
                }
            }
        }
        else
        {
            processes = tools;
        }
        
        for (const auto& process : processes)
        {
            if (command == "start")
            {
                StartProcess(process);
            }
            else if (command == "pause" || command == "stop")
            {
                StopProcess(process);
            }
            else if (command == "restart")
            {
                StopProcess(process);
                Sleep(1000);
                StartProcess(process);
            }
        }

        if (duration != 0)
        {
            std::wstring toolsString;
            if (!tools.empty())
            {
                for (const auto& process : processes)
                {
                    toolsString += process + L" ";
                }
            }

            std::wstring ws;
            if (command == "start")
            {
                ws = L"stop " + toolsString + L"-s " + std::to_wstring(duration);
            }
            else if (command == "pause" || command == "stop")
            {
                ws = L"start " + toolsString + L"-s " + std::to_wstring(duration);
            }

            if (!ws.empty())
            {
                ShellExecute(NULL, L"open", L"ToolManager.exe", ws.c_str(), NULL, SW_HIDE);
            }
        }
    }
    else
    {
        int mode = 0;
        int count = 0;

        DWORD processes[1024], cbNeeded, processCount;

        if (!EnumProcesses(processes, sizeof(processes), &cbNeeded))
        {
            std::cerr << "Failed to enumerate processes." << std::endl;
            return false;
        }

        processCount = cbNeeded / sizeof(DWORD);

        for (unsigned int i = 0; i < processCount; ++i)
        {
            if (processes[i] != 0)
            {
                std::wstring processName = GetProcessName(processes[i]);
                if (processName == processNames[0] || processName == processNames[1] || processName == processNames[2])
                {
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

        int enabledToolCount = 0;
        for (int i = 0; i < 3; i++)
            if (toolEnabled[i])
                enabledToolCount++;
        if (enabledToolCount != 3)
            std::cout << enabledToolCount << " tools are enabled. Modify config to change." << std::endl;

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
                option = GetOption();
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
