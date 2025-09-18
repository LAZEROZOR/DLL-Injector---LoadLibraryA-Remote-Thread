#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

DWORD GetPID(const char* processName);
