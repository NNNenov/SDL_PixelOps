#pragma once

#include <string>
#include <vector>
#include "SDL.h"
#include "vecClass.h"

class genImage
{
public:

	genImage();
	~genImage();

	void init(const int xSize, const int ySize, SDL_Renderer* pRenderer);
	void setPixel(const int x, const int y, const vec3i col);
	void setPixel(const int x, const int y, const pixel8bit col);
	void setImage(std::vector<vec3i>& pic);

	vec3i getPixel(const int x, const int y);
	void displayAdapter();
	void displayThru(std::vector<vec3i*> pic);
	void displayThruUint(Uint32* pic);
protected:

	//funcs
	//Uint32 convertCol32(const vec3f col);
	void initTexture();

	//variables
	std::vector<vec3i> pixArray;

	int xSize, ySize; //image dimensions

	SDL_Renderer* pRenderer;
	SDL_Texture* pTexture;

};

