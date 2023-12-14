#pragma once

#include "Utils.h"
#include <tuple>
#include <algorithm>
#include <unordered_map>

struct coord {
	int x, y;

	bool operator==(const coord& other) {
		return x == other.x && y == other.y;
	}
};

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

	std::vector<std::tuple<int, int>> directions{
		{0, -1},{-1, 0},{0, 1},{1,0}
	};

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
	std::vector<std::string> currentMap = newMap;
	fillMapWithRocks(currentMap, rocks);
	for (size_t cycle = 0; cycle < numCycles; cycle++) {
		int currentCycleResult = 0;

		for (size_t directionIndex = 0; directionIndex < directions.size(); directionIndex++) {

			std::sort(rocks.begin(), rocks.end(), [directionIndex](const coord& a, const coord& b) {
				switch (directionIndex) {
					case 0:
						return a.y < b.y;
					case 1:
						return a.x < b.x;
					case 2:
						return a.y > b.y;
					case 3:
						return a.x > b.x;
				}

				return false;
			});
			for (size_t i = 0; i < rocks.size(); i++) {
				auto& rockCoord = rocks[i];
				const auto& direction = directions[directionIndex];

				coord resCoord;
				resCoord.x = rockCoord.x;
				resCoord.y = rockCoord.y;

				for (int iteration = 1;; iteration++) {
					coord newCoord;
					newCoord.x = rockCoord.x + (iteration * std::get<0>(direction));
					newCoord.y = rockCoord.y + (iteration * std::get<1>(direction));

					if (newCoord.x < 0 || newCoord.y < 0 || newCoord.x >= width || newCoord.y >= height) { //out of bounds
						break;
					}

					if (currentMap[newCoord.y][newCoord.x] == '.') {
						resCoord = newCoord;
						continue;
					}

					break;
				}
				currentMap[rockCoord.y][rockCoord.x] = '.';
				rockCoord.x = resCoord.x;
				rockCoord.y = resCoord.y;
				currentMap[rockCoord.y][rockCoord.x] = 'O';

				if (directionIndex == directions.size() - 1) {
					currentCycleResult += height - rockCoord.y;
				}
			}
			//std::cout << "Dir" << directionIndex << std::endl;
			//PrintMap(currentMap);
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