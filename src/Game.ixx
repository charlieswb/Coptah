module;

#include<iostream>
#include<deque>
#include<format>
#include<SDL2/SDL.h>
#include"Config.hpp"
#include"../resource.h"

import SDLApp;
import LTexture;
import AObj;
import LTimer;
import Random;

export module Game;

bool checkBlockCollision(const SDL_Rect&, const SDL_Rect&);

export class Game {
    static int instance;
    int id;
    int score;
    bool isGameOver;
    bool isPaused;
    SDLApp* app;
    AObj copter;
    SDL_Rect copterPos;
    std::deque<SDL_Rect> blocks;
    LTimer capTimer;
    Rand_Int rnd;

    int xVel;
    double yVel;
    double yAcc;
    
public:
	Game(SDLApp* a) : copter(AObj{ 4, 3, COPTER_SCALE }), rnd(Rand_Int{ 0, SCREEN_HEIGHT - BLOCK_HEIGHT - 1 }){
		id = ++instance;
        app = a;

        setupAsset();

        copterPos.x = static_cast<int>(SCREEN_WIDTH / 10);
        copterPos.y = static_cast<int>(SCREEN_HEIGHT / 3);
        copterPos.w = static_cast<int>(COPTER_WIDTH_WO_BLADES * COPTER_SCALE);
        copterPos.h = static_cast<int>(COPTER_HEIGHT * COPTER_SCALE);
	}
    void setupAsset() {
        copter.loadFromResource(IDB_PNG1, "PNG", app->renderer);
        copter.setClippingRegion(0, 3, 10, COPTER_WIDTH, COPTER_HEIGHT);
        copter.setClippingRegion(1, 3, 160, COPTER_WIDTH, COPTER_HEIGHT);
        copter.setClippingRegion(2, 3, 310, COPTER_WIDTH, COPTER_HEIGHT);
        copter.setClippingRegion(3, 3, 460, COPTER_WIDTH, COPTER_HEIGHT);
    }
    void setupParams() {
        isGameOver = false;
        isPaused = false;
        score = 0;
        
        copterPos.y = SCREEN_HEIGHT / 3;
        xVel = INIT_GAME_SPEED;
        yVel = 0;
        yAcc = -INIT_COPTER_SPEED_SHIFT;
        
        blocks.clear();
        blocks.push_back({ SCREEN_WIDTH, static_cast<int>(SCREEN_HEIGHT / 3), BLOCK_WIDTH, BLOCK_HEIGHT });

        SDL_SetWindowTitle(app->window, std::format("Coptah Score: {}", score).c_str());
    }


    bool tick() {
        capTimer.start();

        readInput();
        if (isGameOver) return false;

        if (!isPaused) {
            updateParams();

            isGameOver = checkCollision();

            draw();
        }

        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
        {
            //Wait remaining time
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
        return !isGameOver;
    }

    void readInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
                //User requests quit
            case SDL_QUIT:
                std::cout << "Quit!\n";
                isGameOver = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_p) {
                    if (isPaused) {
                        isPaused = false;
                        yAcc = -INIT_COPTER_SPEED_SHIFT;
                        std::cout << "Unpaused\n";
                    }
                    else {
                        isPaused = true;
                        std::cout << "Paused\n";
                    }
                }
                else if (event.key.keysym.sym == SDLK_q) {
                    std::cout << "Quit!\n";
                    isGameOver = true;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                copter.setFramePerPic(1);
                yAcc = INIT_COPTER_SPEED_SHIFT;
                break;
            case SDL_MOUSEBUTTONUP:
                copter.setFramePerPic(3);
                yAcc = -INIT_COPTER_SPEED_SHIFT;
                break;
            default:
                break;
            }
        }
    }

    void updateParams() {
        //update copter pos
        yVel += yAcc;

        if (yVel > INIT_MAX_COPTER_SPEED) yVel = INIT_MAX_COPTER_SPEED;
        else if (yVel < -INIT_MAX_COPTER_SPEED) yVel = -INIT_MAX_COPTER_SPEED;

        copterPos.y -= yVel;

        //update blocks pos
        xVel = INIT_GAME_SPEED + (score / 10);
        /*
        if (xVel != INIT_GAME_SPEED + (score / 10)) {
            xVel = INIT_GAME_SPEED + (score / 10);
           std::cout << std::format("Speed up to: {}\n", xVel);
        }
        */


        while (!blocks.empty() && blocks.front().x - xVel < -blocks.front().w) {
            blocks.pop_front();
            ++score;
            SDL_SetWindowTitle(app->window, std::format("Coptah Score: {}", score).c_str());
        }

        for (SDL_Rect& r : blocks) {
            r.x -= xVel;
        }

        for (int i = blocks.size(); i < NUMBER_OF_BLOCK; ++i) {
            //add first block
            blocks.push_back({ static_cast<int>(blocks.back().x + SCREEN_WIDTH * 0.8), rnd(), BLOCK_WIDTH, BLOCK_HEIGHT });

            //chance to spawn second block
            if(rnd()%20 < xVel )
                blocks.push_back({ blocks.back().x, rnd(), BLOCK_WIDTH, BLOCK_HEIGHT });

        }
    }

    bool checkCollision() {
        if (copterPos.y < 0 || copterPos.y + copterPos.h >= SCREEN_HEIGHT) return true;

        for (SDL_Rect& r : blocks) {
            if (checkBlockCollision(copterPos, r)) return true;
        }

        return false;
    }

    bool mainMenuScreen() {
        app->prepareScene();
        render(*app, copter, copterPos.x, copterPos.y);

        SDL_Color textColor = { 0, 0, 0 };
        //SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        //Start Button
        SDL_Rect startButton;
        {
            LTexture startTextTexture;
            startTextTexture.loadFromRenderedText("Start", textColor, app->renderer, app->gFont);
            startButton = { SCREEN_WIDTH / 2, SCREEN_HEIGHT/4 - startTextTexture.mHeight/2, startTextTexture.mWidth, startTextTexture.mHeight };
            //SDL_RenderFillRect(app->renderer, &startButton);
            render(*app, startTextTexture, startButton.x, startButton.y, NULL);
        }
        //Quit Button
        SDL_Rect quitButton;
        {
            LTexture quitTextTexture;
            quitTextTexture.loadFromRenderedText("Quit", textColor, app->renderer, app->gFont);
            quitButton = { SCREEN_WIDTH / 2, (SCREEN_HEIGHT - quitTextTexture.mHeight) / 2, quitTextTexture.mWidth, quitTextTexture.mHeight };
            //SDL_RenderFillRect(app->renderer, &quitButton);
            render(*app, quitTextTexture, quitButton.x, quitButton.y, NULL);
        }
        app->presentScene();

        //std::cout << std::format("startPos <{}, {}>, quitPos <{}, {}>, ", startButton.x, startButton.y, quitButton.x, quitButton.y);

        while (1) {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type) {
                    //User requests quit
                case SDL_QUIT:
                    return false;

                case SDL_MOUSEBUTTONDOWN: {
                    //Get mouse position
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Rect mPos = { x - 10, y - 10, 25, 25 };
                    //std::cout << std::format("mousePos: <{}, {}>\n", mPos.x, mPos.y);
                    if (checkBlockCollision(mPos, startButton)) {
                        return true;
                    }
                    else if (checkBlockCollision(mPos, quitButton)) {
                        return false;
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    bool resultScreen() {
        app->prepareScene();
        render(*app, copter, copterPos.x, copterPos.y);

        SDL_Color textColor = { 0, 0, 0 };
        //SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        //Score Label
        LTexture scoreTextTexture;
        scoreTextTexture.loadFromRenderedText(std::format("Score: {}, Speed: {}", score, xVel), textColor, app->renderer, app->gFont);
        render(*app, scoreTextTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT/4 - scoreTextTexture.mHeight/2, NULL);
        //Retry Button
        SDL_Rect retryButton;
        {
            LTexture retryTextTexture;
            retryTextTexture.loadFromRenderedText("Retry", textColor, app->renderer, app->gFont);
            retryButton = { SCREEN_WIDTH / 2, (SCREEN_HEIGHT - retryTextTexture.mHeight) / 2, retryTextTexture.mWidth, retryTextTexture.mHeight };
            //SDL_RenderFillRect(app->renderer, &retryButton);
            render(*app, retryTextTexture, retryButton.x, retryButton.y, NULL);
        }
        //Quit Button
        SDL_Rect quitButton;
        {
            LTexture quitTextTexture;
            quitTextTexture.loadFromRenderedText("Quit", textColor, app->renderer, app->gFont);
            quitButton = { SCREEN_WIDTH / 2, (SCREEN_HEIGHT*3/4 - quitTextTexture.mHeight/2), quitTextTexture.mWidth, quitTextTexture.mHeight };
            //SDL_RenderFillRect(app->renderer, &quitButton);
            render(*app, quitTextTexture, quitButton.x, quitButton.y, NULL);
        }
        app->presentScene();

        //std::cout << std::format("retryPos <{}, {}>, quitPos <{}, {}>, ", retryButton.x, retryButton.y, quitButton.x, quitButton.y);

        while (1) {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type) {
                    //User requests quit
                case SDL_QUIT:
                    return false;

                case SDL_MOUSEBUTTONDOWN: {
                    //Get mouse position
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Rect mPos = { x - 10, y - 10, 25, 25 };
                    //std::cout << std::format("mousePos: <{}, {}>\n", x, y);
                    if (checkBlockCollision(mPos, retryButton)) {
                        //yAcc = INIT_COPTER_SPEED_SHIFT;
                        //isPaused = false;
                        return true;
                    }
                    else if (checkBlockCollision(mPos, quitButton)) {
                        return false;
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    void draw() {
        app->prepareScene();

        //draw copter
        render(*app, copter, copterPos.x, copterPos.y);

        //draw blocks
        SDL_SetRenderDrawColor(app->renderer, 0xFF, 0x00, 0x00, 0xFF);
        for (SDL_Rect r : blocks) {
            SDL_RenderFillRect(app->renderer, &r);
        }

        app->presentScene();

    }

    int getScore() {
        return score;
    }

    void printState() {
        std::cout << std::format("ID:{}, Score:{}, Speed: {}, Game Over: {}, Paused: {}\n", id, score, xVel, isGameOver, isPaused);
    }
};

bool checkBlockCollision(const SDL_Rect& a, const SDL_Rect& b) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    /*
        xPos, yPos                          xPos + width x scale, yPos

        xPos, yPos + height x scale         xPos + width x scale, yPos + height x scale
    */

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}