module;

#include<vector>
#include<string>
#include<SDL2/SDL.h>
#include"Config.hpp"

import SDLApp;
import LTexture;

export module AObj;

export class AObj: public LTexture {
	int frames;
	std::vector<SDL_Rect> gSpriteClips;
	int fpp; // frame per pic
public:
	double scale;
	SDL_Texture* texture = NULL;
	AObj(int n, int s, double sc) {
		gSpriteClips.resize(n);
		frames = 0;
		fpp = s;
		scale = sc;
	}
	void setClippingRegion(int n, int x, int y, int w, int h) {
		gSpriteClips[n].x = x;
		gSpriteClips[n].y = y;
		gSpriteClips[n].w = w;
		gSpriteClips[n].h = h;
	}
	SDL_Rect* getNextRect() {
		//SDL_Rect* rect = &gSpriteClips[frames / speed];
		frames = ++frames % (gSpriteClips.size()* fpp);
		return &gSpriteClips[frames / fpp];
	}
	void setFramePerPic(int s) {
		fpp = s;
	}

};

//Renders Animate at given point
export void render(SDLApp& app, AObj& a, int x, int y) {
	//Set rendering space and render to screen
	SDL_Rect* clip = a.getNextRect();

	SDL_Rect renderQuad = { x, y, clip->w * a.scale, clip->h * a.scale };

	//Render to screen
	SDL_RenderCopy(app.renderer, a.mTexture, clip, &renderQuad);
}