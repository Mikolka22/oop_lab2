#pragma once
#include "System.h"

class Line {
    int x, length, color, number, speed;
    unsigned long long start;

public:
    Line(int column, int size, int textColor, int lineSpeed, unsigned long long startTime);
    bool step(System& console, unsigned long long now);
    void show(System& console);
};
