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
	
	vec2i Lmid   ;
	vec2i Rmid   ;
	vec2i Bmid   ;
	vec2i Tmid   ;
	vec2i mid    ;
	vec2i TLeft  ;
	vec2i TRight ;
	vec2i Rbot   ;
	vec2i Lbot   ;

	std::vector<Cell>  cells;          // grid cells
	std::vector<Cell>  cellBuffer;
	//std::vector<vec3i*> pixels;        // output pixels
	Uint32* pixels; 
	int scale = 1;
	int ID = 0;
	
	CellGrid() {}
	~CellGrid() { delete[] pixels;	}

	CellGrid(vec2i pixelSpace, int scl)
	{
		scale = scl;
		pSize = pixelSpace;
		gSize = pSize / scale;
		cells.resize(gSize.x() * gSize.y());
		//pixels.resize(gSize.x() * gSize.y());
	}

	void init(vec2i pixelSpace, int scl)
	{
		scale = scl;
		pSize = pixelSpace;
		gSize = pSize / scale;
		cells.resize(gSize.x() * gSize.y());
		cellBuffer = cells;

		pixels = new Uint32[pSize.x() * pSize.y()];
		memset(pixels, 0, pSize.x() * pSize.y() * sizeof(Uint32));

		std::cout << "\n   pixelGrid initialized " << std::endl;
		std::cout << "         scale: " << scale << std::endl;
		std::cout << "     Grid Size: " << gSize.toString() << std::endl;
		std::cout << "    Pixel Size: " << pSize.toString() << std::endl;
		//cells[gSize.x() * gSize.y() - 1].print();

		TLeft  = 0;
		TRight = { gSize.x()     , 0             };
		Rbot   = { gSize.x()     , gSize.y()     };
		Lbot   = { 0             , gSize.y()     };
		Lmid   = { 0             , gSize.y() / 2 };
		Rmid   = { gSize.x()     , gSize.y() / 2 };
		Bmid   = { gSize.x() / 2 , gSize.y()     };
		Tmid   = { gSize.x() / 2 , 0             };
		mid    = { gSize.x() / 2 , gSize.y() / 2 };
	}

	vec2i gS() { return gSize;     };
	int   xS() { return gSize.x(); };
	int   yS() { return gSize.y(); };

	Uint32* printPixels()
	{
		resizeGrid(gSize, pSize);
		return pixels;
	}

	vec3i* scaleResCell(int x, int y) 
	{

		int x_ratio = (int)((gSize.x() << 16) / pSize.x()) + 1;
		int y_ratio = (int)((gSize.y() << 16) / pSize.y()) + 1;

		float x2, y2;
		x2 = (x * (x_ratio) >> 16);
		y2 = (y * (y_ratio) >> 16);

		int id = (y2 * gSize.x()) + x2;
		return &cells[id].rgb();
	}

	vec2i scaleResLocator(int x, int y, vec2i sourceDims)
	{

		int x_ratio = (int)((gSize.x() << 16) / sourceDims.x()) + 1;
		int y_ratio = (int)((gSize.y() << 16) / sourceDims.y()) + 1;

		float x2, y2;
		x2 = (x * (x_ratio) >> 16);
		y2 = (y * (y_ratio) >> 16);

		int id = (y2 * gSize.x()) + x2;
		return vec2i{ x2, y2 };
	}

	void resizeGrid(vec2i sourceRes, vec2i targetRes) 
	{
		//std::cout << "\n    resizing to: " << targetRes.toString() << std::endl; // << xSize << " , " << ySize << std::endl;
		for (int y = 0; y < targetRes.y(); ++y) 
		{
			for (int x = 0; x < targetRes.x(); ++x) 
			{
				pixels[(y * targetRes.x()) + x] = scaleResCell(x, y)->vtoUint32();
			}
		}
	}

	// drawing functions

	void simulate(int gen, int mod)
	{
		for (int i = 0; i < cells.size(); ++i)
		{
			cells[i].update();
			if (gen%mod ==0) cells[i].olden();
			// cells[i].olden();
		}
	}

	void setCell(vec2i pos, bool A)
	{
		cells[clamp(pos.y(), 0, yS() - 1) * xS() + clamp(pos.x(), 0, xS() - 1)].set(A);
		//std::cout << "\n cell:	" << clamp(pos.y(), 0, ySize - 1) * xSize + clamp(pos.x(), 0, xSize - 1)<< "  set to: "<< A << std::endl;
	}

	void setCellMod(vec2i pos, bool A)
	{
		int wX = (pos.x() > 0) ? (pos.x() % xS()) : (xS() + pos.x()-1);
		int wY = (pos.y() > 0) ? (pos.y() % yS()) : (yS() + pos.y()-1);
		vec2i wrap = { wX,wY };
		cells[wrap.y() * xS() + wrap.x()].set(A);
		//std::cout << "\n cell:	" << clamp(pos.y(), 0, ySize - 1) * xSize + clamp(pos.x(), 0, xSize - 1)<< "  set to: "<< A << std::endl;
	}
	void setBufferCell(vec2i pos, bool A)
	{
		cellBuffer[clamp(pos.y(), 0, yS() - 1) * xS() + clamp(pos.x(), 0, xS() - 1)].set(A);
		//std::cout << "\n cell:	" << clamp(pos.y(), 0, ySize - 1) * xSize + clamp(pos.x(), 0, xSize - 1)<< "  set to: "<< A << std::endl;
	}

	void setBufferCellMod(vec2i pos, bool A)
	{
		int wX = (pos.x() > 0) ? (pos.x() % xS()) : (xS() + pos.x() - 1);
		int wY = (pos.y() > 0) ? (pos.y() % yS()) : (yS() + pos.y() - 1);
		vec2i wrap = { wX,wY };
		cellBuffer[wrap.y() * xS() + wrap.x()].set(A);
		//std::cout << "\n cell:	" << clamp(pos.y(), 0, ySize - 1) * xSize + clamp(pos.x(), 0, xSize - 1)<< "  set to: "<< A << std::endl;
	}
	bool getCell(vec2i pos)
	{
		return cells[clamp(pos.y(), 0, yS() - 1) * xS() + clamp(pos.x(), 0, xS() - 1)].alive();
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
			if (x % (1+gMod) == 1)
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

	void line(vec2i A, vec2i B) // start vector A loop lerp to destination vector B
	{
		int range = A.distance(B);
		for (int a = 1; a<range; ++a)
		{
			vec2i lerpPos = A.lerpTo(B, (float)a / range);
			setCell(lerpPos, true);
		}
	}

	void shiftCells_legacy(vec2i dir, vec2i TL, vec2i BR)
	{
		std::vector<Cell> cellBuffer (xS() * yS());
		//std::cout << "\n direction:	" << dir.toString() << std::endl;
		
		int cX;
		int cY;
		int shiftX;
		int shiftY;

		for (int x = TL.x(); x < BR.x(); ++x)
		{
		for (int y = TL.y(); y < BR.y(); ++y)
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
	std::vector<Cell>& shiftCells(std::vector<Cell>& cellBuffer, vec2i dir, vec2i TL, vec2i BR, bool additive = 0)
	{
		int cX;
		int cY;
		int shiftX;
		int shiftY;

		for (int x = TL.x(); x < BR.x(); ++x)
		{
			for (int y = TL.y(); y < BR.y(); ++y)
			{
				cX = clamp(x, 1, xS() - 1);
				cY = clamp(y, 1, yS() - 1);
				shiftX = clamp(x + dir.x(), 1, xS() - 1);
				shiftY = clamp(y + dir.y(), 1, yS() - 1);
				if (additive)
				{
					if (cellBuffer[shiftY * xS() + shiftX].alive())
					cellBuffer[cY * xS() + cX].set(cells[shiftY * xS() + shiftX]);
				}
				else
				{
				cellBuffer[cY * xS() + cX].set(cells[shiftY * xS() + shiftX]);
				}
			}
		}
		return cellBuffer;
	}

	void symShift(vec2i dir, int R)
	{
		//std::vector<Cell> cellBuffer (xS() * yS());
		//cellBuffer = cells;

		switch (R%9) {
		case 0:
			      shiftCells(cellBuffer, { dir.x()     , dir.y() * -1 }, TLeft, Rmid);
			      shiftCells(cellBuffer, { dir.x()     , dir.y()      }, Lmid,  Rbot);
			break;
		case 1:
			      shiftCells(cellBuffer, { dir.x()     , dir.y() }, Tmid, Rbot);
				  shiftCells(cellBuffer, { dir.x() * -1, dir.y()      }, TLeft, Bmid);
			break;							        
		case 2:								        
			      shiftCells(cellBuffer, { dir.x()     , dir.y() }, Tmid, Rbot);
				  shiftCells(cellBuffer, { dir.x() * -1, dir.y()      }, TLeft, Bmid);
			break;							        
		case 3:								        
			      shiftCells(cellBuffer, { dir.x() * -1, dir.y() }, Tmid, Rbot);
				  shiftCells(cellBuffer, { dir.x()     , dir.y()      }, TLeft, Bmid);
			break;
		case 4:
			      shiftCells(cellBuffer, { dir.x()     , dir.y() * -1 }, TLeft, Rbot);
				  shiftCells(cellBuffer, { dir.x()     , dir.y()      }, TLeft, Rbot, true);
			break;
		case 5:
			      shiftCells(cellBuffer, dir, TLeft, Rbot);
			break;
		case 6:
			      shiftCells(cellBuffer, dir*-1, TLeft, Rbot);
			break;
		case 7:
			switch (R % 4) {
			case 0:	shiftCells(cellBuffer, { dir.x() * -1, dir.y() }, TLeft, mid); break;
			case 1:	shiftCells(cellBuffer, { dir.x()     , dir.y() }, Tmid, Rmid); break;
			case 2:	shiftCells(cellBuffer, { dir.x() * -1, dir.y() * -1 }, Lmid, Bmid); break;
			case 3:	shiftCells(cellBuffer, { dir.x()     , dir.y() * -1 }, mid, Rbot); break;
			}
		case 8:
			        shiftCells(cellBuffer, { dir.x()     , dir.y()      }, TLeft, mid);
			        shiftCells(cellBuffer, { dir.x() * -1, dir.y()      }, Tmid, Rmid);
			        shiftCells(cellBuffer, { dir.x()     , dir.y() * -1 }, Lmid, Bmid);
			        shiftCells(cellBuffer, { dir.x() * -1, dir.y() * -1 }, mid, Rbot);
			break;
		}

		//cells = cellBuffer;
	}

	std::vector<Cell*>  get2Dhood(vec2i pos)
	{
		std::vector<Cell*> result(8); // 8 neighbours
		int c = 0;
		//std::cout << "init pos sample: " << pos.toString() << std::endl;
		for (int i = 0; i < 9; ++i) // iterate 3x3 grid
		{
			int x = i % 3 - 1;		// subtract 1 to center the offset (-1, 0, 1)
			int y = (i - x) / 3 - 1;

			int px = pos.x() + x;
			int py = pos.y() + y;
			int id = py * gSize.x() + px;
			
			if (i!=4)				// id 4 is the center cell, dont count it as neighbour
			{
				++c;
				//std::cout << "pos sample (i): " << i << "__ (c): " << c << " ___ " << vec2i{ x, y }.toString() << " state: "<< cells[id].alive() << std::endl;
				result[c-1] = &(cells[id]);
			}
		}
		//std::cout << "return result " << std::endl;
		return result;
	}

	void CA2D_Sim()
	{
		//cellBuffer = cells;//(xS() * yS());
		//std::cout << "\n direction:	" << dir.toString() << std::endl;

		for (int x = 1; x < xS()-1; ++x)
		{
			for (int y = 1; y < yS() - 1; ++y)
			{
				//cX = clamp(x, 0, xS());
				//cY = clamp(y, 0, yS());
				std::vector<Cell*> nei = get2Dhood({ x, y });

				cellBuffer[y * xS() + x].CA2D( nei );

				//if (y == yS()/2) cellBuffer[cY * xS() + cX].print();
			}
		}

		//cells = cellBuffer;
	}
	
};