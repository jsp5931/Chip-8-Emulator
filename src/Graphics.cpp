#include "Graphics.h"
#include <SDL2/SDL.h>
#include <iostream>


Graphics::Graphics(char const* title, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		std::exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*12, height*12, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

Graphics::~Graphics()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Graphics::update(void const* pixels, int pitch)
{
	SDL_UpdateTexture(texture, nullptr, pixels, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

bool Graphics::handleInput(std::array<uint8_t,16> &input)
{
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event)){

		int status = 0;
		switch (event.type){
			case SDL_QUIT:
			{
				quit = true;
				break;
			} 
			case SDL_KEYDOWN:
				status = 1;
				break;
			case SDL_KEYUP:
				status = 0;
				break;
			}

	
		switch (event.key.keysym.sym){
			case SDLK_ESCAPE:
			{	
				quit = true;
				break;
			} 
			case SDLK_x:
			{
				input[0x0] = status;
				break;
			} 
			case SDLK_1:
			{
				input[0x1] = status;
				break;
			} 
			case SDLK_2:
			{
				input[0x2] = status;
				break;
			} 
			case SDLK_3:
			{
				input[0x3] = status;
				break;
			} 
			case SDLK_q:
			{
				input[0x4] = status;
				break;
			} 
			case SDLK_w:
			{
				input[0x5] = status;
				break;
			} 
			case SDLK_e:
			{
				input[0x6] = status;
				break;
			} 
			case SDLK_a:
			{
				input[0x7] = status;
				break;
			} 
			case SDLK_s:
			{
				input[0x8] = status;
				break;
			} 
			case SDLK_d:
			{
				input[0x9] = status;
				break;
			} 
			case SDLK_z:
			{
				input[0xA] = status;
				break;
			} 
			case SDLK_c:
			{
				input[0xB] = status;
				break;
			} 
			case SDLK_4:
			{
				input[0xC] = status;
				break;
			} 
			case SDLK_r:
			{
				input[0xD] = status;
				break;
			} 
			case SDLK_f:
			{
				input[0xE] = status;
				break;
			} 
			case SDLK_v:
			{
				input[0xF] = status;
				break;
			} 
		}
	}

	return quit;
}
