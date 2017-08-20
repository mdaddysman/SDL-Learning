#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void DrawBox(SDL_Renderer *r, SDL_Rect *box); 

int main(int argc, char* args[])
{
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	SDL_Renderer *renderer = NULL;
	int quit = 0;
	SDL_Event e;
	int color = 1;
	time_t t;
	SDL_Rect boxobj = {
		200,
		200,
		10,
		10,
	};

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return(0);
	}

	srand((unsigned)time(&t));
	boxobj.x = rand() % (SCREEN_WIDTH - 11) + 1;
	boxobj.y = rand() % (SCREEN_HEIGHT - 11) + 1;
	DrawBox(renderer, &boxobj);

	while (quit == 0)
	{		
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = 1;
		}	
		const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP] && !currentKeyStates[SDL_SCANCODE_DOWN])
			boxobj.y--;
		else if (currentKeyStates[SDL_SCANCODE_DOWN] && !currentKeyStates[SDL_SCANCODE_UP])
			boxobj.y++;
		if (currentKeyStates[SDL_SCANCODE_LEFT] && !currentKeyStates[SDL_SCANCODE_RIGHT])
			boxobj.x--;
		else if (currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
			boxobj.x++;

		if (boxobj.x < 1)
			boxobj.x = 1;
		else if (boxobj.x >(SCREEN_WIDTH - 11))
			boxobj.x = SCREEN_WIDTH - 11;

		if (boxobj.y < 1)
			boxobj.y = 1;
		else if (boxobj.y >(SCREEN_HEIGHT - 11))
			boxobj.y = SCREEN_HEIGHT - 11;
		
		DrawBox(renderer, &boxobj);
		SDL_Delay(15);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return(0);
}

void DrawBox(SDL_Renderer *r, SDL_Rect *box)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(r, box);
	SDL_RenderPresent(r);
}