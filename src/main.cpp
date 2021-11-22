#include "Engine.h"
#include "nikMidi.h"

Engine* engine = nullptr;
MidiBaron midier;

int main(int argc, char* argv[])
{
	// midi
	midier.init(argc, argv);
	midier.midiin->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.

	// engine
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	engine = new Engine();

	engine->init("pixelOps", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920/4, 1080/4, true);
	
	while (engine->running())
	{
		frameStart = SDL_GetTicks();
		engine->midiStatus = midier.probeInput();
		engine->handleEvents();
		engine->update();
		engine->render();
		frameTime = SDL_GetTicks() - frameStart;


		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	engine->clean();

	return 0;
}