#pragma once
#include "vecClass.h"
#include "PixelGrid.h"

class nParticle
{
public:
	vec2i pos;
	vec2f dir;
	float mag;

	vec2i bounds;

	int maxAge = 100;
	int age = 0;
	
	int scanDist = 1;
	int scanRange = 100;

	nParticle() {}
	~nParticle() {}

	nParticle(vec2i p, vec2i b) { pos = p; bounds = b; }

	bool alive() { return (age > 0); }
	void reset(vec2i p) { 
		pos = p; 
		age = maxAge; 
		scanDist = 1;

	}

	void growCircle(CellGrid& cells)
	{
		++scanDist;
		scanDist = (clamp(scanDist, 0, scanRange));
		int xmin = pos.x() - scanDist;
		int xmax = pos.x() + scanDist + 1;
		int ymin = pos.y() - scanDist;
		int ymax = pos.y() + scanDist + 1;
		if (scanDist < scanRange)
		{
			for (int x = xmin; x < xmax; ++x)
			{
				for (int y = ymin; y < ymax; ++y)
				{
					// draw circle
					vec2i P = { x,y };
					//int id = (y * res.x()) + x;
					if (P.distance(pos) == scanDist)
					{
						cells.setCellMod(P, true);
					}
					else if (!cells.getCell(pos))
					{
						cells.setCellMod(P, false);
					}
				}
			}
		}
	}

	void olden(int amt = 1)
	{
		age = age - amt;
		age = clamp(age, 0, maxAge);
	}

	void simulate()
	{
		pos = ( pos + vec2i{dir.x()* mag, dir.y()* mag} );
		olden();
	}

	void display(CellGrid & cells)
	{
		if (age > 0) { cells.setCell(pos, true); }
	}

	void randoff()
	{
		dir = vec2f{ randomDouble() * 2, randomDouble() * 2 } -1;
		mag = 2;
		//updatePos({ randomDouble() * 2, randomDouble() * 2 });
	}

};