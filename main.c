#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SCOREBAR_HEIGHT = 30;
const int PLAYAREA_PADDING = 4; 
const int PLAYAREA_BORDER = 6; 

static const char *GAME_MUSIC = "Music/Game1.mp3";
static const char *WALL_SOUND = "Sounds/fail.wav";

enum BoxColors {BLACK = 0, WHITE, RED, BLUE, ORANGE};

void DrawBox(SDL_Renderer *r, SDL_Rect *box, enum BoxColors color); 
SDL_Texture* makeTextTexture(SDL_Renderer *r, TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

int main(int argc, char* args[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	Mix_Music *music = NULL;
	Mix_Chunk *wall = NULL;
	TTF_Font *font = NULL;
	SDL_Texture *tlevel_number = NULL;
	int quit = 0;
	int result = 0;
	short int isedgehit = 0;
	SDL_Event e;
	enum BoxColors playercolor = WHITE;
	time_t t;
	SDL_Rect playerbox = {
		200,
		200,
		10,
		10,
	};
	SDL_Rect border = {
		PLAYAREA_PADDING,
		SCOREBAR_HEIGHT + PLAYAREA_PADDING,
		SCREEN_WIDTH - 2 * PLAYAREA_PADDING,
		SCREEN_HEIGHT - SCOREBAR_HEIGHT - 2 * PLAYAREA_PADDING
	};
	SDL_Rect playarea = {
		PLAYAREA_PADDING + PLAYAREA_BORDER,
		SCOREBAR_HEIGHT + PLAYAREA_PADDING + PLAYAREA_BORDER,
		SCREEN_WIDTH - 2 * PLAYAREA_PADDING - 2 * PLAYAREA_BORDER,
		SCREEN_HEIGHT - SCOREBAR_HEIGHT - 2 * PLAYAREA_PADDING - 2 * PLAYAREA_BORDER
	};
	int left_edge, right_edge, top_edge, bottom_edge;
	int movespeed = 1; //pixels moved per frame 
	SDL_Color text_color = { 255, 255, 255 };
	SDL_Color bg_color = { 0, 0, 0 };
	SDL_Rect level_rect;
	int w, h;
	left_edge = playarea.x;
	top_edge = playarea.y;
	right_edge = playarea.x + playarea.w - playerbox.w;
	bottom_edge = playarea.y + playarea.h - playerbox.h;

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

	if (TTF_Init() < 0)
	{
		printf("SDL TTF could not initialize! Error: %s\n", SDL_GetError());
		Mix_Quit();
		SDL_Quit();
		return(0);
	}

	window = SDL_CreateWindow("Box Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window could not be created! Error: %s\n", SDL_GetError());
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();
		return(0);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();
		return(0);
	}

	font = TTF_OpenFont("Fonts/RipeApricots.ttf", 36);
	if (font == NULL)
	{
		printf("Font could not be loaded Error:%s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();
		return(0);
	}

	srand((unsigned)time(&t));

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS(GAME_MUSIC);
	wall = Mix_LoadWAV(WALL_SOUND);
	Mix_PlayMusic(music, -1);
	tlevel_number = makeTextTexture(renderer, font, "Level: 1", text_color, bg_color);
	SDL_QueryTexture(tlevel_number, NULL, NULL, &w, &h);
	level_rect.x = PLAYAREA_PADDING;
	level_rect.y = PLAYAREA_PADDING;
	level_rect.w = w;
	level_rect.h = h;


	playerbox.x = rand() % playarea.w + playarea.x;
	playerbox.y = rand() % playarea.h + playarea.y;
	

	while (quit == 0)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = 1;
		}
		isedgehit = 0;
		playercolor = WHITE;
		movespeed = 1;
		const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_LCTRL] || currentKeyStates[SDL_SCANCODE_RCTRL])
		{
			movespeed = 3;
		}

		if (currentKeyStates[SDL_SCANCODE_UP] && !currentKeyStates[SDL_SCANCODE_DOWN])
			playerbox.y = playerbox.y - movespeed;
		else if (currentKeyStates[SDL_SCANCODE_DOWN] && !currentKeyStates[SDL_SCANCODE_UP])
			playerbox.y = playerbox.y + movespeed;
		if (currentKeyStates[SDL_SCANCODE_LEFT] && !currentKeyStates[SDL_SCANCODE_RIGHT])
			playerbox.x = playerbox.x - movespeed;
		else if (currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
			playerbox.x = playerbox.x + movespeed;

		if (playerbox.x < left_edge)
		{
			playerbox.x = left_edge;
			isedgehit = 1;
		}
		else if (playerbox.x > right_edge)
		{
			playerbox.x = right_edge;
			isedgehit = 1;
		}

		if (playerbox.y < top_edge)
		{
			playerbox.y = top_edge;
			isedgehit = 1;
		}
		else if (playerbox.y > bottom_edge)
		{
			playerbox.y = bottom_edge;
			isedgehit = 1;
		}

		if (isedgehit == 1)
		{
			Mix_PlayChannel(-1, wall, 0);
			playercolor = RED;
		}

		//clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		//draw all the text
		SDL_RenderCopy(renderer, tlevel_number, NULL, &level_rect);
		//draw all the boxes
		DrawBox(renderer, &border, playercolor);
		DrawBox(renderer, &playarea, BLACK);
		DrawBox(renderer, &playerbox,playercolor);
		SDL_RenderPresent(renderer); //present the frame 
	}

	Mix_HaltMusic();
	Mix_FreeMusic(music);
	Mix_FreeChunk(wall);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return(0);
}

void DrawBox(SDL_Renderer *r, SDL_Rect *box, enum BoxColors color)
{
	//enum BoxColors {BLACK = 0, WHITE, RED, BLUE, ORANGE}
	switch(color)
	{
	case WHITE:
		SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
		break;
	case BLACK:
		SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
		break;
	case RED:
		SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_OPAQUE);
		break;
	case BLUE:
		SDL_SetRenderDrawColor(r, 0, 255, 255, SDL_ALPHA_OPAQUE);
		break;
	case ORANGE:
		SDL_SetRenderDrawColor(r, 255, 165, 0, SDL_ALPHA_OPAQUE);
		break;
	default:
		SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
		break;
	}		
	SDL_RenderFillRect(r, box);

}

SDL_Texture* makeTextTexture(SDL_Renderer *r, TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg)
{
	SDL_Surface *text_surface = NULL;
	SDL_Texture *text_texture = NULL;

	if (!(text_surface = TTF_RenderText_Shaded(font, text, fg, bg)))
	{
		printf("Error creating text surface. Error: %s", TTF_GetError());
		return(NULL);
	}

	text_texture = SDL_CreateTextureFromSurface(r, text_surface);
	SDL_FreeSurface(text_surface);
	text_surface = NULL;
	return(text_texture);
}