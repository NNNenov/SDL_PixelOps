#pragma once
#include "vecClass.h"
#include "PixelGrid.h"

class nParticle
{
public:
	vec2i pos = 0;
	vec2f dir = 0;
	float mag = 0;

	vec2i bounds;

	int maxAge = 100;
	int age = 0;
	
	int scanDist = 1;
	int scanRange = 100;

	int pID;

	bool latch = false;
	bool ignore = false;

	nParticle *nearestParticle;

	vec2i posNearest = 0;
	int distNearest = scanRange;

	nParticle() { }
	~nParticle() { delete nearestParticle; }

	nParticle(vec2i p, vec2i b) { pos = p; bounds = b; }

	void updateNearest(std::vector<nParticle>& partis)
	{
		float accum = scanRange; // init accumulator with this particle's max scan range;
		int targetID = 0;
		//printf("recieved particle array with size: %d", partis.size());
		for (int i = 0; i < partis.size(); ++i)
		{
			if (pID != partis[i].pID)
			{
				float dist = partis[i].pos.distance(pos);
				if (dist < accum && !partis[i].ignore)
				{
					accum = dist;
					targetID = partis[i].pID;
				};
			}
		}
		posNearest = partis[targetID].pos;
		distNearest = accum;
		nearestParticle = &partis[targetID];
		//return &partis[targetID];
	}

	bool alive() { return (age > 0); }
	void reset(vec2i p) { 
		pos = p; 
		age = maxAge; 
		scanDist = 1;
		latch = false;
	}

	void growCircle(CellGrid& cells)
	{
		mag = 0;
		if (distNearest < scanDist)
		{
			//printf("\ndistNearest: %d", distNearest);
			age = 0;
		} 
		else
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
			//else { scanDist = 1; }
		}
	}

	void olden(int amt = 1)
	{
		age = age - amt;
		age = clamp(age, 0, maxAge);
	}

	void setPos(vec2i p)
	{
		if (bounds.x() > 0)
		{
			int wX = (p.x() > 0) ? (p.x() % bounds.x()) : (bounds.x() + p.x() - 1);
			int wY = (p.y() > 0) ? (p.y() % bounds.y()) : (bounds.y() + p.y() - 1);
			vec2i wrap = { wX,wY };
			pos = wrap;
		}
	}

	void simulate()
	{
		setPos( pos + vec2i{dir.x()* mag, dir.y()* mag} );
		//olden();
		if (age < maxAge / 2) ignore = false;
		if (!alive()) mag = 0;
	}

	void display(CellGrid & cells)
	{
		if (age > 0) { cells.setCell(pos, true); }
		else
		{
			if (!nearestParticle->ignore && !nearestParticle->latch)
			{
				nearestParticle->latch = true;
				ignore = true;
				nearestParticle->ignore = true;
			}
			cells.line(pos, posNearest);
		}
	}

	void randoff()
	{
		dir = vec2f{ randomDouble() * 2, randomDouble() * 2 } -1;
		mag = 2;
		//updatePos({ randomDouble() * 2, randomDouble() * 2 });
	}


	void randFlow()
	{
		if (!latch)
		{
			dir += vec2f{ sin(randomDouble() * 6), cos(randomDouble() * 6) }*0.1;
			dir = unit_vector(dir);
			mag = 2* randomDouble();
			//updatePos({ randomDouble() * 2, randomDouble() * 2 });
		}

		
	}

};