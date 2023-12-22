#pragma once
#include "Utils.h"
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>

int loopCoordinate(int input, int mod) {
	int res = abs(input) % mod;

	if (input < 0) {
		res = (mod - (mod + res) % mod) % mod;
	}

	return res;
}

const static std::vector<coord> directions = {
		{0, 1}, {0, -1}, {1, 0}, {-1, 0}
};

std::vector<coord> GetSurrounding(coord position, std::vector<std::string>& lines) {
	std::vector<coord> results;

	for (const auto dir : directions) {
		coord current(position.x + dir.x, position.y + dir.y);

		if (current.y < 0 || current.y > lines.size() - 1 || current.x < 0 || current.x > lines[current.y].size() - 1) {
			//out of bounds
			continue;
		}

		if (lines[current.y][current.x] == '.' || lines[current.y][current.x] == 'S') {
			results.push_back(current);
		}
	}

	return results;
}

struct position {
	int stepsRemaining;
	coord coord;

	bool operator==(const position& other) const {
		return stepsRemaining == other.stepsRemaining && coord == other.coord;
	}

};

namespace std {
	template<>
	struct hash<position> {
		std::size_t operator()(const position& c) const {
			std::size_t res = 17;
			res = res * 31 + hash<coord>()(c.coord);
			res = res * 31 + hash<int>()(c.stepsRemaining);
			return res;
		}
	};
}

int GetNumEndPositions(position pos, std::vector<std::string>& lines, std::unordered_map<position, int>& cache, std::vector<std::string>& mapCopy) {
	if (cache.count(pos)) {
		return 0;
	}

	if (pos.stepsRemaining == 0) {
		//char& cur = mapCopy[pos.coord.y][pos.coord.x];
		//if (cur == 'S' || cur == '.') {
		//	cur = '1';
		//}
		//else {
		//	cur++;
		//}

		cache[pos] = 1;

		return 1;
	}



	int result = 0;
	for (const auto dir : directions) {
		coord current(pos.coord.x + dir.x, pos.coord.y + dir.y);

		//if (current.y < 0 || current.y > lines.size() - 1 || current.x < 0 || current.x > lines[current.y].size() - 1) {
		//	//out of bounds
		//	continue;
		//}

		int loopedY = loopCoordinate(current.y, lines.size());
		int loopedX = loopCoordinate(current.x, lines[loopedY].size());
		if (lines[loopedY][loopedX] == '.' || lines[loopedY][loopedX] == 'S') {
			position next;
			next.coord = current;
			next.stepsRemaining = pos.stepsRemaining - 1;
			result += GetNumEndPositions(next, lines, cache, mapCopy);
		}
	}

	cache[pos] = result;

	return result;
}

bool drawMap = false;
std::vector<std::string> mapCopy;

int GetStepsNeededToFill(std::vector<coord> startPositions, std::vector<std::string>& lines, int& reachableGardens, int maxSteps) {
	std::queue<coord> current;
	for (auto sp : startPositions) {
		current.push(sp);
	}
	std::queue<coord> next;

	std::unordered_set<coord> cache;

	int steps = 0;


	int reachable = 0;
	while ((!current.empty() || !next.empty())) {
		if (current.empty()) {
			current = next;
			next = {};

			steps++;
			if (steps > maxSteps) break;
		}

		auto p = current.front();
		current.pop();

		if (cache.count(p)) continue;
		cache.insert(p);
		int remainingSteps = maxSteps - steps;
		bool even = remainingSteps % 2 == 0;
		if (even) {
			if (drawMap) {
				if (mapCopy[p.y][p.x] != 'S') {
					mapCopy[p.y][p.x] = 'X';
				}
			}
			reachable++;
		}

		for (const auto dir : directions) {
			coord current(p.x + dir.x, p.y + dir.y);

			if (current.y < 0 || current.y > lines.size() - 1 || current.x < 0 || current.x > lines[current.y].size() - 1) {
				//out of bounds
				continue;
			}

			if (lines[current.y][current.x] == '.' || lines[current.y][current.x] == 'S') {
				next.push(current);
			}
		}
	}

	reachableGardens = reachable;

	return steps;
}

void DoDay21() {
	auto lines = GetLines("input/day21.txt");

	mapCopy = lines;
	std::unordered_set<coord> gardens;
	coord startPoint;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t c = 0; c < lines[i].size(); c++) {
			if (lines[i][c] == 'S') {
				startPoint.y = i;
				startPoint.x = c;
			}

			if (lines[i][c] == '.' || lines[i][c] == 'S') {
				coord g(c, i);
				gardens.insert(g);
			}
		}
	}

	const long long maxSteps = 26501365;

	const int mapSize = lines.size();

	const long long totalMapsPerAxis = maxSteps / mapSize;
	const long long fullSquaresPerDir = totalMapsPerAxis - 1;


	long long endAmount = 0;
	//get how many steps it takes to visit every garden of one map
	std::vector<coord> startPositionsStart = {
		startPoint
	};
	std::vector<coord> startPositionsNext;
	for (auto dir : directions) {
		coord n;
		n.x = dir.x + startPoint.x;
		n.y = dir.y + startPoint.y;
		startPositionsNext.push_back(n);
	}

	int evenCount, oddCount; //not 100% sure here

#define print PrintMap(mapCopy); mapCopy = lines; std::cout << std::endl;

//#define print 

	drawMap = true;
	GetStepsNeededToFill(startPositionsStart, lines, oddCount, maxSteps);
	print
		GetStepsNeededToFill(startPositionsNext, lines, evenCount, maxSteps);
	print
		//

		long long amountOfOddMaps, amountofEvenMaps;

	amountOfOddMaps = ((fullSquaresPerDir / 2) * 2) + 1;
	amountofEvenMaps = (totalMapsPerAxis + 1) / 2 * 2;

	amountofEvenMaps = pow(amountofEvenMaps, 2);
	amountOfOddMaps = pow(amountOfOddMaps, 2);

	long long evenAdditon = evenCount * amountofEvenMaps;
	long long oddAddition = oddCount * amountOfOddMaps;
	endAmount += evenAdditon;
	endAmount += oddAddition;

	//these 2 are confirmed by reddit

	int topCorner, rightCorner, bottomCorner, leftCorner;
	std::vector<coord> topPosStart{ {65, mapSize - 1} };
	std::vector<coord> rightPosStart{ {0, 65} };
	std::vector<coord> botPosStart{ {65, 0} };
	std::vector<coord> leftPosStart{ {mapSize - 1, 65} };


	//mapsize -1 steps as we are at 131 steps before we step into the map
	GetStepsNeededToFill(topPosStart, lines, topCorner, mapSize - 1);
	print
		GetStepsNeededToFill(rightPosStart, lines, rightCorner, mapSize - 1);
	print
		GetStepsNeededToFill(botPosStart, lines, bottomCorner, mapSize - 1);
	print
		GetStepsNeededToFill(leftPosStart, lines, leftCorner, mapSize - 1);
	print

	long long corners = topCorner + rightCorner + bottomCorner + leftCorner;

	endAmount += topCorner;
	endAmount += rightCorner;
	endAmount += bottomCorner;
	endAmount += leftCorner;

	int topRightCorner, topLeftCorner, bottomRightCorner, bottomLeftCorner;
	std::vector<coord> topRightPosStart{ {0, mapSize - 1} };
	std::vector<coord> topLeftPosStart{ {mapSize - 1, mapSize - 1} };
	std::vector<coord> botRightPosStart{ {0, 0} };
	std::vector<coord> botLeftPosStart{ {mapSize - 1, 0} };


	//SMALL CORNERS
	int stepsRemaining = 64;
	GetStepsNeededToFill(topRightPosStart, lines, topRightCorner, stepsRemaining);
	print
		GetStepsNeededToFill(topLeftPosStart, lines, topLeftCorner, stepsRemaining);
	print
		GetStepsNeededToFill(botRightPosStart, lines, bottomRightCorner, stepsRemaining);
	print
		GetStepsNeededToFill(botLeftPosStart, lines, bottomLeftCorner, stepsRemaining);
	print

	long long smallCorners = topRightCorner + topLeftCorner + bottomLeftCorner + bottomRightCorner;

	endAmount += topRightCorner * totalMapsPerAxis;
	endAmount += topLeftCorner * totalMapsPerAxis;
	endAmount += bottomLeftCorner * totalMapsPerAxis;
	endAmount += bottomRightCorner * totalMapsPerAxis;


	//BIG CORNERS
	int bigtopRightCorner, bigtopLeftCorner, bigbottomRightCorner, bigbottomLeftCorner;
	std::vector<coord> bigTopRightPosStart{ {0, mapSize - 1} };
	std::vector<coord> bigTopLeftPosStart{ {mapSize - 1, mapSize - 1} };
	std::vector<coord> bigBotRightPosStart{ {0, 0} };
	std::vector<coord> bigBotLeftPosStart{ {mapSize - 1, 0} };
	//drawMap = true;

	int steps = 131 + 130 - 66;
	GetStepsNeededToFill(bigTopRightPosStart, lines, bigtopRightCorner, steps);
	print
		GetStepsNeededToFill(bigTopLeftPosStart, lines, bigtopLeftCorner, steps);
	print
		GetStepsNeededToFill(bigBotRightPosStart, lines, bigbottomRightCorner, steps);
	print
		GetStepsNeededToFill(bigBotLeftPosStart, lines, bigbottomLeftCorner, steps);
	print

	long long bigCornerAmt = totalMapsPerAxis - 1;
	long long bigCorners = bigtopLeftCorner + bigtopRightCorner + bigbottomLeftCorner + bigbottomRightCorner;


	endAmount += bigtopRightCorner * bigCornerAmt;
	endAmount += bigtopLeftCorner * bigCornerAmt;
	endAmount += bigbottomLeftCorner * bigCornerAmt;
	endAmount += bigbottomRightCorner * bigCornerAmt;



	//PrintMap(lines);

	std::cout << endAmount << std::endl;

	const long long ANSWER = 632257949158206;

	if(endAmount != ANSWER) {
		std::cout << "WRONG ANSWER! - off by: " << (abs(ANSWER-endAmount)) << std::endl;
	}
	//int amountOfOddMaps = pow(totalMapsPerDirection+1, 2);
	//int amountOfEvenMaps = pow(a)
}