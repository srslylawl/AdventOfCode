#pragma once
#include "Utils.h"
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>



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
		char& cur = mapCopy[pos.coord.y][pos.coord.x];
		if (cur == 'S' || cur == '.') {
			cur = '1';
		}
		else {
			cur++;
		}

		cache[pos] = 1;

		return 1;
	}



	int result = 0;
	for (const auto dir : directions) {
		coord current(pos.coord.x + dir.x, pos.coord.y + dir.y);

		if (current.y < 0 || current.y > lines.size() - 1 || current.x < 0 || current.x > lines[current.y].size() - 1) {
			//out of bounds
			continue;
		}

		if (lines[current.y][current.x] == '.' || lines[current.y][current.x] == 'S') {
			position next;
			next.coord = current;
			next.stepsRemaining = pos.stepsRemaining - 1;
			result += GetNumEndPositions(next, lines, cache, mapCopy);
		}
	}

	cache[pos] = result;

	return result;
}



void DoDay21() {
	auto lines = GetLines("input/day21.txt");

	coord startPoint;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t c = 0; c < lines[i].size(); c++) {
			if (lines[i][c] == 'S') {
				startPoint.y = i;
				startPoint.x = c;
				break;
			}
		}
	}

	//basically floodfill
	int maxSteps = 64;

	position start;
	start.coord = startPoint;
	start.stepsRemaining = maxSteps;
	std::queue<position> positions;
	positions.push(start);

	std::unordered_set<coord> endPoints;

	std::unordered_map<position, int> cache;

	std::vector<std::string> mapCopy = lines;


	int result = GetNumEndPositions(start, lines, cache, mapCopy);


	//while (!positions.empty()) {
	//	auto current = positions.front();
	//	positions.pop();

	//	char& cur = mapCopy[current.coord.y][current.coord.x];


	//	if (current.stepsRemaining == 0) {
	//		if (cur == 'S' || cur == '.') {
	//			cur = '1';
	//		}
	//		else {
	//			cur++;
	//		}
	//		endPoints.insert(current.coord);
	//		//PrintMap(mapCopy);
	//		continue;
	//	}

	//	//PrintMap(mapCopy);
	//	//std::cout << std::endl;
	//	auto nextPositions = GetSurrounding(current.coord, lines);

	//	for (auto p : nextPositions) {
	//		position n;
	//		n.coord = p;
	//		n.stepsRemaining = current.stepsRemaining - 1;
	//		positions.push(n);
	//	}
	//}

	PrintMap(mapCopy);
	std::cout << result << std::endl;

}