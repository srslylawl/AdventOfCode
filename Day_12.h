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
	for (size_t charIndex = 1; charIndex < example.size() && currentGroupIndex < groups.size(); charIndex++)
	{
		char curr = example[charIndex];

		if (curr == '#') {
			if (!insideGroup) {
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
	std::vector<char> characters = { '.', '#'};

	int stringLength = 5;
	auto powers = GetCharacterPowers(stringLength, characters.size());
	auto permutations = GeneratePermutations(&powers, &characters, stringLength);

	for (auto p : permutations) {
		std::cout << p << std::endl;
	}


	auto lines = GetLines("input/day12.txt");

	std::vector<arrangement> arrangements;
	arrangements.resize(lines.size());

	for (size_t lineIndex = 0; lineIndex < lines.size(); lineIndex++)
	{
		int numbersIndex = 0;
		for (size_t charIndex = 0; charIndex < lines[lineIndex].size(); charIndex++)
		{
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

	for (size_t aIndex = 0; aIndex < arrangements.size(); aIndex++)
	{
		//solve each arrangement
		auto& arrangement = arrangements[aIndex];
		for (size_t i = 0; i < arrangement.unknownIndices.size(); i++)
		{

		}
	}
}