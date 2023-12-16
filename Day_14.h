#pragma once

#include "Utils.h"
#include <tuple>
#include <algorithm>
#include <unordered_map>



void DoDay14() {
	auto lines = GetLines("input/day14.txt");

	std::vector<coord> cubes;
	std::vector<coord> rocks;


	for (size_t i = 0; i < lines.size(); i++) {
		const auto& line = lines[i];

		for (size_t charIndex = 0; charIndex < line.size(); charIndex++) {
			if (line[charIndex] == 'O') {
				coord rockCoord;
				rockCoord.x = charIndex;
				rockCoord.y = i;
				rocks.push_back(rockCoord);
			}
			else if (line[charIndex] == '#') {
				coord cubeCoord;
				cubeCoord.x = charIndex;
				cubeCoord.y = i;
				cubes.push_back(cubeCoord);
			}
		}
	}

	std::vector<std::string> newMap;
	newMap.resize(lines.size());

	for (auto& s : newMap) {
		s.append(lines[0].size(), '.');
	}

	for (size_t i = 0; i < cubes.size(); i++) {
		auto coord = cubes[i];
		newMap[coord.y][coord.x] = '#';
	}

	int numCycles = 1000000000;

	int result = 0;



	int width = lines[0].size();
	int height = lines.size();

	int lastCycleResult = 0;

	std::unordered_map<int, std::vector<int>> cycleResultsMap;

	std::vector<int> cycleResults;


	int sameCount = 0;

	int highestCycleLastFound = 0;

	auto start = GetCurrentTime();

	auto fillMapWithRocks = [](std::vector<std::string>& map, std::vector<coord>& rocks) {
		for (size_t i = 0; i < rocks.size(); i++) {
			auto coord = rocks[i];
			map[coord.y][coord.x] = 'O';
		}
	};

	std::vector<std::tuple<int, int>> directions{
	{0, -1},{-1, 0},{0, 1},{1,0}
	};
	std::vector<std::string> currentMap = newMap;
	fillMapWithRocks(currentMap, rocks);
	for (size_t cycle = 0; cycle < numCycles; cycle++) {
		int currentCycleResult = 0;

		for (size_t directionIndex = 0; directionIndex < directions.size(); directionIndex++) {
			int outerItStart, innerItStart;
			int outerItEnd, innerItEnd;
			int innerItDir, outerItDir;

			int xGrowth, yGrowth;

			xGrowth = std::get<0>(directions[directionIndex]);
			yGrowth = std::get<1>(directions[directionIndex]);

			bool xCoordIsOuter;

			switch (directionIndex) {
				case 0:
				{
					//top to bottom, left to right
					outerItStart = 0;
					innerItStart = 0;
					outerItEnd = height;
					innerItEnd = width;
					outerItDir = 1;
					innerItDir = 1;
					xCoordIsOuter = false;
				}
				break;
				case 1:
					//left to right, top to bottom
					outerItStart = 0;
					innerItStart = 0;
					outerItEnd = width;
					innerItEnd = height;
					outerItDir = 1;
					innerItDir = 1;
					xCoordIsOuter = true;
					break;
				case 2:
					//bottom to top, left to right
					outerItStart = height-1;
					innerItStart = 0;
					outerItEnd = -1;
					innerItEnd = width;
					outerItDir = -1;
					innerItDir = 1;
					xCoordIsOuter = false;
					break;
				case 3:
					//right to left, top to bottom
					outerItStart = width -1;
					innerItStart = 0;
					outerItEnd = -1;
					innerItEnd = height;
					outerItDir = -1;
					innerItDir = 1;
					xCoordIsOuter = true;
					break;
				default:
					throw;
			}

			for (int outer = outerItStart; outer != outerItEnd; outer += outerItDir) {
				for (int inner = innerItStart; inner != innerItEnd; inner += innerItDir) {
					coord current;
					if (xCoordIsOuter) {
						current.x = outer;
						current.y = inner;
					}
					else {
						current.x = inner;
						current.y = outer;
					}
					char c = currentMap[current.y][current.x];
					if (c != 'O') continue;

					coord endPosition = current;
					for (int iteration = 1;; iteration++) {
						coord newCoord;
						newCoord.x = current.x + (iteration * xGrowth);
						newCoord.y = current.y + (iteration * yGrowth);

						if (newCoord.x < 0 || newCoord.y < 0 || newCoord.x >= width || newCoord.y >= height) { //out of bounds
							break;
						}

						if (currentMap[newCoord.y][newCoord.x] == '.') {
							endPosition = newCoord;
							continue;
						}
						break;
					}

					currentMap[current.y][current.x] = '.';
					currentMap[endPosition.y][endPosition.x] = 'O';

					if (directionIndex == directions.size() - 1) {
						currentCycleResult += height - endPosition.y;
					}
				}
			}
		}

		if (sameCount > 100) {
			auto vec = cycleResultsMap[currentCycleResult];
			int cycleLastFound = *(vec.end() - 1);
			int cyclesOffset = cycle - cycleLastFound;

			if (cyclesOffset > highestCycleLastFound) {
				highestCycleLastFound = cyclesOffset;
			}
		}

		if (sameCount > 200) {
			std::cout << "cycle length" << highestCycleLastFound << std::endl;

			//find start position
			int loopStartIndex;
			for (int startIndex = 0; ; startIndex++) {
				bool works = true;
				for (size_t loopPos = 0; loopPos < highestCycleLastFound; loopPos++) {
					int loopIndexStart = startIndex + loopPos;
					int loopIndexRepeat = loopIndexStart + highestCycleLastFound;
					if (cycleResults[loopIndexStart] != cycleResults[loopIndexRepeat]) {
						works = false;
						break;
					}
				}

				if (works) {
					loopStartIndex = startIndex;
					break;
				}
			}
			std::cout << "Start index found: " << loopStartIndex << std::endl;

			//what is position at numCycles ?
			int pos = numCycles - 1;
			int loopIndexAtPos = (pos - loopStartIndex) % highestCycleLastFound;
			int calcResult = cycleResults[loopStartIndex + loopIndexAtPos];

			auto end = GetCurrentTime();

			auto res = GetSeconds(start, end);

			std::cout << "result: " << calcResult << " time: " << res << "s" << std::endl;
			return;
		}

		if (cycleResultsMap.find(currentCycleResult) == cycleResultsMap.end()) {
			cycleResultsMap[currentCycleResult].push_back(cycle);
			sameCount = 0;
		}
		else {
			sameCount++;
			cycleResultsMap[currentCycleResult].push_back(cycle);
		}

		cycleResults.push_back(currentCycleResult);

		//else {
		//	auto& curr = cycleResults[currentCycleResult];
		//	if(std::find(curr.begin(), curr.end(), cycle) == curr.end())
		//	curr.push_back(cycle);
		//	uniqueResults++;
		//}

		//std::cout << "CYCLE: " << cycle << " res: " << currentCycleResult << " dbg: " << (numCycles % cycle) <<std::endl;
		//PrintMap(currentMap);

		//if (currentCycleResult == lastCycleResult) {
		//	std::cout << "cycle result same at " << cycle << ": " << currentCycleResult << std::endl;
		//	result = currentCycleResult;
		//}
		//lastCycleResult = currentCycleResult;
	}



	//for (int lineIndex = 0; lineIndex < newMap.size(); lineIndex++) {
	//	auto& s = newMap[lineIndex];
	//	std::cout << s << std::endl;

	//	//int r = 0;
	//	//for (size_t i = 0; i < s.size(); i++) {
	//	//	if(s[i] == 'O') r+= newMap.size() - lineIndex;
	//	//}

	//	//std::cout << " " << r << std::endl;
	//	//result += r;
	//}

	std::cout << result << std::endl;
}