#include <SDL3/SDL.h>

class LTimer
{
    public:
        //Initializes variables                                                                                                                                                                            
        LTimer():mStartTicks{ 0 },
	    mPausedTicks{ 0 },

	    mPaused{ false },
	    mStarted{ false }
	{

	}

        void start(){
	    mStarted = true;

	    mPaused = false;

	    mStartTicks = SDL_GetTicksNS();
	    mPausedTicks = 0;
	}

	void stop()
	{
	    mStarted = false;

	    mPaused = false;

	    mStartTicks = 0;
	    mPausedTicks = 0;
	}

	void pause()
	{
	    if( mStarted && !mPaused )
	    {
		mPaused = true;

		mPausedTicks = SDL_GetTicksNS() - mStartTicks;
		mStartTicks = 0;
	    }
	}

        void unpause()
	{
	    if( mStarted && mPaused )
	    {
		mPaused = false;

		mStartTicks = SDL_GetTicksNS() - mPausedTicks;

		mPausedTicks = 0;
	    }
	}

        Uint64 getTicksNS(){

	    Uint64 time = 0;

	    if( mStarted )
	    {
		if( mPaused )
		{
		    time = mPausedTicks;
		}
		else
		{
		    time = SDL_GetTicksNS() - mStartTicks;
		}
	    }

	    return time;
	}

	bool isStarted()
	{
	    return mStarted;
	}

	bool isPaused()
	{
	    return mPaused && mStarted;
	}


    private:
        Uint64 mStartTicks;

        //The ticks stored when the timer was paused                                                                                                                                                       
        Uint64 mPausedTicks;

        bool mPaused;
        bool mStarted;
};
