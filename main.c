#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	int quit = 0;
	SDL_Event e;
	int color = 1;
	time_t t;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		SDL_Quit();
		return(0);
	}

	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window could not be created! Error: %s\n", SDL_GetError());
		SDL_Quit();
		return(0);
	}

	srand((unsigned)time(&t));

	screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 200, 30));
	SDL_UpdateWindowSurface(window);

	while (quit == 0)
	{
		color = rand() % 13;
		switch (color)
		{
		default:
		case 0:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0, 0));	
			break;
		case 1:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 127, 0));
			break;
		case 2:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 0));
			break;
		case 3:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 127, 255, 0));
			break;
		case 4:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 255, 0));
			break;
		case 5:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 255, 127));
			break;
		case 6:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 255, 255));
			break;
		case 7:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 127, 255));
			break;
		case 8:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 255));
			break;
		case 9:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 127, 0, 255));
			break;
		case 10:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 127, 0, 255));
			break;
		case 11:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0, 255));
			break;
		case 12:
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0, 127));
			break;
		}
		SDL_UpdateWindowSurface(window);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = 1;
		}	
		
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return(0);
}