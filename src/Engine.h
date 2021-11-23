#pragma once
#include "SDL.h"
#include <iostream>
#include "SDL_image.h"
#include "genImage.h"
#include "PixelGrid.h"
#include "nikMidi.h"
#include "particle.h"

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

	// presets
	void CA_Fx();

	//particle test
	int readPartiID;
	std::vector<nParticle> parti{ 750 };

	// grid stuff
	const int unitScale = 1;
	CellGrid cGrid;
	genImage m_image;

	// Engine stuff
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int cnt = 1;
	int custom = 1;
	int winW, winH;
	vec2i resizeWin;

	vec2i mousePos = 0;
	vec2i mousePPos= 10;


};



