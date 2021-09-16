#define SDL_MAIN_HANDLED

#include<iostream>

import SDLApp;
import Game;

int Game::instance = 0;

int main(int argc, char* argv[]) {
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
        //cout << "Game Over\n";
        //g.printState();
        retry = g.resultScreen();
    }
	return 0;
}