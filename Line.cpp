#include "Line.h"
#include "Symbol.h"

Line::Line(int column, int size, int textColor, int lineSpeed, unsigned long long startTime) {
    x = column;
    length = size;
    color = textColor;
    number = 0;
    speed = lineSpeed;
    start = startTime;
}

bool Line::step(System& console, unsigned long long now) {
    int dy[7] = {0, 1, 1, 2, 3, 3, 4};
    // Время каждого шага считаем от рождения линии, чтобы не копить задержку.
    while (now >= start + number * 1000ULL / speed) {
        if (number >= length) {
            int old = number - length;
            int y = old / 7 * 5 + dy[old % 7];
            if (y >= console.height) return false;
        }
        ++number;
    }
    return true;
}

void Line::show(System& console) {
    // Семь букв ромба: A / B C / D / E F / G.
    int dx[7] = {0, -1, 1, 0, -1, 1, 0};
    int dy[7] = {0, 1, 1, 2, 3, 3, 4};
    int first = number - length;
    if (first < 0) first = 0;

    for (int n = first; n < number; ++n) {
        int y = n / 7 * 5 + dy[n % 7];
        if (y < console.height) {
            char letter = 'A' + n % 26;
            Symbol symbol(x + dx[n % 7], y, letter, color);
            symbol.show(console);
        }
    }
}
