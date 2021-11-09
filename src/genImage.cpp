#include "genImage.h"

// default constructor
genImage::genImage() 
{
	xSize = 0;
	ySize = 0;
	pTexture = NULL;
}

// destructor
genImage::~genImage()
{
	if (pTexture != NULL)
	SDL_DestroyTexture(pTexture);
}

void genImage::init(const int xS, const int yS, SDL_Renderer* pR)
{
	// resize the vector array (new size, value for new items)
	pixArray.resize(xS * yS, { 0,0,0 });

	xSize = xS;
	ySize = yS;

	// store the pointer to the renderer
	pRenderer = pR;

	// initialize the texture
	initTexture();
}

void genImage::setPixel(const int x, const int y, const vec3i col)
{
	pixArray.at(clamp(y, 0, ySize - 1) * xSize + clamp(x, 0, xSize - 1)) = col;
}
void genImage::setPixel(const int x, const int y, const pixel8bit col)
{
	pixArray.at(clamp(y, 0, ySize - 1) * xSize + clamp(x, 0, xSize - 1)) = { col.r() , col.g(), col.b() };
}

vec3i genImage::getPixel(const int x, const int y)
{
	return pixArray[clamp(y, 0, ySize-1) * xSize + clamp(x, 0, xSize-1)];
}

void genImage::setImage(std::vector<vec3i>& pic)
{
	for (int i = 0; i < xSize * ySize; ++i)
	{
		pixArray[i] = pic[i];
	}
}

void genImage::displayAdapter()
{
	// temp pixel buffer
	Uint32* tempPixels = new Uint32[xSize * ySize];

	// clear pixel buffer
	memset(tempPixels, 0, xSize * ySize * sizeof(Uint32)); 
	// memset asks for a memory address, then fills it with a 
	// value starting from that address up to the size defined by the 
	// 3rd argument (start address, value, how much address 
	// to fill with that value.

	for (int i = 0; i < xSize * ySize; ++i)
	{
		tempPixels[i] = pixArray[i].vtoUint32();
	}

	// update the texture with the temp buffer
	SDL_UpdateTexture(pTexture, NULL, tempPixels, xSize * sizeof(Uint32)); //https://youtu.be/JN5yUrJPThI?t=2804

	// delete the pixel buffer
	delete[] tempPixels;

	// copy texture to renderer

	//SDL_Rect srcRect, bounds;
	//srcRect.x = 0;
	//srcRect.y = 0;
	//srcRect.w = xSize;
	//srcRect.h = ySize;
	//bounds = srcRect;
	SDL_RenderCopy(pRenderer, pTexture, NULL, NULL); //&srcRect, &bounds); // maybe try use blit instead?
}

void genImage::displayThru(std::vector<vec3i> &pic)
{
	//std::cout << "\n     display thru xy size: " << std::endl; // << xSize << " , " << ySize << std::endl;
	// temp pixel buffer
	Uint32* tempPixels = new Uint32[xSize * ySize];

	// clear pixel buffer
	memset(tempPixels, 0, xSize * ySize * sizeof(Uint32));

	for (int i = 0; i < xSize * ySize; ++i)
	{
		tempPixels[i] = pic[i].vtoUint32();
		//tempPixels[i] = vec3i(70,0,120).vtoUint32();
	}
	
	// update the texture with the temp buffer
	SDL_UpdateTexture(pTexture, NULL, tempPixels, xSize * sizeof(Uint32)); //https://youtu.be/JN5yUrJPThI?t=2804

	// delete the pixel buffer
	delete[] tempPixels;

	SDL_RenderCopy(pRenderer, pTexture, NULL, NULL); //&srcRect, &bounds); // maybe try use blit instead?
}


// function to turn initialize the texture

void genImage::initTexture()
{
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	// delete any previously created texture
	if (pTexture != NULL) SDL_DestroyTexture(pTexture);

	SDL_Surface* tempSurface = SDL_CreateRGBSurface(0, xSize, ySize, 32, rmask, gmask, bmask, amask);
	pTexture = SDL_CreateTextureFromSurface(pRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	// std::cout << "texture initialized" << std::endl;
}


// aside from being used for namespaces, 
// the syntax :: also can mean you are saying this is a thing which
// belongs to the class definition genImage
// it helps to have cleaner code, you can define functions outside 
// of the class definition, c++ knows its meant to be in that class definition.
