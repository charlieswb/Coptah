module;

#include<SDL2/SDL.h>

export module LTimer;

//The application time based timer
export class LTimer {
public:
    //Initializes variables
    LTimer() {
        mStartTicks = 0;
        mPausedTicks = 0;

        mPaused = false;
        mStarted = false;
    }

    //Start the timer
    void start() {
        mStarted = true;

        //Unpause the timer
        mPaused = false;

        //Get the current clock time
        mStartTicks = SDL_GetTicks();
        mPausedTicks = 0;
    }

    //Stop the timer
    void stop() {
        mStarted = false;

        //Unpause the timer
        mPaused = false;

        //Clear tick variables
        mStartTicks = 0;
        mPausedTicks = 0;
    }

    //Pause the timer
    void pause() {
        //If the timer is running and isn't already paused
        if (mStarted && !mPaused)
        {
            //Pause the timer
            mPaused = true;

            //Calculate the paused ticks
            mPausedTicks = SDL_GetTicks() - mStartTicks;
            mStartTicks = 0;
        }
    }

    //Unpause the timer
    void unpause() {
        //If the timer is running and paused
        if (mStarted && mPaused)
        {
            //Unpause the timer
            mPaused = false;

            //Reset the starting ticks
            mStartTicks = SDL_GetTicks() - mPausedTicks;

            //Reset the paused ticks
            mPausedTicks = 0;
        }
    }

    //Gets the timer's time
    Uint32 getTicks() {
        //The actual timer time
        Uint32 time = 0;

        //If the timer is running
        if (mStarted)
        {
            //If the timer is paused
            if (mPaused)
            {
                //Return the number of ticks when the timer was paused
                time = mPausedTicks;
            }
            else
            {
                //Return the current time minus the start time
                time = SDL_GetTicks() - mStartTicks;
            }
        }

        return time;
    }

    //Timer is running and paused or unpaused
    bool isStarted() {
        return mStarted;
    }

    //Timer is running and paused
    bool isPaused() {
        return mPaused && mStarted;
    }

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};