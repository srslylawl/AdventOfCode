#pragma once
#include <map>

#include "Utils.h"
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <unordered_map>


void DoDay23() {
	auto lines = GetLines("input/day23.txt");

	auto t = GetCurrentTime();

	int height = lines.size();
	int width = lines[0].size();

	coord startCoord(1, 0);
	coord goal(width - 2, height - 1);

	struct customHigher {
		bool operator()(const pathNode& left, const pathNode& right) const {
			return (left.pathCost) < (right.pathCost);
		}
	};
	//std::priority_queue<pathNode, std::vector<pathNode>, customHigher> prioQueue;

	std::vector<std::unordered_set<coord>> visited;
	visited.resize(1);
	visited.reserve(99999);
	std::vector<std::unordered_set<coord>> visitedGraph = visited;

	pathNode startNode;
	startNode.pathCost = 0;
	startNode.lastDir = coord(0, 1);
	startNode.position = startCoord;
	startNode.seen = &visitedGraph[0];


	pathNode* startGraphPos = new pathNode();
	startGraphPos->position = startCoord;
	startGraphPos->cost = 0;
	startGraphPos->pathCost = 0;
	startNode.headPtr = startGraphPos;

	std::stack<pathNode> stack;
	stack.push(startNode);
	//prioQueue.push(startNode);

	const std::vector<coord> directions = {
		{0,1}, {0, -1}, {1, 0}, {-1, 0}
	};
	int highestCost = 0;

	std::unordered_set<coord> seen;

	std::map<coord, std::set<pathNode>> graph;



	auto GetNextPositions = [&lines, &directions, &goal, &graph, &width](pathNode current, std::unordered_set<coord>& seen) -> std::vector<pathNode> {
		std::vector<pathNode> result;
		char pathSymbol = lines[current.position.y][current.position.x];
		int dirIndex = 0;
		for (const auto& dir : directions) {
			if (current.lastDir == coord{ -dir.x, -dir.y }) continue; //don't go back
			coord nextPos(current.position.x + dir.x, current.position.y + dir.y);
			if (nextPos.x >= width || nextPos.y >= width || nextPos.y < 0 || nextPos.x < 0) continue;
			pathNode nextNode;
			nextNode.position = nextPos;
			nextNode.pathCost = current.pathCost + 1;
			nextNode.cost = goal.x - nextPos.x + goal.y - nextPos.y;
			nextNode.lastDir = dir;
			nextNode.headPtr = current.headPtr;
			nextNode.seen = current.seen;

			bool isGraphNode = false;

			if (graph.count(nextPos)) {
				isGraphNode = true;
			}

			if (seen.count(nextPos) && !isGraphNode) continue;
			//seen.insert(nextPos);
			//auto currentVisited = ((std::unordered_set<coord>*)current.headPtr);
			//if (currentVisited->count(nextPos)) {
			//	continue;
			//}
			//currentVisited->insert(nextPos);


			char nextPathSymbol = lines[nextPos.y][nextPos.x];
			if (nextPathSymbol == '#') continue;
			//if (nextPathSymbol == 'v' && dir != coord(0, 1)) continue;
			//if (nextPathSymbol == '<' && dir != coord(-1, 0)) continue;
			//if (nextPathSymbol == '>' && dir != coord(1, 0)) continue;
			//if (nextPathSymbol == '^' && dir != coord(0, -1)) continue;

			//if (pathSymbol == '>' && dir != coord{ 1, 0 }) continue;
			//if (pathSymbol == 'v' && dir != coord{ 0, 1 }) continue;
			//if (pathSymbol == '^' && dir != coord{ 0, -1 }) continue;
			//if (pathSymbol == '<' && dir != coord{ -1, 0 }) continue;

			//if(dirIndex == 0) {
			//	nextNode.headPtr = current.headPtr;
			//}
			//else {
			//	std::unordered_set<coord> newSet = *currentVisited;
			//	visited.emplace_back(std::move(newSet));
			//	nextNode.headPtr = &visited[visited.size()-1];
			//}
			result.push_back(nextNode);
			dirIndex++;
		}

		return result;
	};


	//build graph by remembering intersection points
	auto mapCopy = lines;

	while (!stack.empty()) {
		auto currentNode = stack.top();
		stack.pop();


		bool isGoal = currentNode.position == goal;
		if (isGoal) {
			std::cout << "goal reached in steps: " << currentNode.pathCost << std::endl;
			//continue;
		}

		currentNode.seen->insert(currentNode.position);
		mapCopy[currentNode.position.y][currentNode.position.x] = 'O';

		auto nextPositions = GetNextPositions(currentNode, *currentNode.seen);

		//PrintMap(mapCopy);

		bool isGraphNode = graph.count(currentNode.position) && currentNode.position != startCoord;


		if (nextPositions.size() == 0) {
			//mapCopy[currentNode.position.y][currentNode.position.x] = '?';
			//PrintMap(mapCopy);
			if (!isGoal && !isGraphNode) {
				delete currentNode.headPtr;
				continue;
			}

		}

		if (nextPositions.size() == 1 && !isGoal && !isGraphNode) {
			stack.push(nextPositions[0]);
			continue;
		}

		if (isGraphNode) {
			//std::cout << "revisiting node " << std::endl;
		}



		pathNode thisIntersection;
		thisIntersection.position = currentNode.position;

		if (currentNode.headPtr) {
			pathNode* lastIntersection = (pathNode*)currentNode.headPtr;
			int costToThis = currentNode.pathCost;
			thisIntersection.pathCost = costToThis;
			graph[lastIntersection->position].insert(thisIntersection);
			lastIntersection->pathCost = costToThis;

			//std::cout << "graph node: x" << thisIntersection.position.x << " y" << thisIntersection.position.y << " cost: " << lastIntersection->pathCost << " -> x" << lastIntersection->position.x << " y" << lastIntersection->position.y << " cost: " << costToThis << std::endl;
			//PrintMap(mapCopy);
			//mapCopy = lines;
			//if (lastIntersection->position != startCoord) {
			graph[thisIntersection.position].insert(*lastIntersection);
			//}

			delete lastIntersection;
		}



		if (isGoal || isGraphNode) continue;
		//is intersection
		char& c = mapCopy[currentNode.position.y][currentNode.position.x];
		c = 'X';



		int nodeIndex = 0;
		for (auto& pn : nextPositions) {
			//this intersection
			pathNode* newIntersection = new pathNode(thisIntersection);
			pn.headPtr = newIntersection;
			pn.pathCost = 1;
			stack.push(pn);
			nodeIndex++;
		}
	}
	int edges = 0;
	int sumCosts = 0;
	for (const auto& collection : graph) {
		edges += collection.second.size();
		std::cout << "node: x" << collection.first.x << "y" << collection.first.y << " edges:";
		for (const auto& idk : collection.second) {
			std::cout << " (x" << idk.position.x << "y" << idk.position.y << "c" << idk.pathCost << ")";
			sumCosts += idk.pathCost;
		}

		std::cout << std::endl;
	}
	std::cout << "graph nodes: " << graph.size() << " edges: " << edges << std::endl;
	std::cout << "sumcosts: " << sumCosts << std::endl;


	startNode.seen = new std::unordered_set<coord>();

	//std::stack<pathNode> stack;

	//stack.push(startNode);

	std::vector<std::vector<bool>> seenPositions;
	seenPositions.resize(width);
	for (int i = 0; i < width; ++i) {
		seenPositions[i].resize(width);
	}
	std::function<void(std::tuple<coord, int>)> dfs;
	int count = 0;
	dfs = [&seenPositions, &graph, &goal, &dfs, &count, &highestCost](std::tuple<coord, int> node)-> void {
		count++;
		if (std::get<0>(node) == goal) {
			highestCost = std::max(highestCost, std::get<1>(node));
			return;
		}

		coord pos = std::get<0>(node);
		if (seenPositions[pos.y][pos.x]) return;
		seenPositions[pos.y][pos.x] = true;

		auto& next = graph[pos];
		for (const auto& n : next) {
			if (seenPositions[n.position.y][n.position.x]) continue;
			auto next = std::make_tuple(n.position, n.pathCost);

			std::get<1>(next) += std::get<1>(node);
			dfs(next);
		}
		seenPositions[pos.y][pos.x] = false;
	};
	auto startTuple = std::make_tuple(startNode.position, 0);
	dfs(startTuple);
	std::cout << highestCost << " count: " << count << std::endl;

	auto tend = GetCurrentTime();

	auto seconds = GetSeconds(t, tend);

	std::cout << "duration: " << seconds << std::endl;
}