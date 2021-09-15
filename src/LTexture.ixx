module;

#include<iostream>
#include<string>
#include<format>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"Config.hpp"

import SDLApp;

export module LTexture;

//Texture wrapper class
export class LTexture{

public:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;

    //Init Texture to null
    LTexture() {
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }

    //Deallocates memory
    ~LTexture() {
        free();
    }

    //Loads image at specified path
    bool loadFromFile(std::string path, SDL_Renderer* renderer) {
        free();
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            std::cerr << std::format("Unable to load image {}! SDL_image Error: {}\n", path, IMG_GetError());
        }
        else {
            //Color key image
            //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
            
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (newTexture == NULL) {
                std::cerr << std::format("Unable to create texture from {}! SDL Error: {}\n", path.c_str(), SDL_GetError());
            }
            else {
                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        //Return success
        mTexture = newTexture;

        return mTexture != NULL;
    }

    //Loads image at specified path
    bool loadFromSurface(SDL_Surface* surface, SDL_Renderer* renderer) {
        free();
        //The final texture
        SDL_Texture* newTexture = NULL;


            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(renderer, surface);
            if (newTexture == NULL) {
                std::cerr << std::format("Unable to create texture from surface! SDL Error: {}\n", SDL_GetError());
            }
            else {
                //Get image dimensions
                mWidth = surface->w;
                mHeight = surface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface(surface);
        

        //Return success
        mTexture = newTexture;

        return mTexture != NULL;
    }

    //Deallocates texture
    void free() {
        //Free texture if it exists
        if (mTexture){
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
            mWidth = 0;
            mHeight = 0;
        }
    }
    
};

//Renders texture at given point
export void render(SDLApp& app, LTexture& t, int x, int y, SDL_Rect* clip = NULL) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, t.mWidth, t.mHeight };

    //Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy(app.renderer, t.mTexture, clip, &renderQuad);
}