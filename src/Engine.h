#pragma once
#include "SDL.h"
#include <iostream>
#include "SDL_image.h"
#include "genImage.h"
#include "PixelGrid.h"
#include "nikMidi.h"

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

	bool running() { return isRunning; }

	vec3i chekUV(const int x, const int y)
	{
		float r = (float(x) / winW) * 255;
		float g = (float(y) / winH) * 255;
		return { r, g, 0 };
	}

	//midi
	channelData midiStatus;

protected:
	// grid stuff
	const int unitScale = 2;
	CellGrid cGrid;
	genImage m_image;

	// Engine stuff
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int cnt =0;
	int custom = 0;
	int winW, winH;

};



