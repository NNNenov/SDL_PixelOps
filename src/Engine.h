#pragma once
#include "SDL.h"
#include <iostream>
#include "SDL_image.h"
#include "genImage.h"


class Engine
{
public:
	Engine();
	~Engine();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	
	void CA_Op_Debug(int cnt);
	void directionMod(int cnt);

	bool running() { return isRunning; }

	vec3i chekUV(const int x, const int y)
	{
		float r = (float(x) / winW) * 255;
		float g = (float(y) / winH) * 255;
		return { r, g, 0 };
	}

protected:
	// CA stuff
	const int unitScale = 4;
	genImage m_image;

	// Engine stuff
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int cnt =0;

	int winW, winH;

};


