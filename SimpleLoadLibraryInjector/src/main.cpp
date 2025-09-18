//***************************************************************//
//                                                               //
//  Project : Dll injector                                       //
//  Description : Inject a dll into a process using LoadLibraryA //
//                and CreateRemoteThread                        //
//                                                               //
//  Author      : LAZEROZOR                                      //
//  License     : Open Source - Educational use only             //
//  Date        : 2025                                           //
//                                                               //
//  Warning     : For educational and testing purposes only.     //
//                Use responsibly and ethically.                 //
//                                                               //
//***************************************************************//

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include "../memory/memory.h"

int main()
{
    const char* dll_path = "C:\\full\\path\\to\\your\\dll.dll";
    const char* targetProcess = "ArkAscended.exe";

    DWORD pid = GetPID(targetProcess);
    if (pid == 0)
    {
        std::cout << "[-] Process not found \n";
        std::cin.get();
        return 1;
    }
    std::cout << "[+] Process found PID : " << pid << "\n";

    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!proc)
    {
        std::cout << "[-] OpenProcess failed. Error: " << GetLastError() << "\n";
        std::cin.get();
        return 1;
    }
    std::cout << "[+] Handle to the target process opened \n";

    SIZE_T dllSize = strlen(dll_path) + 1;
    LPVOID lpBaseAddress = VirtualAllocEx(proc, nullptr, dllSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!lpBaseAddress)
    {
        std::cout << "[-] VirtualAllocEx failed \n";
        CloseHandle(proc);
        std::cin.get();
        return 1;
    }

    if (!WriteProcessMemory(proc, lpBaseAddress, dll_path, dllSize, nullptr))
    {
        std::cout << "[-] WriteProcessMemory failed \n";
        VirtualFreeEx(proc, lpBaseAddress, 0, MEM_RELEASE);
        CloseHandle(proc);
        std::cin.get();
        return 1;
    }

    HMODULE kernel32base = GetModuleHandleA("kernel32.dll");
    if (!kernel32base)
    {
        std::cout << "[-] GetModuleHandleA failed \n";
        VirtualFreeEx(proc, lpBaseAddress, 0, MEM_RELEASE);
        CloseHandle(proc);
        std::cin.get();
        return 1;
    }

    FARPROC loadLibraryAddr = GetProcAddress(kernel32base, "LoadLibraryA");
    if (!loadLibraryAddr)
    {
        std::cout << "[-] GetProcAddress for LoadLibraryA failed \n";
        VirtualFreeEx(proc, lpBaseAddress, 0, MEM_RELEASE);
        CloseHandle(proc);
        std::cin.get();
        return 1;
    }

    HANDLE thread = CreateRemoteThread(proc, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, lpBaseAddress, 0, nullptr);
    if (!thread)
    {
        std::cout << "[-] CreateRemoteThread failed \n";
        VirtualFreeEx(proc, lpBaseAddress, 0, MEM_RELEASE);
        CloseHandle(proc);
        std::cin.get();
        return 1;
    }

    WaitForSingleObject(thread, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeThread(thread, &exitCode);
    if (exitCode == 0)
        std::cout << "[-] LoadLibraryA failed \n";
    else
        std::cout << "[+] DLL loaded\n";

    VirtualFreeEx(proc, lpBaseAddress, 0, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(proc);

    std::cin.get();
    return 0;
}
