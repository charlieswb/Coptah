module;

#include<iostream>
#include<string>
#include<format>
#include<windows.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

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

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            std::cerr << std::format("Unable to load image {}! SDL_image Error: {}\n", path, IMG_GetError());
        }
        else {
            //Color key image
            //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
            loadFromSurface(loadedSurface, renderer);
        }

        return mTexture != NULL;
    }

    //Load texture from text
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* gFont)
    {
        free();

        //Render text surface
        SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
        if (textSurface == NULL) {
            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
        }
        else {
            loadFromSurface(textSurface, renderer);
        }

        //Return success
        return mTexture != NULL;
    }

    //Load texture from resource
    bool loadFromResource(int resourceName, std::string resourceType, SDL_Renderer* renderer) {
        free();
        std::wstring ws = std::wstring(resourceType.begin(), resourceType.end());
        HRSRC hResource = FindResourceW(nullptr, MAKEINTRESOURCEW(resourceName), ws.c_str());
        HGLOBAL hData = LoadResource(nullptr, hResource);
        DWORD dataSize = SizeofResource(nullptr, hResource);

        auto data = LockResource(hData);

        auto sdlimg = SDL_RWFromConstMem(data, dataSize);


        SDL_Surface* loadedSurface = IMG_Load_RW(sdlimg, 1);

        if (loadedSurface == NULL) {
            std::cerr << std::format("Unable to load image! SDL_image Error: {}\n", IMG_GetError());
        }
        else {
            //Color key image
            //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
            loadFromSurface(loadedSurface, renderer);
        }

        return mTexture != NULL;
    }

    //Loads texture from surface
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