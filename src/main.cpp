#define SDL_MAIN_HANDLED

#include<ios>
#include<iostream>
#include<format>
#include<string>
#include<vector>
#include<deque>
#include<list>
#include<stdexcept>
#include<cassert>
#include<chrono>
#include<algorithm>
#include<filesystem>
#include<fstream>
#include<utility>
#include<functional>
#include<thread>
#include<future>
#include<coroutine>
#include<cmath>
#include<SDL2/SDL.h>
#include<windows.h>
#include"../resource.h"
#include"Config.hpp"

import SDLApp;
import Game;

int Game::instance = 0;

int main(int argc, char* args[]) {
	using namespace std;
    SDLApp a;
    if (!a.initSDL("Coptah")) {
        exit(1);
    }

    Game g{ &a };
    bool retry = g.mainMenuScreen();
    while (retry) {
        g.setupParams();
        while (g.tick());
        cout << "Game Over\n";
        g.printState();
        retry = g.resultScreen();
    }
	return 0;
}

