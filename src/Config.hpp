#pragma once

#include<string>

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 576;
constexpr int SCREEN_FPS = 60;
constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const std::string FONT_PATH = R"(C:\Users\Alpha\source\repos\Coptah\resources\lazy.ttf)";

const std::string COPTER_PNG = R"(C:\Users\Alpha\source\repos\Coptah\resources\helicopter-spritesheet.png)";
constexpr int COPTER_WIDTH = 416;
constexpr int COPTER_WIDTH_WO_BLADES = 343;
constexpr int COPTER_HEIGHT = 131;
constexpr double COPTER_SCALE = 0.4;

constexpr int BLOCK_WIDTH = 50;
constexpr int BLOCK_HEIGHT = 100;
constexpr int NUMBER_OF_BLOCK = 2;

constexpr double INIT_GAME_SPEED = 6.0;
constexpr double INIT_COPTER_SPEED_SHIFT = 0.4;
constexpr double INIT_MAX_COPTER_SPEED = 8.0;
