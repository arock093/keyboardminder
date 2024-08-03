#include <windows.h>
#include <shlobj.h>
#include <comdef.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <conio.h> 

bool CreateShortcut(const std::string& exeName, const std::wstring& startupPath) {
    
    std::filesystem::path cwd = std::filesystem::current_path();
    std::wstring wCwd = cwd.wstring();
    std::wstring wExeName(exeName.begin(), exeName.end());
    std::wstring targetPath = wCwd + L"\\" + wExeName + L".exe";
    std::wstring shortcutPath = std::wstring(startupPath) + L"\\" + wExeName + L".lnk";
    std::wstring description = L"Your Application Description";

    if (std::filesystem::exists(shortcutPath))
    {
        std::cout << exeName + " already added to startup. ";
        return false;
    }
    else
    {

        CoInitialize(NULL);

        IShellLink* pShellLink = NULL;
        HRESULT hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
        if (SUCCEEDED(hRes)) {
            pShellLink->SetPath(targetPath.c_str());
            pShellLink->SetDescription(description.c_str());

            IPersistFile* pPersistFile = NULL;
            hRes = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
            if (SUCCEEDED(hRes)) {
                hRes = pPersistFile->Save(shortcutPath.c_str(), TRUE);
                pPersistFile->Release();
            }
            pShellLink->Release();
        }

        CoUninitialize();
        return SUCCEEDED(hRes);
    }
}
 
int main() {

    std::string tools[4] = { "DisableTaskbar-x64", "RequireCapsLock", "TimeBetweenClicks" };

    wchar_t startupPath[MAX_PATH];
    HRESULT hRes = SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, startupPath);
    if (FAILED(hRes)) {
        std::cout << "Failed to get startup folder path";
        return 1;
    }
    
    int option;
    std::string sOption = "";
    
    std::cout << "Type 1 or 2 to select an option\n1) Add tool(s) to startup\n2) Remove tool(s) from startup\n";
    std::cin >> option;
    while (option != 1 && option != 2)
    {
        std::cout << "That is not an option. Please try again.\n";
        std::cin >> option;
    }
    if (option == 1)
    {
        option = 0;
        
        std::cout << "1) Add all tools to startup\n2) Add some tools to startup\n";
        std::cin >> option;
        while (option != 1 && option != 2)
        {
            std::cout << "That is not an option. Please try again.\n";
            std::cin >> option;
        }
        if (option == 1)
        {
            for (int i = 0; i < 3; i++) {
                if (CreateShortcut(tools[i], startupPath)) {
                    std::cout << "Shortcut created successfully\n";
                }
                else {
                    std::cout << "Failed to create shortcut\n";
                }
            }
        }
        else if (option == 2)
        {
            for (int i = 0; i < 3; i++) {
                sOption = "";
                std::cout << "Add " + tools[i] + " to startup\nY/N\n";
                std::cin >> sOption;
                while (sOption != "Y" && sOption != "y" && sOption != "N" && sOption != "n")
                {
                    std::cout << "That is not an option. Please try again.\n";
                    std::cin >> sOption;
                }
                if (sOption == "Y" || sOption == "y")
                {
                    if (CreateShortcut(tools[i], startupPath)) {
                        std::cout << "Shortcut created successfully\n";
                    }
                    else {
                        std::cout << "Failed to create shortcut\n";
                    }
                }
                else if (sOption == "N" || sOption == "n")
                {
                    continue;
                }
            }
        }
    }
    else if (option == 2)
    {
        option = 0;
        std::cout << "1) Remove all tools from startup\n2) Remove some tools from startup\n";
        std::cin >> option;
        while (option != 1 && option != 2)
        {
            std::cout << "That is not an option. Please try again.\n";
            std::cin >> option;
        }
        if (option == 1)
        {
            for (int i = 0; i < 3; i++) {
                
                std::wstring wExeName(tools[i].begin(), tools[i].end());
                std::wstring wShortcutPath = std::wstring(startupPath) + L"\\" + wExeName + L".lnk";
                if (std::filesystem::exists(wShortcutPath))
                {
                    if (_wremove(wShortcutPath.c_str()) == 0)
                        std::cout << "Succesfully removed " + tools[i] + " from startup.\n";
                    else
                        std::cout << "Unable to remove " + tools[i] + " from startup.\n";
                }
                else
                    std::cout << "Failed to remove " + tools[i] + ". Tool not present in startup.\n";
            }
        }
        else if (option == 2)
        {
            for (int i = 0; i < 3; i++) {
                sOption = "";
                std::cout << "Remove " + tools[i] + " from startup?\nY/N\n";
                std::cin >> sOption;
                while (sOption != "Y" && sOption != "y" && sOption != "N" && sOption != "n")
                {
                    std::cout << "That is not an option. Please try again.\n";
                    std::cin >> sOption;
                }
                if (sOption == "Y" || sOption == "y")
                {
                    std::wstring wExeName(tools[i].begin(), tools[i].end());
                    std::wstring wShortcutPath = std::wstring(startupPath) + L"\\" + wExeName + L".lnk";
                    if (std::filesystem::exists(wShortcutPath))
                    {
                        if (_wremove(wShortcutPath.c_str()) == 0)
                            std::cout << "Succesfully removed " + tools[i] + " from startup.\n";
                        else
                            std::cout << "Unable to remove " + tools[i] + " from startup.\n";
                    }
                    else
                        std::cout << "Failed to remove " + tools[i] + ". Tool not present in startup.\n";
                }
                else if (sOption == "N" || sOption == "n")
                {
                    continue;
                }
            }
        }
    }
    std::cout << "Press ENTER to Exit.";
    _getch();
    return 0;
}
