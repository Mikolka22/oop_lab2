#include "AppManager.h"
#include "System.h"
#include "Line.h"
#include <iostream>
#include <list>

int AppManager::number(std::string text) {
    int value = 0;
    if (text.empty()) return 0;
    for (unsigned int i = 0; i < text.size(); ++i) {
        if (text[i] < '0' || text[i] > '9') return 0;
        value = value * 10 + text[i] - '0';
        if (value > 30) return 0;
    }
    return value;
}

int AppManager::readNumber(const char* prompt) {
    std::string text;
    int value;
    do {
        std::cout << prompt;
        if (!std::getline(std::cin, text)) return 0;
        value = number(text);
        if (value == 0) std::cout << "Введите целое число от 1 до 30.\n";
    } while (value == 0);
    return value;
}

void AppManager::makeTimes(System& console, int times[]) {
    for (int i = 0; i < frequency; ++i) {
        times[i] = console.random(0, 1000);
    }
    // сначала должны появиться более ранние линии.
    for (int i = 1; i < frequency; ++i) {
        int time = times[i];
        int j = i - 1;
        while (j >= 0 && times[j] > time) {
            times[j + 1] = times[j];
            --j;
        }
        times[j + 1] = time;
    }
}

int AppManager::run(int argc, char* argv[]) {
    System console;
    std::string mode;
    if (argc == 2) {
        std::string help = argv[1];
        if (help == "--help" || help == "/?") {
            std::cout << "matrix.exe [частота скорость длина Y|N]\n"
                      << "Частота: 1..30 линий/с. Скорость: 1..30 символов/с. Длина: 1..30.\n"
                      << "Без параметров — диалог. Пример: matrix.exe 3 23 8 Y\n"
                      << "Y: цвет всей линии случайный, N: зелёный. Выход: Esc или Ctrl+C.\n";
            return 0;
        }
    }

    if (argc == 1) {
        frequency = readNumber("Частота появления линий (1..30): ");
        if (frequency == 0) return 1;
        speed = readNumber("Скорость (1..30): ");
        if (speed == 0) return 1;
        length = readNumber("Длина (1..30): ");
        if (length == 0) return 1;
        do {
            std::cout << "Режим (Y/N): ";
            if (!std::getline(std::cin, mode)) return 1;
            if (mode != "Y" && mode != "y" && mode != "N" && mode != "n") {
                std::cout << "Введите Y или N.\n";
            }
        } while (mode != "Y" && mode != "y" && mode != "N" && mode != "n");
    } else if (argc == 5) {
        frequency = number(argv[1]);
        speed = number(argv[2]);
        length = number(argv[3]);
        mode = argv[4];
    } else {
        std::cout << "Нужны четыре параметра. Справка: matrix.exe --help\n";
        return 1;
    }

    if (frequency == 0 || speed == 0 || length == 0 ||
        (mode != "Y" && mode != "y" && mode != "N" && mode != "n")) {
        std::cout << "Неверные параметры. Справка: matrix.exe --help\n";
        return 1;
    }
    colorful = (mode == "Y" || mode == "y");
    if (!console.start()) {
        std::cout << "Нужна консоль Windows шириной хотя бы 3 символа.\n";
        return 1;
    }

    std::list<Line> lines;
    int times[30];
    makeTimes(console, times);
    int next = 0;
    unsigned long long second = console.time();

    while (!console.stop()) {
        unsigned long long now = console.time();
        while (true) {
            while (next < frequency && now >= second + times[next]) {
                int x = console.random(1, console.width - 2);
                int color = 10;
                if (colorful) color = console.random(1, 15);
                lines.emplace_back(x, length, color, speed, second + times[next]);
                ++next;
            }
            if (now < second + 1000) break;
            second += 1000;
            makeTimes(console, times);
            next = 0;
        }

        for (auto line = lines.begin(); line != lines.end();) {
            if (!line->step(console, now)) {
                line = lines.erase(line);
            } else {
                ++line;
            }
        }

        console.clearFrame();
        for (Line& line : lines) {
            line.show(console);
        }
        console.present();
        console.wait(1);
    }
    return 0;
}
