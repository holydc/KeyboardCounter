#include <cstdio>
#include <Windows.h>

class KeyboardCounter {
 public:
  static KeyboardCounter& getInstance() {
    return (*m_singleton);
  }

  void go() {
    m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
    MSG msg;
    while(!GetMessage(&msg, nullptr, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(m_hook);
  }

 private:
  static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    KeyboardCounter &app = KeyboardCounter::getInstance();
    if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) {
      ++app.m_counter[0]; // since the virtual key code must be a value in the range 1 to 254
      auto kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
      ++app.m_counter[kb->vkCode];
      printf("press:0x%x count:%d total:%d\n", kb->vkCode, app.m_counter[kb->vkCode], app.m_counter[0]);
    }
    return CallNextHookEx(app.m_hook, code ,wParam , lParam);
  }

  static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  }

  static KeyboardCounter *m_singleton;

  KeyboardCounter() : m_hook(nullptr) {
    memset(m_counter, 0, sizeof(m_counter));
  }

  HHOOK m_hook;
  int m_counter[256];
}; // class KeyboardCounter

KeyboardCounter *KeyboardCounter::m_singleton = new KeyboardCounter();

int main() {
  KeyboardCounter::getInstance().go();
  return 0;
}
