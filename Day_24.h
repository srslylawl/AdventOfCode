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

std::ostream& operator<<(std::ostream& os, const hail& h)
{
	os << h.posX << ", " << h.posY << ", " << h.posZ << " @ " << h.velX << ", " << h.velY << ", " << h.velZ;
	return os;
}

struct line {
	long long posX1, posX2;
	long long posY1, posY2;
};

std::pair<double, double> lineIntersection(const line& a, const line& b, bool& isParallel) {
	double x1 = a.posX1;
	double x2 = a.posX2;
	double x3 = b.posX1;
	double x4 = b.posX2;

	double y1 = a.posY1;
	double y2 = a.posY2;
	double y3 = b.posY1;
	double y4 = b.posY2;

	double px_numerator = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);

	double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	double px = px_numerator / denominator;

	double py_numerator = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4));

	double py = py_numerator / denominator;

	//std::cout << " px_numerator: " << px_numerator << ", px_denominator:" << px_denominator << std::endl;
	//std::cout << " py_numerator: " << py_numerator << ", py_denominator:" << py_denominator << std::endl;

	isParallel = denominator == 0;

	return std::make_pair(px, py);
}


bool isInPast(double pos, double velocity, double point) {
	if (velocity > 0) {
		return point < pos;
	}

	return point > pos;
}


void DoDay24() {
	auto lines = GetLines("input/day24.txt");

	std::vector<hail> hails(lines.size());
	std::vector<line> hailLines(lines.size());

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

		line l;
		l.posX1 = hail.posX;
		l.posX2 = hail.posX + hail.velX;
		l.posY1 = hail.posY;
		l.posY2 = hail.posY + hail.velY;
		hailLines[i] = l;
	}

	//convert hails to lines

	int result = 0;

	//int current = 0;
	//int total = (lines.size() * (lines.size()-1)) / 2;
	for (size_t a = 0; a < lines.size() - 1; a++)
	{
		for (size_t b = a + 1; b < lines.size(); b++)
		{
			//std::cout << "A: " << hails[a] << std::endl;
			//std::cout << "B: " << hails[b] << std::endl;

			bool isParallel;
			auto [intersectionX, intersectionY] = lineIntersection(hailLines[a], hailLines[b], isParallel);
			if (isParallel) {
				//std::cout << "Lines are parallel.\n" << std::endl;
				continue;
			}

			bool aInFuture = intersectionX > hailLines[a].posX1 == hailLines[a].posX2 > hailLines[a].posX1;
			//bool a_y_inpast = isInPast(hails[a].posY, hails[a].velY, intersectionY);
			bool aInPast = !aInFuture;

			bool bInFuture = intersectionX > hailLines[b].posX1 == hailLines[b].posX2 > hailLines[b].posX1;
			//bool b_y_inpast = isInPast(hails[b].posY, hails[b].velY, intersectionY);
			bool bInpast = !bInFuture;

			bool withinTestArea = intersectionX >= minSpace && intersectionX <= maxSpace && intersectionY >= minSpace && intersectionY <= maxSpace;

			if (a == 6 && b == 7) {
				std::cout << "Collision: " << intersectionX << "," << intersectionY << std::endl;
				std::cout << "aInFuture " << aInFuture << std::endl;
				std::cout << "bInFuture " << bInFuture << std::endl;
				std::cout << "withinTest " << withinTestArea << std::endl;
			}
			if (!isParallel && aInFuture && bInFuture && withinTestArea) {
				std::cout << a << "," << b << std::endl;
				std::cout << "\n" << std::endl;
				result++;
				//continue;
			}

			//if (aInPast && bInpast) {
			//	//std::cout << "Collision in past for A and B.\n" << std::endl;
			//	continue;
			//}

			//if (aInPast) {
			//	//std::cout << "Collision in past for A.\n" << std::endl;
			//	continue;
			//}

			//if (bInpast) {
			//	//std::cout << "Collision in past for B.\n" << std::endl;
			//	continue;
			//}
			//if (!withinTestArea) {
			//	//std::cout << "Collision outside test area.\n" << std::endl;
			//	continue;
			//}

			////std::cout << "Cross at: x=" << intersectionX << ", y=" << intersectionY << "\n" << std::endl;
			//result++;
		}
	}

	//correct result would be 27732

	std::cout << "Result: " << result << std::endl;
}