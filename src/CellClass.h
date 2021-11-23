#pragma once

class Cell
{

public:

	int maxAge = 150;
	int age = 0;
	vec3i col = { 0,0,0 };
	vec3i* colPtr{};
	

	// cellular automata parameters
	std::vector<int> B = { 2 };     // living neighbours required to be born (if dead)
	std::vector<int> S = { 3 , 4 , 5 }; // living neighbours required to survive (if alive)
	int ref = 32;                    // refractory states // countdown after dying before exposing to calculation
	int r = 0;                      // refractory state buffer

	std::vector<vec3i> palette{
								vec3i( 4   , 5   , 10  ) ,
								vec3i( 5   , 10  , 15  ) ,
								vec3i( 1   , 12  , 22  ) ,
								vec3i( 10   , 10  , 40  ) ,
								vec3i( 30   , 70  , 150 ) ,
								vec3i( 80 , 150 , 255 ) ,
								vec3i( 215 , 220 , 255 ) };

	int pid = 0;

	Cell() {}
	~Cell() {}
	Cell(int A, int L)
	{
		maxAge = A;
		age = L;
	}
	Cell(bool A, int L)
	{
		maxAge = L;
		if (A) age = L;
	}

	void set(Cell & rhs)
	{
		maxAge = rhs.maxAge;
		age = rhs.age;
		pid = rhs.pid;
	}

	bool alive() { return (age > maxAge - ref); }

	float ageNormal()    { return (age>0) ? (float(age) / float(maxAge)) : 0; }
	float refracNormal() { return (r > 0) ? (float(r) / ref) : 0; }

	vec3i & rgb()
	{
		//std::cout << "ageNormal: " << ageNormal() << std::endl;
		//std::cout << "refracNormal: " << refracNormal() << std::endl;
		vec3i pal = palette[clamp(pid, 0, 6)];
		pal = vec3i{ (float)pal.r()*refracNormal() , pal.g() *ageNormal() , pal.b()};
		//col = pal;
		//colPtr = &col;
		return pal;
	}     

	void olden(int amt = 1)
	{
		age = age - amt;
		age = clamp(age, 0, maxAge);
		//update(alive);
	}

	void set(bool s)
	{
		age = s? maxAge : 0;
		update();
	}

	void update()
	{
		pid = (float)palette.size() * ageNormal();
		
	}

	Cell& CA2D(std::vector<Cell*>& nei)  // 2d cellular automata
	{
		int liveN = 0;                  // count neighbours

		for (int i = 0; i < nei.size(); ++i)
		{
			if (nei[i]->alive()) { ++liveN; }
		}

		if (r == 0)
		{
			if (alive())
			{
				int s = 0;
				for (int i = 0; i < S.size(); ++i)
				{
					if (liveN == S[i]) { ++s; } // increment s if any survival count rule matches number of living neighbours
				}
				if (s == 0) { r = ref; age = maxAge - ref; } // if no rule matches occured, s == 0, kill cell and set refractory timer to max
			}
			else 
			{
				int s = 0;
				for (int i = 0; i < B.size(); ++i)
				{
					if (liveN == B[i]) { ++s; }
				}
				if (s > 0) { age = maxAge; }
			}
		}
		else
		{
			//--r;
			olden();
			r = clamp(--r, 0, ref);
		}

		//Cell result = Cell(maxAge, age);
		return *this;//result;
	}

	void print()
	{
		std::cout << "\n    rgb() = " << rgb().toString(1) <<
			         "\n   maxAge = " << maxAge <<
			         "\n      age = " << age <<
			         "\nageNormal = " << ageNormal() <<
			         "\n  alive() = " << alive() << std::endl;
	}
};
