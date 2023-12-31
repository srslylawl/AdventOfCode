#pragma once

#include "Utils.h"
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <unordered_set>

struct cube {
	int minX, minY, minZ;
	int maxX, maxY, maxZ;

	bool intersects(const cube& other) {
		bool yOverlap = (other.minY >= minY && other.minY <= maxY) || (other.maxY >= minY && other.maxY <= maxY) || (other.minY <= minY && other.maxY >= maxY);
		bool xOverlap = (other.minX >= minX && other.minX <= maxX) || (other.maxX >= minX && other.maxX <= maxX) || (other.minX <= minX && other.maxX >= maxX);
		bool zOverlap = (other.minZ >= minZ && other.minZ <= maxZ) || (other.maxZ >= minZ && other.maxZ <= maxZ) || (other.minZ <= minZ && other.maxZ >= maxZ);

		return yOverlap && xOverlap && zOverlap;
	}

	void MoveDown(int amt) {
		minZ -= amt;
		maxZ -= amt;
	}

	bool operator==(const cube& o) const {
		return minZ == o.minZ && maxZ == o.maxZ && minX == o.minX && minY == o.minY && maxX == o.maxX && maxY == o.maxY;
	}

	cube() : minX(0), minY(0), minZ(0), maxX(0), maxY(0), maxZ(0) {
	}
};

void AssertNoCubesOverlap(std::vector<cube>& cubes) {
	for (size_t i = 0; i < cubes.size(); i++) {
		for (size_t ii = i + 1; ii < cubes.size() - 1; ii++) {
			assert(!cubes[i].intersects(cubes[ii]));
		}
	}

}


void PrintCubes(std::vector<cube>& cubes, int width, int height, bool xAxis) {
	std::vector<std::string> map;
	map.resize(height + 1);

	for (size_t i = 0; i < height + 1; i++) {
		map[i].append((width + 1) * 6, ' ');
	}


	int cubeIndex = 0;
	for (auto& cube : cubes) {

		int widthMin = xAxis ? cube.minX : cube.minY;
		int widthMax = xAxis ? cube.maxX : cube.maxY;
		std::string indexToString = std::to_string(cubeIndex);
		for (int z = cube.minZ; z <= cube.maxZ; z++) {
			for (int x = widthMin * 6; x <= widthMax * 6; x += 6) {
				map[height + 1 - z][x] = '[';
				map[height + 1 - z][x + 1] = '0';
				map[height + 1 - z][x + 2] = '0';
				map[height + 1 - z][x + 3] = '0';
				map[height + 1 - z][x + 4] = '0';
				map[height + 1 - z][x + 5] = ']';
				for (size_t i = 0; i < indexToString.size(); i++) {
					map[height + 1 - z][x + 4 - i] = indexToString[indexToString.size() - 1 - i];
				}
			}
		}
		cubeIndex++;
	}

	PrintMap(map);
	std::cout << std::endl;
}



void DoDay22() {
	cube a;
	a.minX = 0;
	a.maxX = 10;
	a.minY = 0;
	a.maxY = 10;

	cube b;
	b.minX = 5;
	b.maxX = 5;
	b.minY = 5;
	b.maxY = 5;

	assert(a.intersects(b));
	assert(b.intersects(a));

	b.minZ = 1;
	b.maxZ = 1;

	assert(!b.intersects(a));
	assert(!a.intersects(b));

	cube c = b;

	assert(b.intersects(c));
	assert(c.intersects(b));



	auto lines = GetLines("input/day22.txt");
	std::vector<cube> cubes;

	int highestX = 0;
	int highestZ = 0;
	int highestY = 0;

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

				switch (attrIndex++) {
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
						highestX = std::max(highestX, num);
						break;
					case 4:
						cube.maxY = num;
						highestY = std::max(highestY, num);
						break;
					case 5:
						cube.maxZ = num;
						highestZ = std::max(highestZ, num);
						break;
					default:
						throw;
				}
				startIndex = cIndex + 1;
			}
		}

		cubes.push_back(cube);
	}

	//PrintCubes(cubes, highestX, highestZ, true);
	//std::cout << "^ cubes originally, X axis is width" << std::endl;
	//PrintCubes(cubes, highestY, highestZ, false);
	//std::cout << "^ cubes originally, Y axis is width" << std::endl;

	AssertNoCubesOverlap(cubes);


	//sort by Z min, lowest is at beginning of list
	//std::sort(cubes.begin(), cubes.end(), [](const cube& first, const cube& second) {
	//	return first.minZ < second.minZ;
	//	});

	//highestZ = 0;

	//fall down -- might be an issue with some blocks being sorted that are at same y pos but different orientation, might have to check more
	bool somethingMoved = false;
	do {
		somethingMoved = false;
		for (int i = 0; i < cubes.size(); i++) {
			cube& c = cubes[i];
			if (c.minZ == 1) continue; //cant go below Z 1
			cube lastViablePos = c;
			cube movedDown = c;
			bool collided = false;
			while (!collided) {

				movedDown.MoveDown(1);
				if (movedDown.minZ == 0) {
					break;
				}
				for (int belowIndex = cubes.size() - 1; belowIndex >= 0; belowIndex--) {
					if (belowIndex == i) continue;
					const cube& belowCube = cubes[belowIndex];
					if (movedDown.intersects(belowCube)) {
						collided = true;
						break;
					}
				}

				if (!collided) {
					somethingMoved = true;
					lastViablePos = movedDown;
				}
			}
			c = lastViablePos;
			highestZ = std::max(c.maxZ, highestZ);
			//PrintCubes(cubes, highestX, highestZ);
		}
	} while (somethingMoved);


	int cubeScore = 0;
	for (auto c : cubes) {
		cubeScore += c.minZ + c.maxZ;
	}
	std::cout << cubeScore << " CUBESCORE" << std::endl;

	AssertNoCubesOverlap(cubes);


	/*std::sort(cubes.begin(), cubes.end(), [](const cube& first, const cube& second) {
		return first.minZ < second.minZ;
		});*/

	std::unordered_map<int, std::vector<int>> cubesToCubesAbove; //which cubes depend on these?
	std::unordered_map<int, std::vector<int>> cubesToCubesBelow; //which cubes does this cube depend on?

	//PrintCubes(cubes, highestX, highestZ, true);
	//std::cout << "^ cubes on ground" << std::endl;

	//PrintCubes(cubes, highestY, highestZ, false);
	//std::cout << "^ cubes on ground, Y axis" << std::endl;


	int result = 0;
	int sum = 0;
	//for (int cubeIndex = cubes.size() - 1; cubeIndex >= 0; cubeIndex--) {
	//	auto cubesCopy = cubes;
	//	cubesCopy.erase(cubesCopy.begin() + cubeIndex);
	//	//if this cube would be removed, would any other fall?
	//	std::unordered_set<int> cubesThatFall;
	//	while (true) {
	//		bool hasMoved = false;
	//		for (int i = 0; i < cubesCopy.size(); i++) {
	//			cube& c = cubesCopy[i];
	//			if (c.minZ == 1) continue; //cant go below Z 1
	//			cube movedDown = c;
	//			bool collided = false;
	//			movedDown.MoveDown(1);
	//			for (int belowIndex = cubesCopy.size() - 1; belowIndex >= 0; belowIndex--) {
	//				if (belowIndex == i) continue;
	//				const cube& belowCube = cubesCopy[belowIndex];
	//				if (movedDown.intersects(belowCube)) {
	//					collided = true;
	//					break;
	//				}
	//			}
	//			if (!collided) {
	//				cubesThatFall.insert(i);
	//				c = movedDown;
	//				hasMoved = true;
	//			}
	//		}

	//		if (!hasMoved) break;
	//	}

	//	std::cout << "removing " << cubeIndex << " ..." << "causes " << cubesThatFall.size() << " cubes to fall" << std::endl;

	//	if (cubesThatFall.size() == 0) {
	//		result++;
	//	}
	//	sum += cubesThatFall.size();
	//}

	for (int i = 0; i < cubes.size(); ++i) {
		const cube& c = cubes[i];
		cube movedDown = c;
		movedDown.MoveDown(1);
		for (int ii = 0; ii < cubes.size(); ii++) {
			if (i == ii) continue;
			if (movedDown.intersects(cubes[ii])) {
				cubesToCubesAbove[ii].push_back(i);
				cubesToCubesBelow[i].push_back(ii);
				std::cout << i << " rests on " << ii << std::endl;
			}
		}
		std::cout << ".. " << i << " rests on numCubes: " << cubesToCubesBelow[i].size() << std::endl;
	}

	std::unordered_map<int, int> cubesWouldFallMap;

	std::function<int(int)> GetAmountCubesThatWouldFall;
	GetAmountCubesThatWouldFall = [&cubesToCubesAbove, &cubesToCubesBelow, &GetAmountCubesThatWouldFall, &cubesWouldFallMap](int cubeIndex) {
		if (cubesWouldFallMap.count(cubeIndex)) {
			return cubesWouldFallMap[cubeIndex];
		}
		int amount = 0;
		auto& cubesRestingOnThis = cubesToCubesAbove[cubeIndex];
		for (auto cubeOnThis : cubesRestingOnThis) {
			//if the cube only rests on this cube, it will fall.
			int amtThisCubeRestsOn = cubesToCubesBelow[cubeOnThis].size();
			if (amtThisCubeRestsOn < 2) {
				amount += 1; //this cube would fall
				amount += GetAmountCubesThatWouldFall(cubeOnThis); //add all cubes above this one
			}
		}

		std::cout << cubeIndex << " would make " << amount << " cubes fall!" << std::endl;

		cubesWouldFallMap[cubeIndex] = amount;

		return amount;
	};

	for (int i = 0; i < cubes.size(); i++) {


		int amtRestingOnThisCube = cubesToCubesAbove.count(i);
		if (amtRestingOnThisCube == 0) {
			result++; //can be safely destroyed
		}
		else {
			int amountThatWouldFall = GetAmountCubesThatWouldFall(i);
			sum += amountThatWouldFall;
			auto cubesRestingOnThis = cubesToCubesAbove[i];
			bool canBeDeleted = true;
			for (auto cubeOnThis : cubesRestingOnThis) {
				int amtThisCubeRestsOn = cubesToCubesBelow[cubeOnThis].size();
				if (amtThisCubeRestsOn < 2) {
					canBeDeleted = false;
					break;
				}

			}
			if (canBeDeleted) {
				result++;
			}
		}

	}

	std::cout << result << " p2: " << sum << std::endl;
}