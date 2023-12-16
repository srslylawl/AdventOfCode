#pragma once

#include "Utils.h"
#include <unordered_set>

struct beam {
	coord position;
	coord direction;

	beam(coord pos, coord dir) : position(pos), direction(dir) {
	}

	bool operator==(const beam& other) const {
		return position == other.position && direction == other.direction;
	}
};

namespace std {
	template<>
	struct hash<beam> {
		std::size_t operator()(const beam& b) const {
			std::size_t res = 17;
			res = res * 31 + hash<coord>()(b.position);
			res = res * 31 + hash<coord>()(b.direction);
			return res;
		}

	};
}


std::vector<beam> beams{ beam({-1,0}, {1,0}) };

std::unordered_set<coord> energizedTiles{};

std::unordered_set<beam> exploredBeams;


bool HandleBeam(beam& b, std::vector<std::string>& lines) {
	coord nextPos = b.position;
	nextPos.x += b.direction.x;
	nextPos.y += b.direction.y;

	if (exploredBeams.find(b) != exploredBeams.end()) {
		return false;
	}

	exploredBeams.insert(b);

	//check bounds

	if (nextPos.x < 0 || nextPos.y < 0 || nextPos.y >= lines.size() || nextPos.x >= lines[nextPos.y].size()) {
		return false;
	}

	energizedTiles.insert(nextPos);
	b.position = nextPos;
	char nextSymbol = lines[nextPos.y][nextPos.x];

	switch (nextSymbol) {
		case '/':
		{
			b.direction = { -b.direction.y, -b.direction.x };
		}
		break;
		case '\\':
		{
			b.direction = { b.direction.y, b.direction.x };
		}
		break;
		case '|':
		{
			if (b.direction.x != 0) {
				b.direction = { 0, -1 }; // go up
				beam newBeam(b.position, { 0, 1 });
				beams.push_back(newBeam);
			}
		}
		break;
		case '-':
		{
			if (b.direction.y != 0) {
				b.direction = { -1, 0 };
				beam newBeam(b.position, { 1, 0 });
				beams.push_back(newBeam);
			}
		}
		break;
		default:
			break;
	}


	return true;
}

int GetResult(beam startBeam, std::vector<std::string>& lines) {
	energizedTiles.clear();
	exploredBeams.clear();
	beams.clear();

	beams.push_back(startBeam);
	while (beams.size() > 0) {
		if (!HandleBeam(beams[0], lines)) {
			beams.erase(beams.begin());
		}
	}

	return energizedTiles.size();
}

void DoDay16() {
	auto lines = GetLines("input/day16.txt");


	int highestResult = 0;

	std::vector<int> directions = { -1, 1 };

	for (size_t y = 0; y < lines.size(); y++) {
		for (auto dir : directions) {

			int xCoord = dir == 1 ? -1 : lines[y].size();
			coord startCoord(xCoord, y);
			coord direction(dir, 0);
			beam start(startCoord, direction);
			int res = GetResult(start, lines);
			//std::cout << "Result x" << startCoord.x << " y" << startCoord.y << " :" << res << std::endl;
			if (res > highestResult)
				highestResult = res;
		}
	}

	for (size_t x = 0; x < lines[0].size(); x++) {
		for (auto dir : directions) {

			int yCoord = dir == 1 ? -1 : lines.size();
			coord startCoord(x, yCoord);
			coord direction(0, dir);
			beam start(startCoord, direction);
			int res = GetResult(start, lines);
			//std::cout << "Result x" << startCoord.x << " y" << startCoord.y << " :" << res << std::endl;
			if (res > highestResult)
				highestResult = res;
		}
	}

	std::cout << "result: " << highestResult << std::endl;
}