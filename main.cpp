#include <windows.h>
#include <iostream>

#pragma comment(lib, "User32.lib")

// lresult - type representing result of processing msg or hook notif
LRESULT CALLBACK KBHook(int code, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;
    if (code == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD vkCode = pKeyStruct->vkCode; // extract virtual key code
            std::cout << "Key pressed: " << vkCode << std::endl;
        } 
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}

int main(int argc, char* argv[]) {

    HHOOK hKeyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,         // Hook type
        KBHook,                 // Pointer to callback func
        GetModuleHandle(NULL),  // this program
        0                       // monitor all threads
    );

    MSG msg;
    // Any msg thats not WM_QUIT, GetMessage returns non-zero val
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); // takes msg and delivers to callback function
    }

    UnhookWindowsHookEx(hKeyboardHook);

    return 0;
}