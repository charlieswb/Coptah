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
    g.setupAsset();
    string retry = "y";
    while (retry == "Y" || retry == "y") {
        g.setupParams();
        while (g.tick());
        cout << "Game Over\n";
        g.printState();
        cout << "Retry?: (Y/N)";
        cin >> retry;
    }
	return 0;
}

