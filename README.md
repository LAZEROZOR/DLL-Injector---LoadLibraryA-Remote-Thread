# DLL Injector (LoadLibraryA)

A small Windows utility that demonstrates classic DLL injection.  
It allocates memory in a target process, writes a DLL path, and executes `LoadLibraryA` via `CreateRemoteThread`.

**Disclaimer**: This project is for **educational/testing purposes only**. Injecting code into third-party software without permission may violate terms of service or law. Use responsibly.

## Features
- Find a process PID by its executable name.
- Allocate and write the DLL path into the target process.
- Invoke `LoadLibraryA` remotely with `CreateRemoteThread`.
- Basic error reporting at each step.

## Requirements
- Windows 10/11 (x64 or x86 build matching the target process).
- Administrator privileges.
- Visual Studio or any C++ compiler with WinAPI support.

## Usage
1. Edit `dll_path` and `targetProcess` in `main.cpp`.
2. Build the injector in the same architecture as the target process.
3. Run as Administrator.

