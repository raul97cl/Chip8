#include <iostream>
#include "Chip.h"
#include <stdlib.h>
#include <time.h>


using namespace std;

int main(int argc, char **argv)
{
	int last_tick = 0;

	//Inicia emulador
	Chip *chip8 = new Chip();
	chip8->initialize();
	chip8->load_rom("PONG");

	//Inicia el motor SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Chip-8",
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  640, 320,
										  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,
											 SDL_TEXTUREACCESS_STREAMING,
											 64, 32);
	SDL_Surface *surface = SDL_CreateRGBSurface(0, 64, 32, 32,
													0x00FF0000,
													0x0000FF00,
													0x000000FF,
													0xFF000000);
	SDL_LockTexture(texture, nullptr, &surface->pixels, &surface->pitch);
	chip8->screen_conversion((Uint32*)surface->pixels);
	SDL_UnlockTexture(texture);

	int quit = 0;
	SDL_Event event;
	while(!quit)
	{


		//Evento de fin de bucle
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
			}

		}

		if(SDL_GetTicks() - last_tick > (1000 / 60))
		{
			chip8->initialize();

			SDL_LockTexture(texture, nullptr, &surface->pixels, &surface->pitch);
			chip8->screen_conversion((Uint32*)surface->pixels);
			SDL_UnlockTexture(texture);

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);
			SDL_RenderPresent(renderer);
			last_tick = SDL_GetTicks();
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	delete chip8;

	return 0;
}

