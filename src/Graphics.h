#pragma once

#include <cstdint>
#include <array>
#include <SDL2/SDL.h>

class Graphics
{
public:
	Graphics(char const* title, int width, int height);
	~Graphics();
	void update(void const* pixels, int pitch);
	bool handleInput(std::array<uint8_t,16> &input);

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
};