#include <windows.h>
#include <iostream>

bool getPath(wchar_t* exePath, DWORD bufferSize);
bool WriteRegistryRun(wchar_t* path);