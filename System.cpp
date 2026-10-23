#include "System.h"
#include <windows.h>
#include <conio.h>
#include <cstdlib>

System::System() {
    left = top = width = height = 0;
    SetConsoleOutputCP(CP_UTF8);
    std::srand(GetTickCount());
}

bool System::start() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(console, &info)) return false;

    left = info.srWindow.Left;
    top = info.srWindow.Top;
    width = info.srWindow.Right - left + 1;
    height = info.srWindow.Bottom - top + 1;
    if (width < 3) return false;

    DWORD written;
    DWORD size = info.dwSize.X * info.dwSize.Y;
    COORD pos = {0, 0};
    FillConsoleOutputCharacterA(console, ' ', size, pos, &written);
    FillConsoleOutputAttribute(console, info.wAttributes, size, pos, &written);
    letters.assign(width * height, ' ');
    oldLetters.assign(width * height, ' ');
    colors.assign(width * height, 0);
    oldColors.assign(width * height, info.wAttributes);
    return true;
}

void System::draw(int x, int y, char letter, int color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int i = y * width + x;
    letters[i] = letter;
    colors[i] = color;
}

void System::clearFrame() {
    for (int i = 0; i < width * height; ++i) {
        letters[i] = ' ';
        colors[i] = 0;
    }
}

void System::present() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < width * height; ++i) {
        if (letters[i] == oldLetters[i] && colors[i] == oldColors[i]) continue;
        COORD pos;
        pos.X = left + i % width;
        pos.Y = top + i / width;
        WORD attribute = colors[i];
        DWORD written;
        WriteConsoleOutputCharacterA(console, &letters[i], 1, pos, &written);
        WriteConsoleOutputAttribute(console, &attribute, 1, pos, &written);
        oldLetters[i] = letters[i];
        oldColors[i] = colors[i];
    }
}

unsigned long long System::time() {
    return GetTickCount64();
}

void System::wait(int milliseconds) {
    Sleep(milliseconds);
}

bool System::stop() {
    if (_kbhit()) {
        if (_getch() == 27) return true;
    }
    return false;
}

int System::random(int from, int to) {
    return from + std::rand() % (to - from + 1);
}
