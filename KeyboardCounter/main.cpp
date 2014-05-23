#include <cstdio>
#include <Windows.h>

HHOOK hHook = nullptr;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
  static int count = 0;
  if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) {
    ++count;
    printf("%d\n", count);

    auto kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
    printf("vk:%x sc:%x\n", kb->vkCode, kb->scanCode);
  }
  return CallNextHookEx(hHook , code ,wParam , lParam);
}

int main() {
  hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
  MSG msg;
  while(!GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  UnhookWindowsHookEx(hHook);
  return 0;
}
