#pragma once

#include "Utils.h"
#include <unordered_map>

struct entry {
	std::string left;
	std::string right;

	std::string GoTo(char c) {
		if (c == 'L') {
			return left;
		}

		return right;
	}
};

void DoDay8() {
	auto lines = GetLines("input/day8.txt");

	const std::string LRsequence = lines[0];

	std::unordered_map<std::string, entry> map;

	std::string destination = "ZZZ";
	std::string start = "AAA";

	for (int lineIndex = 2; lineIndex < lines.size(); lineIndex++) {

		int charIndex = 0;
		std::string mapName;
		for (; charIndex < lines[lineIndex].size(); charIndex++) {
			char current = lines[lineIndex][charIndex];
			if (current != ' ') continue;

			mapName = lines[lineIndex].substr(0, charIndex);
			break;
		}

		//if (lineIndex == 2) {
		//	start = mapName;
		//}

		//if (lineIndex == lines.size() - 1) {
		//	destination = mapName;
		//}

		int leftBeginIndex;
		int rightBeginIndex;
		std::string leftName;
		std::string rightName;
		for (; charIndex < lines[lineIndex].size(); charIndex++) {
			char current = lines[lineIndex][charIndex];
			if (current == '(') {
				leftBeginIndex = charIndex + 1;
				continue;
			};

			if (current == ',') {
				leftName = lines[lineIndex].substr(leftBeginIndex, charIndex - leftBeginIndex);
				rightBeginIndex = charIndex + 2;
			}

			if (current == ')') {
				rightName = lines[lineIndex].substr(rightBeginIndex, charIndex - rightBeginIndex);
			}
		}

		entry e;
		e.left = leftName;
		e.right = rightName;

		map.insert({ mapName, e });
	}

	for (auto it = map.begin(); it != map.end(); it++) {
		std::cout << it->first << " = " << "L: " << it->second.left << " R: " << it->second.right << std::endl;
	}

	std::string current = start;

	int sequenceIndex = 0;
	int steps = 0;
	while (current != destination) {
		steps++;
		sequenceIndex = sequenceIndex % LRsequence.size();

		char currentDir = LRsequence[sequenceIndex];

		//std::cout << current << " -> " << map[current].GoTo(currentDir) << std::endl;

		current = map[current].GoTo(currentDir);


		sequenceIndex++;
	}

	std::cout << steps << std::endl;
}

void DoDay8_2() {
	auto lines = GetLines("input/day8.txt");

	const std::string LRsequence = lines[0];

	std::unordered_map<std::string, entry> map;

	std::vector<std::string> startNodes;

	for (int lineIndex = 2; lineIndex < lines.size(); lineIndex++) {

		int charIndex = 0;
		std::string mapName;
		for (; charIndex < lines[lineIndex].size(); charIndex++) {
			char current = lines[lineIndex][charIndex];
			if (current != ' ') continue;

			mapName = lines[lineIndex].substr(0, charIndex);
			break;
		}

		if (mapName[mapName.size() - 1] == 'A') {
			startNodes.push_back(mapName);
		}

		int leftBeginIndex;
		int rightBeginIndex;
		std::string leftName;
		std::string rightName;
		for (; charIndex < lines[lineIndex].size(); charIndex++) {
			char current = lines[lineIndex][charIndex];
			if (current == '(') {
				leftBeginIndex = charIndex + 1;
				continue;
			};

			if (current == ',') {
				leftName = lines[lineIndex].substr(leftBeginIndex, charIndex - leftBeginIndex);
				rightBeginIndex = charIndex + 2;
			}

			if (current == ')') {
				rightName = lines[lineIndex].substr(rightBeginIndex, charIndex - rightBeginIndex);
			}
		}

		entry e;
		e.left = leftName;
		e.right = rightName;

		map.insert({ mapName, e });
	}

	for (auto it = map.begin(); it != map.end(); it++) {
		std::cout << it->first << " = " << "L: " << it->second.left << " R: " << it->second.right << std::endl;
	}
	std::vector<std::string> currentNodes = startNodes;
	std::vector<int> loopLengths;
	std::vector<int> lastLoopSteps;
	std::vector<int> stableAt;
	std::vector<bool> isStable;
	for (size_t i = 0; i < currentNodes.size(); i++) {
		loopLengths.push_back(0);
		lastLoopSteps.push_back(0);
		stableAt.push_back(0);
		isStable.push_back(false);
	}

	int sequenceIndex = 0;
	int steps = 0;
	while (true) {
		steps++;
		sequenceIndex = sequenceIndex % LRsequence.size();

		char currentDir = LRsequence[sequenceIndex];

		for (int currentIndex = 0; currentIndex < currentNodes.size(); currentIndex++) {
			std::string current = currentNodes[currentIndex];
			currentNodes[currentIndex] = map[current].GoTo(currentDir); //next


			if (currentNodes[currentIndex][currentNodes[currentIndex].size() - 1] == 'Z') {
				int last = lastLoopSteps[currentIndex];
				int currentLength = steps - last;
				if (currentLength == loopLengths[currentIndex]) {
					stableAt[currentIndex] = steps;
					std::cout << "STABLE at " << currentLength << " " << startNodes[currentIndex] << std::endl;
					isStable[currentIndex] = true;
				}
				else {
					std::cout << startNodes[currentIndex] << " hit Z at len " << currentLength
						<< ", previous: " << loopLengths[currentIndex] << std::endl;
					isStable[currentIndex] = false;
				}

				loopLengths[currentIndex] = currentLength;
				lastLoopSteps[currentIndex] = steps;
			}
		}

		bool allStable = true;
		for (auto stable : isStable) {
			if (!stable) {
				allStable = false;
				break;
			}
		}
		if(allStable) break;

		//std::cout << current << " -> " << map[current].GoTo(currentDir) << std::endl;
		sequenceIndex++;
	}

	std::cout << steps << std::endl;
}