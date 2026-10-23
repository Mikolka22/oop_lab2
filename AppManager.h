#pragma once
#include <string>

class System;

class AppManager {
    int frequency, speed, length;
    bool colorful;

    int number(std::string text);
    int readNumber(const char* prompt);
    void makeTimes(System& console, int times[]);

public:
    int run(int argc, char* argv[]);
};
