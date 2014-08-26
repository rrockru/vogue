#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;
const int SCREEN_BPP = 32;

int main( int argc, char *args[] )
{
	SDL_Surface *clock = NULL;
	SDL_Surface *date = NULL;
	SDL_Surface *screen = NULL;

	SDL_Event event;
	TTF_Font *fontDate = NULL;
	TTF_Font *fontClock = NULL;
	SDL_Color textColor = { 0, 255, 0 };

	if (SDL_Init( SDL_INIT_VIDEO ) == -1)
	{
		return false;
	}	

	screen=SDL_SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if( screen == NULL )
    {
        return false;    
    }
	SDL_WM_SetCaption( "Yay!", NULL );
	SDL_ShowCursor(SDL_DISABLE);

	if (TTF_Init() == -1)
	{
		return false;
	}

	fontDate = TTF_OpenFont("/root/LCDMB___.TTF", 60);
	if (fontDate == NULL)
	{
		return false;
	}

	fontClock = TTF_OpenFont("/root/LCDMB___.TTF", 150);
	if (fontClock == NULL)
	{
		return false;
	}

	bool done = false;
	while ( !done )
	{
		while (SDL_PollEvent ( &event ) )
		{
			if ( event.type == SDL_KEYDOWN )
			{
				if ( event.key.keysym.sym == SDLK_ESCAPE )
					done = true;
			} else
			if ( event.type == SDL_QUIT )
				done = true;
		}

		time_t currTime = time(NULL);
		struct tm *localTime = localtime(&currTime);

		char dateStr[10];
		sprintf(dateStr, "%02d.%02d.%04d", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);

		date = TTF_RenderText_Solid(fontDate, dateStr, textColor);
		if (date == NULL)
		{
			return false;
		}
		char clockStr[8];
		char *col = " ";
		if ((localTime->tm_sec % 2) == 0)
			col = ":";
		sprintf(clockStr, "%02d%s%02d", localTime->tm_hour, col, localTime->tm_min);
		clock = TTF_RenderText_Blended(fontClock, clockStr, textColor);
		if (clock == NULL)
		{
			return false;
		}
		SDL_Rect dateLocation = { (SCREEN_WIDTH - date->w) / 2, 5, 0, 0 };
		SDL_Rect clockLocation = { (SCREEN_WIDTH - clock->w) / 2, date->h + 20, 0, 0 };
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(date, NULL, screen, &dateLocation);
		SDL_BlitSurface(clock, NULL, screen, &clockLocation);

		if (SDL_Flip(screen) == -1)
		{
			return false;
		}

		SDL_FreeSurface(date);
		SDL_FreeSurface(clock);
	}

	TTF_CloseFont(fontDate);
	TTF_CloseFont(fontClock);

	TTF_Quit();
	SDL_Quit();

	return 0;
}
