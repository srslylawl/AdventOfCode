#pragma once

#include "Utils.h"
#include <algorithm>

struct cube {
	int minX, minY, minZ;
	int maxX, maxY, maxZ;

	bool intersects(const cube& other) {
		//TODO: implement
		throw;
	}
};



void DoDay22() {
	auto lines = GetLines("input/day22.txt");
	std::vector<cube> cubes;

	for (auto& str : lines) {
		cube cube;

		int attrIndex = 0;
		int startIndex = 0;

		for (int cIndex = 0; cIndex < str.size(); cIndex++) {
			char c = str[cIndex];

			bool atEnd = cIndex == str.size() - 1;
			if (c == ',' || c == '~' || atEnd) {
				int endIndex = cIndex - 1;
				if (atEnd) endIndex = cIndex;

				int num = ParseDigitFromString(str.substr(startIndex, endIndex - startIndex + 1));

				switch (attrIndex++)
				{
				case 0:
					cube.minX = num;
					break;
				case 1:
					cube.minY = num;
					break;
				case 2:
					cube.minZ = num;
					break;
				case 3:
					cube.maxX = num;
					break;
				case 4:
					cube.maxY = num;
					break;
				case 5:
					cube.maxZ = num;
					break;
				default:
					throw;
				}
				startIndex = cIndex + 1;
			}
		}

		cubes.push_back(cube);
	}

	//sort by Y min, lowest is at beginning of list
	std::sort(cubes.begin(), cubes.end(), [](const cube& first, const cube& second) {
		return first.minY < second.minY;
		});

	//fall down

	//for each cube, check if intersects with cubes below, if yes, set to point one before intersection

	//check which cubes are directly resting on other cubes, if noone is resting on it, add 1 to result, if there is, check if its the only one they are resting on, if not, add 1

}