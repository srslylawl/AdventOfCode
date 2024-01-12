#pragma once
#include "Utils.h"
#include <cassert>

struct hail {
	long long posX, posY, posZ;
	long long velX, velY, velZ;

	hail() : posX(0), posY(0), posZ(0), velX(0), velY(0), velZ(0) {}

	long long& operator[](int i) {
		switch (i) {
		case 0:
			return posX;
		case 1:
			return posY;
		case 2:
			return posZ;
		case 3:
			return velX;
		case 4:
			return velY;
		case 5:
			return velZ;
		}

		throw std::exception("out of bounds");
	}
};


void DoDay24() {
	auto lines = GetLines("input/day24.txt");

	std::vector<hail> hails(lines.size());

	const long long minSpace = 200000000000000;
	const long long maxSpace = 400000000000000;

	for (size_t i = 0; i < lines.size(); i++)
	{
		hail hail;
		auto nums = GetLongSignedNumbersFromString(lines[i]);

		assert(nums.size() == 6);
		for (size_t n = 0; n < 6; n++)
		{
			hail[n] = nums[n];
		}

		hails[i] = hail;
	}

	//convert hails to lines

	hail& h = hails[0];
	hail& h2 = hails[1];

	float x1 = h.posX;
	float y1 = h.posY;

	float x2 = h.posX + h.velX;
	float y2 = h.posY + h.velY;

	float x3 = h2.posX;
	float y3 = h2.posY;

	float x4 = h2.posX + h2.velX;
	float y4 = h2.posY + h2.velY;

	float px = 
		((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) 
		/ ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

	float py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4))
		/ ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));

	std::cout << "PX: " << px << " PY:" << py << std::endl;


}