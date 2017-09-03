#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static const char *GAME_MUSIC = "Music/Game1.mp3";
static const char *WALL_SOUND = "Sounds/fail.wav";

void DrawBox(SDL_Renderer *r, SDL_Rect *box, int hitedge); 

int main(int argc, char* args[])
{
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	SDL_Renderer *renderer = NULL;
	Mix_Music *music = NULL;
	Mix_Chunk *wall = NULL;
	int quit = 0;
	int result = 0;
	int isedgehit = 0;
	SDL_Event e;
	int color = 1;
	time_t t;
	SDL_Rect boxobj = {
		200,
		200,
		10,
		10,
	};

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		SDL_Quit();
		return(0);
	}

	if (MIX_INIT_MP3 != (result = Mix_Init(MIX_INIT_MP3)))
	{
		printf("SDL Mixer could not initialize! Code: %d Error: %s\n",result, SDL_GetError());
		SDL_Quit();
		return(0);
	}

	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window could not be created! Error: %s\n", SDL_GetError());
		Mix_Quit();
		SDL_Quit();
		return(0);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		Mix_Quit();
		SDL_Quit();
		return(0);
	}

	srand((unsigned)time(&t));

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS(GAME_MUSIC);
	wall = Mix_LoadWAV(WALL_SOUND);
	Mix_PlayMusic(music, -1);

	boxobj.x = rand() % (SCREEN_WIDTH - 11) + 1;
	boxobj.y = rand() % (SCREEN_HEIGHT - 11) + 1;
	DrawBox(renderer, &boxobj,0);

	while (quit == 0)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = 1;
		}
		isedgehit = 0;
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
		{
			boxobj.x = 1;
			isedgehit = 1;
			Mix_PlayChannel(-1, wall, 0);
		}
		else if (boxobj.x >(SCREEN_WIDTH - 11))
		{
			boxobj.x = SCREEN_WIDTH - 11;
			isedgehit = 1;
			Mix_PlayChannel(-1, wall, 0);
		}

		if (boxobj.y < 1)
		{
			boxobj.y = 1;
			isedgehit = 1;
			Mix_PlayChannel(-1, wall, 0);
		}
		else if (boxobj.y >(SCREEN_HEIGHT - 11))
		{
			boxobj.y = SCREEN_HEIGHT - 11;
			isedgehit = 1;
			Mix_PlayChannel(-1, wall, 0);
		}

		DrawBox(renderer, &boxobj,isedgehit);
		SDL_Delay(15);
	}

	Mix_HaltMusic();
	Mix_FreeMusic(music);
	Mix_FreeChunk(wall);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	SDL_Quit();
	return(0);
}

void DrawBox(SDL_Renderer *r, SDL_Rect *box, int hitedge)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(r);
	if (hitedge == 1)
		SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(r, box);
	SDL_RenderPresent(r);
}