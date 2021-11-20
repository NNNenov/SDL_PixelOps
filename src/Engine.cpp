// Making A Game #1: Making The Game Loop : C++ And SDL2 Tutorial
// https://www.youtube.com/watch?v=44tO977slsU&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=4

#include "Engine.h"

//SDL_Texture * tstTex;
//SDL_Rect srcR, destR;
//SDL_Surface* eSurface; //= IMG_Load("../../assets/alexiSale.png");

Engine::Engine(){}

Engine::~Engine(){}

void Engine::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	winW = width;
	winH = height;

	if (fullscreen) { flags = SDL_WINDOW_RESIZABLE; }//SDL_WINDOW_RESIZABLE; }// SDL_WINDOW_FULLSCREEN; }


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "subsystems initialised." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, winW, winH, flags);
		if (window) { std::cout << "window created" << std::endl; }
		
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) 
		{ 
			std::cout << "renderer created" << std::endl; 
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			//SDL_SetWindowSize(window, winW, winH);
		}
			 isRunning = true;


			 m_image.init(winW, winH, renderer);
			 cGrid.init({ winW, winH }, unitScale);
			 
	} else { isRunning = false; }
}

void Engine::handleEvents()
{
	SDL_Event event;
	//SDL_PollEvent(&event);
		//Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT)
			{
				isRunning = false;
				break;
			}
		}
}


void Engine::update()
{
	int sl_A = 32;
	int sl_B = 16;
	int sl_C = 4;
	int secA = winW / sl_A;
	int secB = winW / sl_B;
	int secC = winW / sl_C;
		
	if (cnt == 0)
	{

		cGrid.pointGrid({ 50,50 } , 80);
		cGrid.boxGrid(  { 50,50 } , 40, 10);
		
	}



	for (int i = 0; i < 1; ++i)
	{
		if (midiStatus.CCid == 1)
		{
			cnt = midiStatus.CC;
		}
		if (midiStatus.CCid == 2)
		{
			custom = 9*((float)midiStatus.CC/128);
		}
		cGrid.cellBuffer = cGrid.cells;
		if (midiStatus.note.latch)
		{
			cnt ++;
			if (midiStatus.channel != 0)
			{
				custom++; 
			}
			midiStatus.note.latch = false;
		}
		if (midiStatus.note.gate)
		{
			float mapA = midiStatus.deltaBi() * secA * 2 + cnt % (1 + custom);
			int fancy = clamp<int>(mapA, int(0-secA+2), secA * 3);
			cGrid.pointGrid({ secA + sin((float)cnt / secC +cnt) * secA -fancy,  secA + cos((float)cnt / secC +custom + fancy) * 20 }, secA+fancy);
			if (midiStatus.channel == 1) cGrid.boxGrid({ secA,secA }, secA + secA * (1 + custom % 4), 10 * (1 + custom % 2));
		}
			
			vec2i dir = { sin((float)cnt / secA) * 2, cos((float)cnt / secA) * 2 * midiStatus.deltaBi()*2 };

			if (midiStatus.channel % 2 == 0)
			{
				if (midiStatus.note.gate)
				{
					cGrid.CA2D_Sim();
				}
				else cGrid.symShift(dir, custom);
			}
			else cGrid.symShift(dir, custom);
			custom = custom % 30;
		cGrid.cells = cGrid.cellBuffer;
		cGrid.simulate(cnt, 2);
		////
		
	}
	
}

void Engine::render()
{
	SDL_RenderClear(renderer);

	cGrid.resizeGrid( cGrid.gSize , { winW, winH });
	//m_image.displayThru(cGrid.pixels);
	m_image.displayThruUint(cGrid.pixels);
	SDL_RenderPresent(renderer);

}

void Engine::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Engine cleaned" << std::endl;
}
/*
SDL_Color GetPixelColor(const SDL_Surface* pSurface, const int X, const int Y)
{
	const int Bpp = pSurface->format->BytesPerPixel;
	const Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;
	const Uint32 PixelColor = (Uint32)pPixel;

	SDL_Color Color = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };

	SDL_GetRGBA(PixelColor, pSurface->format, &Color.r, &Color.g, &Color.b, &Color.a);

	return Color;
}

void SetPixelColor(SDL_Surface* pSurface, int X, int Y, Uint32 Pixel)
{
	Uint32* pPixels = (Uint32*)pSurface->pixels;

	pPixels[(Y * pSurface->w) + X] = Pixel;
}

*/