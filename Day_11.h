#pragma once
#include "Utils.h"
#include <vector>


void DoDay11() {
	auto lines = GetLines("input/day11.txt");

	std::vector<int> expandRows;
	std::vector<int> expandColumns;

	int width = lines[0].size();

	int rowIndex = 0;
	for (auto it = lines.begin(); it != lines.end();) {
		bool hasGalaxy = false;
		for (int i = 0; i < it->size(); i++) {
			char curr = (*it)[i];
			if (curr == '#') {
				hasGalaxy = true;
				break;
			}
		}

		if (!hasGalaxy) {
			expandRows.push_back(rowIndex);
			std::cout << "empy row: " << rowIndex << std::endl;
		}

		rowIndex++;
		it++;
	}

	for (int x = 0; x < width; x++) {
		bool columnHasGalaxy = false;
		for (int y = 0; y < lines.size(); y++) {
			char c = lines[y][x];
			if (c == '#') {
				columnHasGalaxy = true;
				break;
			}
		}
		if (!columnHasGalaxy) {
			expandColumns.push_back(x);
			std::cout << "empy column: " << x << std::endl;
		}
	}

	std::vector<std::string> galaxyMap = lines;

	//find galaxies
	std::vector<coord> galaxyPositions;
	for (int row = 0; row < galaxyMap.size(); row++) {
		for (int cIndex = 0; cIndex < galaxyMap[row].size(); cIndex++) {
			char c = galaxyMap[row][cIndex];
			if (c == '#') {
				coord coordinate;
				coordinate.y = row;
				coordinate.x = cIndex;
				galaxyMap[row][cIndex] = '0' + (int)galaxyPositions.size() + 1;
				galaxyPositions.push_back(coordinate);
			}
		}
		std::cout << galaxyMap[row] << std::endl;
	}
	long long ExpansionSize = 1000000;
	long long sum = 0;
	for (size_t i = 0; i < galaxyPositions.size()-1; i++) {
		for (size_t j = i + 1; j < galaxyPositions.size(); j++) {
			if (i == j) continue;
			coord iPos = galaxyPositions[i];
			coord jPos = galaxyPositions[j];

			int xOffset = iPos.x - jPos.x;
			int xDirection = xOffset < 0 ? 1 : -1;
			long long xDistance = abs(xOffset);
			for (int x = iPos.x; x != jPos.x; x += xDirection) {
				if (std::find(expandColumns.begin(), expandColumns.end(), x) != expandColumns.end()) {
					//std::cout << 
					xDistance += ExpansionSize-1;
				}
			}

			int yOffset = iPos.y - jPos.y;
			int yDirection = yOffset < 0 ? 1 : -1;
			long long yDistance = abs(yOffset);
			for (int y = iPos.y; y != jPos.y; y += yDirection) {
				if (std::find(expandRows.begin(), expandRows.end(), y) != expandRows.end()) {
					yDistance += ExpansionSize-1;
				}
			}

			long long distance = xDistance + yDistance;
			sum += distance;
			//std::cout << "Dist between " << i + 1 << " and " << j + 1 << " is " << distance << std::endl;
		}

		//sum += smallestDistance;
	}

	std::cout << sum << std::endl;
}