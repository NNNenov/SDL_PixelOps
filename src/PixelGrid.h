#pragma once
#include <vector>
#include "genImage.h"
#include "CellClass.h"

class CellGrid
{
public:

	vec2i coord = { 0,0 };       // cell grid top left corner
	vec2i gSize;                 // grid resolution
	vec2i pSize;                 // output resolution
	
	std::vector<Cell>  cells;    // grid cells
	std::vector<vec3i> pixels;   // output pixels
	
	int scale = 1;
	int ID = 0;
	
	CellGrid() {}
	~CellGrid() {}

	CellGrid(vec2i pixelSpace, int scl)
	{
		scale = scl;
		pSize = pixelSpace;
		gSize = pSize / scale;
		cells.resize(gSize.x() * gSize.y());
		pixels.resize(gSize.x() * gSize.y());
	}

	void init(vec2i pixelSpace, int scl)
	{
		scale = scl;
		pSize = pixelSpace;
		gSize = pSize / scale;
		cells.resize(gSize.x() * gSize.y());
		pixels.resize(pSize.x() * pSize.y());
		std::cout << "\n   pixelGrid initialized " << std::endl;
		std::cout << "         scale: " << scale << std::endl;
		std::cout << "     Grid Size: " << gSize.toString() << std::endl;
		std::cout << "    Pixel Size: " << pSize.toString() << std::endl;
		//cells[gSize.x() * gSize.y() - 1].print();
	}

	vec2i gS() { return gSize;     };
	int   xS() { return gSize.x(); };
	int   yS() { return gSize.y(); };

	std::vector<vec3i> printPixels()
	{
		resizeGrid(gSize, pSize);
		return pixels;
	}

	Cell scaleResCell(int x, int y) 
	{

		int x_ratio = (int)((gSize.x() << 16) / pSize.x()) + 1;
		int y_ratio = (int)((gSize.y() << 16) / pSize.y()) + 1;

		float x2, y2;
		x2 = (x * (x_ratio) >> 16);
		y2 = (y * (y_ratio) >> 16);

		int id = (y2 * gSize.x()) + x2;
		return cells[id]; 
	}

	void resizeGrid(vec2i sourceRes, vec2i targetRes) 
	{
		//std::cout << "\n    resizing to: " << targetRes.toString() << std::endl; // << xSize << " , " << ySize << std::endl;
		for (int y = 0; y < targetRes.y()-1; ++y) 
		{
			for (int x = 0; x < targetRes.x()-1; ++x) 
			{
				pixels[(y * targetRes.x()) + x] = scaleResCell(x, y).rgb();
			}
		}
	}

	// drawing functions

	void simulate()
	{
		for (int i = 0; i < cells.size(); ++i)
		{
			cells[i].update();
			cells[i].olden();
		}
	}

	void setCell(vec2i pos, bool A)
	{
		cells[clamp(pos.y(), 0, yS() - 1) * xS() + clamp(pos.x(), 0, xS() - 1)].set(A);
		//std::cout << "\n cell:	" << clamp(pos.y(), 0, ySize - 1) * xSize + clamp(pos.x(), 0, xSize - 1)<< "  set to: "<< A << std::endl;
	}

	void box(const vec2i TL, const vec2i wh)
	{
		const vec2i A = TL;
		const vec2i B = wh;

		for (int i = 0; i < wh.x(); ++i)
		{
			setCell({ TL.x() + i      , TL.y() }, 1);
			setCell({ TL.x() + i      , TL.y() + wh.y() }, 1);
		}

		for (int i = 0; i < wh.y() + 1; ++i)
		{
			setCell({ TL.x()         , TL.y() + i }, 1);
			setCell({ TL.x() + wh.x()  , TL.y() + i }, 1);
		}
	}

	void pointGrid(const vec2i off , const int gMod)
	{
		for (int x = 1 + off.x(); x < xS() - 1 - off.x(); ++x)
		{
			if (x % gMod == 0)
			{
				for (int y = 1 + off.y(); y < yS() - 1 - off.y(); ++y)
				{
					if (y % gMod == 0)
					{
						setCell({ x , y }, 1);
					}
				}
			}
		}
	}

	void boxGrid(const vec2i off, const int gMod, int s)
	{
		for (int x = 1 + off.x(); x < xS() - 1 - off.x(); ++x)
		{
			if (x % gMod == 0)
			{
				for (int y = 1 + off.y(); y < yS() - 1 - off.y(); ++y)
				{
					if (y % gMod == 0)
					{
						box({ x-s/2 , y-s/2 }, s);
					}
				}
			}
		}
	}

	void shiftCells(vec2i dir, bool cl = false)
	{
		std::vector<Cell> cellBuffer(xS()* yS());
		//std::cout << "\n direction:	" << dir.toString() << std::endl;
		
		int cX;
		int cY;
		int shiftX;
		int shiftY;

		for (int x = xS(); x >0  ; --x)
		{
			for (int y = 0; y < yS() - 1; ++y)
			{
				cX = clamp(x, 1, xS() - 1);
				cY = clamp(y, 1, yS() - 1);
				shiftX = clamp(x + dir.x(), 1, xS() - 1);
				shiftY = clamp(y + dir.y(), 1, yS() - 1);

				cellBuffer[cY * xS() + cX].set( cells[shiftY * xS() + shiftX]);

				//if (y == yS()/2) cellBuffer[cY * xS() + cX].print();
			}
		}

		cells = cellBuffer;
	}


};