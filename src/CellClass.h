#pragma once

class Cell
{

public:
	int maxAge = 50;
	int age = 0;

	std::vector<vec3i> palette{
								vec3i( 0   , 0   , 0   ) ,
								vec3i( 0   , 0   , 15  ) ,
								vec3i( 1   , 5   , 22  ) ,
								vec3i( 2   , 10  , 40  ) ,
								vec3i( 6   , 40  , 150 ) ,
								vec3i( 215 , 240 , 255 ) };

	int pid = 0;

	Cell() {}
	~Cell() {}

	Cell(bool A, int L)
	{
		maxAge = L;
		if (A) age = L;
	}

	void operator = (Cell rhs)
	{
		int maxAge = rhs.maxAge;
		int age = rhs.age;
		update();
	}

	bool alive() { return (age > 0); }

	float ageNormal() { return (age>0) ? (float(age) / maxAge) : 0; }

	vec3i rgb()
	{
		//std::cout << "pid: " << pid << std::endl;
		return palette[clamp(pid-1, 0, 6)];
	}

	void olden()
	{
		--age;
		age = clamp(age, 0, maxAge);
		//update(alive);
	}

	void set(bool s)
	{
		age = maxAge*s;
		update();
	}

	void update()
	{
			pid = (float)palette.size() * ageNormal();
	}

	void print()
	{
		std::cout << "\n    rgb() = " << rgb().toString(1) <<
			         "\n   maxAge = " << maxAge <<
			         "\n      age = " << age <<
			         "\nageNormal = " << ageNormal() <<
			         "\n    alive = " << alive() << std::endl;
	}
};
