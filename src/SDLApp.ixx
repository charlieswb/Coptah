module;

#include<iostream>
#include<string>
#include<windows.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include"Config.hpp"
#include"../resource.h"

export module SDLApp;

export class SDLApp {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* gFont;

    SDLApp() {
        window = nullptr;
        renderer = nullptr;
        gFont = nullptr;
    }

    ~SDLApp() {
        //Free global font
        if (gFont) {
            TTF_CloseFont(gFont);
            gFont = nullptr;
        }
        
        //Destroy renderer
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        //Destroy window
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        //Quit SDL subsystems
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    bool initSDL(std::string name) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
            return false;
        }

        window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
            return false;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL)
        {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)){
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            return false;
        }

        if (TTF_Init() == -1){
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
            return false;
        }

        HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCEW(IDR_TTF1), TEXT("TTF"));
        if (!hResource) {
            std::cerr << "Error Loading Resource TTF" << '\n';
            return false;
        }
        HGLOBAL hData = LoadResource(nullptr, hResource);
        DWORD dataSize = SizeofResource(nullptr, hResource);

        auto data = LockResource(hData);

        auto sdldata = SDL_RWFromConstMem(data, dataSize);

        gFont = TTF_OpenFontRW(sdldata, 1, 32);
        //gFont = TTF_OpenFont(FONT_PATH.c_str(), 28);
        if (gFont == NULL){
            std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << '\n';
            return false;
        }

        return true;
    }

    void prepareScene() {
        SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);

        //Clear screen
        SDL_RenderClear(renderer);
    }

    void presentScene() {
        // Update screen
        SDL_RenderPresent(renderer);
    }

};

export bool doInput(){
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        //User requests quit
        case SDL_QUIT:
            return false;
        default:
            break;
        }
    }

    return true;
}



