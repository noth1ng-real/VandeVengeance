#include <windows.h>
#include <iostream>

#pragma comment(lib, "Advapi32.lib")

bool getPath(wchar_t* exePath, DWORD bufferSize);
bool WriteRegistryRun(wchar_t* path);