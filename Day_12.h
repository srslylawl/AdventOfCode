#pragma once
#include "Utils.h"

struct arrangement {
	std::string wells;
	std::vector<int> contiguousGroups;
	std::vector<int> unknownIndices;
};

bool arrangementIsValid(std::string wells, std::vector<int> groups) {
	std::string example = '.' + wells + '.'; //append broken wells to avoid checks

	int currentGroupIndex = 0;
	int startIndex = 0;

	bool insideGroup = false;
	for (size_t charIndex = 1; charIndex < example.size(); charIndex++) {
		char curr = example[charIndex];

		if (curr == '#') {
			if (!insideGroup) {
				if(currentGroupIndex >= groups.size()) return false;
				startIndex = charIndex;
				insideGroup = true;
			}
		}
		else {
			if (insideGroup) {
				insideGroup = false;
				int length = charIndex - startIndex;
				if (length != groups[currentGroupIndex]) {
					return false;
				}

				currentGroupIndex++;
			}
		}
	}



	return currentGroupIndex == groups.size();
}

void DoDay12() {
	auto lines = GetLines("input/day12.txt");

	std::vector<arrangement> arrangements;
	arrangements.resize(lines.size());
	
	for (size_t lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
		int numbersIndex = 0;
		for (size_t charIndex = 0; charIndex < lines[lineIndex].size(); charIndex++) {
			char currentChar = lines[lineIndex][charIndex];

			if (currentChar == ' ') {
				arrangements[lineIndex].wells = lines[lineIndex].substr(0, charIndex);
				numbersIndex = charIndex + 1;
				continue;
			}

			if (currentChar == '?') {
				arrangements[lineIndex].unknownIndices.push_back(charIndex);
				continue;
			}

			bool endOfLine = charIndex == lines[lineIndex].size() - 1;
			if (currentChar == ',' || charIndex == lines[lineIndex].size() - 1) {
				int len = charIndex - numbersIndex;
				if (endOfLine) len++;
				std::string numstr = lines[lineIndex].substr(numbersIndex, len);
				int num = ParseDigitFromString(numstr);
				arrangements[lineIndex].contiguousGroups.push_back(num);
				numbersIndex = charIndex + 1;
			}
		}
	}

	int possibilities = 0;
	std::vector<char> symbols = { '.', '#' };
	for (size_t aIndex = 0; aIndex < arrangements.size(); aIndex++) {
		//solve each arrangement
		auto& arrangement = arrangements[aIndex];

		std::cout << "Line: " << arrangement.wells << " ";
		for (auto g : arrangement.contiguousGroups) {
			std::cout << g << ",";
		}
		std::cout << std::endl;

		int possibilitiesCurrent = 0;
		auto permutations = GeneratePermutations(arrangement.unknownIndices.size(), &symbols);
		for (auto& p : permutations) {
			std::string result = arrangement.wells;
			for (int i = 0; i < arrangement.unknownIndices.size(); i++) {
				int index = arrangement.unknownIndices[i];
				char symbol = p[i];
				result[index] = symbol;
			}

			bool valid = arrangementIsValid(result, arrangement.contiguousGroups);

			if(valid) {
				//std::cout << "valid: " << result << std::endl;
				possibilitiesCurrent++;
			}
			else {
				//std::cout << "invalid: " << result << std::endl;
			}
		}

		//std::cout << "Valid: " << possibilitiesCurrent << std::endl;

		possibilities+= possibilitiesCurrent;
	}

	std::cout << "result: " << possibilities << std::endl;
}