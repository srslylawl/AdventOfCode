#pragma once
#include "Utils.h"
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <set>
#include <unordered_set>


struct pathNode {
	coord position;
	coord lastDir;
	int cost;
	int countSameDir;
	int pathCost;
	//std::vector<pathNode> previous;

	bool operator==(const pathNode& other) const {
		return position == other.position &&
			lastDir == other.lastDir &&
			countSameDir == other.countSameDir; // &&

			//cost == other.cost &&
			//pathCost == other.pathCost;
	}
};

#define pathTS(p) "x " << (p).position.x << " y " << (p).position.y << " from: x " << p.lastDir.x << " y " << p.lastDir.y << " ct: " << p.countSameDir << " cost: " << p.cost << " pcost: " << p.pathCost

namespace std {
	template<>
	struct hash<pathNode> {
		std::size_t operator()(const pathNode& c) const {
			std::size_t res = 17;
			res = res * 31 + hash<coord>()(c.position);
			res = res * 31 + hash<coord>()(c.lastDir);
			res = res * 31 + hash<int>()(c.countSameDir);

			//res = res * 31 + hash<int>()(c.cost);
			//res = res * 31 + hash<int>()(c.pathCost);
			return res;
		}
	};
}


std::vector<std::string> lines;
std::vector<std::vector<int>> nums;
std::unordered_map<coord, int> pathCosts;
const std::vector<coord> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

std::vector<pathNode> getSurroundingNodes(coord pos, coord lastDir, coord endPos, int countSameDir) {
	std::vector<pathNode> result;
	result.reserve(3);
	for (const auto& dir : directions) {

		coord next = pos;
		next.x += dir.x;
		next.y += dir.y;

		bool outOfBounds = next.y < 0 || next.y > endPos.y || next.x < 0 || next.x > endPos.x;
		if (outOfBounds) continue;

		bool isOppositeDirection = (dir.x != 0 && dir.x == -lastDir.x) || (dir.y != 0 && dir.y == -lastDir.y);
		if (isOppositeDirection) continue;

		int sameDir = countSameDir;
		bool sameDirectionAsLast = dir == lastDir;
		if (sameDirectionAsLast) {
			sameDir++;
		}

		if (sameDir >= 10) {
			continue;
		}

		if (sameDir < 3 && !sameDirectionAsLast) {
			continue;
		}

		if (!sameDirectionAsLast) {
			sameDir = 0;
		}

		int cost = nums[next.y][next.x];
		int distFromEnd = (endPos.x + endPos.y) - (pos.x + pos.y);

		pathNode node;
		node.cost = cost;
		node.pathCost = cost + distFromEnd;
		node.position = next;
		node.lastDir = dir;
		node.countSameDir = sameDir;

		result.push_back(node);
	}

	return result;
}


void DoDay17() {
	lines = GetLines("input/day17.txt");
	nums.resize(lines.size());
	for (size_t i = 0; i < lines.size(); i++) {
		nums[i].resize(lines[i].size());
		for (size_t j = 0; j < lines[i].size(); j++) {
			const char c = lines[i][j];
			int digit = ParseDigitFromString({ c });
			nums[i][j] = digit;
		}
	}

	auto pathMap = lines;

	//navigate to end
	coord endPos;
	endPos.y = lines.size() - 1;
	endPos.x = lines[endPos.y].size() - 1;


	//cost to end = endpos.x - currentpos.x + endpos.y - currentpos.y

	coord startPos(0, 0);

	struct customLess {
		bool operator()(const pathNode& left, const pathNode& right) const {
			return (left.pathCost * 1) + left.cost > (right.pathCost * 1) + right.cost;
		}
	};
	std::priority_queue<pathNode, std::vector<pathNode>, customLess> prioQueue;

	pathNode start;
	start.position = startPos;
	start.lastDir = { 1, 0 };
	start.cost = 0;
	start.pathCost = 0;
	start.countSameDir = 0;

	prioQueue.push(start);

	start.lastDir = { 0, 1 };
	prioQueue.push(start);

	std::unordered_set<pathNode> seen;
	int solutionCost = INT_MAX;

	while (!prioQueue.empty()) {
		pathNode next = prioQueue.top();
		prioQueue.pop();
		if (next.cost > solutionCost) continue;

		//std::cout << " checking: " << pathTS(next) << std::endl;

		auto nextNodes = getSurroundingNodes(next.position, next.lastDir, endPos, next.countSameDir);

		for (auto& node : nextNodes) {
			node.cost += next.cost;
			//node.previous = next.previous;
			//pathNode prevCopy = next;
			//prevCopy.previous.clear();
			//node.previous.push_back(prevCopy);

			if (node.cost > solutionCost) continue;
			//only add node if we havent explored it yet
			if (!seen.count(node)) {
				prioQueue.push(node);
				seen.insert(node);
			}
			else {
				//already seen
				//std::cout << "Already checked x " << node.position.x << " y " << node.position.y << std::endl;
			}
			auto it = pathCosts.find(node.position);
			if (it == pathCosts.end() || it->second > node.cost) {
				pathCosts[node.position] = node.cost;
				//std::cout << "X: " << node.position.x << " Y: " << node.position.y << " -> " << node.cost << std::endl;
			}

			if (node.position == endPos) {
				solutionCost = node.cost;
				std::cout << "Solution: " << solutionCost << pathTS(node) << std::endl;
				//auto mapCpy = pathMap;

				//for (auto pn : node.previous) {
				//	char c = 'x';
				//	mapCpy[pn.position.y][pn.position.x] = c;
				//}
				//PrintMap(mapCpy);

				//goto end;
			}
		}
		//auto temp = pathMap[next.position.y][next.position.x];
		//pathMap[next.position.y][next.position.x] = 'X';
		//PrintMap(pathMap);
		//std::cout << std::endl;
		//pathMap[next.position.y][next.position.x] = temp;
		//std::cout << std::endl;
	}

end:
	std::cout << pathCosts[endPos] << std::endl;
}