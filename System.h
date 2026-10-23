#pragma once
#include <vector>

class System {
    int left, top;
    std::vector<char> letters, oldLetters;
    std::vector<int> colors, oldColors;

public:
    int width, height;

    System();
    bool start();
    void draw(int x, int y, char letter, int color);
    void clearFrame();
    void present();
    unsigned long long time();
    void wait(int milliseconds);
    bool stop();
    int random(int from, int to);
};
