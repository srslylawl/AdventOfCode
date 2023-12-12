#pragma once
#include "Utils.h"
#include <functional>
#include <unordered_map>

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
				if (currentGroupIndex >= groups.size()) return false;
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

struct iteration {
	int charIndex, groupIndex, currentGroupLength;

	bool operator==(const iteration& other) const {
		return charIndex == other.charIndex 
			&& groupIndex == other.groupIndex 
			&& currentGroupLength == other.currentGroupLength;
	}
};

namespace std {
	template<>
	struct hash<iteration> {
		std::size_t operator()( const iteration& k ) const
    {

        std::size_t res = 17;
        res = res * 31 + hash<int>()( k.charIndex );
        res = res * 31 + hash<int>()( k.currentGroupLength );
        res = res * 31 + hash<int>()( k.groupIndex );
        return res;
    }
	
	};
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

	for (size_t i = 0; i < arrangements.size(); i++) {
		auto& arrangement = arrangements[i];

		std::string newWells = arrangement.wells;
		std::vector<int> newGroups = arrangement.contiguousGroups;

		for (size_t j = 0; j < 4; j++) {
			newWells += '?' + arrangement.wells;
			newGroups.insert(newGroups.end(), arrangement.contiguousGroups.begin(), arrangement.contiguousGroups.end());
		}

		std::vector<int> newUnknowns;
		newUnknowns.reserve(arrangement.unknownIndices.size() * 5);
		for (size_t k = 0; k < newWells.size(); k++) {
			char c = newWells[k];
			if (c == '?') {
				newUnknowns.push_back(k);
			}
		}

		arrangement.wells = newWells;
		arrangement.contiguousGroups = newGroups;
		arrangement.unknownIndices = newUnknowns;
	}




	int possibilities = 0;
	size_t possDyn = 0;
	std::vector<char> symbols = { '.', '#' };
	for (size_t aIndex = 0; aIndex < arrangements.size(); aIndex++) {
		//solve each arrangement
		auto& arrangement = arrangements[aIndex];

		std::unordered_map<iteration, size_t> cache;

		std::function<size_t(std::string&, std::vector<int>&, int, int, int)> f;
		f = [&](std::string& line, std::vector<int>& groups, int charIndex, int groupIndex, int currentGroupLength) -> size_t {
			// if at end 
			iteration key;
			key.charIndex = charIndex;
			key.groupIndex = groupIndex;
			key.currentGroupLength = currentGroupLength;

			auto res = cache.find(key);
			if (res != cache.end()) {
				return res->second;
			}

			if (charIndex == line.size()) {
				//end reached and all good
				if (groupIndex == groups.size() && currentGroupLength == 0) {
					return 1;
				}
				if (groupIndex == groups.size() - 1 && currentGroupLength == groups[groupIndex])
					return 1; //last symbol and is right size

				return 0;
			}
			size_t answer = 0;
			for (char s : symbols) {
				if (!(line[charIndex] == s || line[charIndex] == '?')) continue;

				if (s == '.' && currentGroupLength != 0 && groupIndex < groups.size() && currentGroupLength == groups[groupIndex]) {
					answer += f(line, groups, charIndex + 1, groupIndex + 1, 0);
				}
				else if (s == '.' && currentGroupLength == 0) {
					answer += f(line, groups, charIndex + 1, groupIndex, 0);
				}
				else if (s == '#') {
					answer += f(line, groups, charIndex + 1, groupIndex, currentGroupLength + 1);
				}
			}
			cache[key] = answer;
			return answer;

		};

		possDyn += f(arrangement.wells, arrangement.contiguousGroups, 0, 0, 0);

		std::cout << "Line: " << arrangement.wells << " ";
		for (auto g : arrangement.contiguousGroups) {
			std::cout << g << ",";
		}
		std::cout << " res: " << possDyn << std::endl;

		//int possibilitiesCurrent = 0;
		//auto permutations = GeneratePermutations(arrangement.unknownIndices.size(), &symbols);
		//for (auto& p : permutations) {
		//	std::string result = arrangement.wells;
		//	for (int i = 0; i < arrangement.unknownIndices.size(); i++) {
		//		int index = arrangement.unknownIndices[i];
		//		char symbol = p[i];
		//		result[index] = symbol;
		//	}

		//	bool valid = arrangementIsValid(result, arrangement.contiguousGroups);

		//	if(valid) {
		//		//std::cout << "valid: " << result << std::endl;
		//		possibilitiesCurrent++;
		//	}
		//	else {
		//		//std::cout << "invalid: " << result << std::endl;
		//	}
		//}

		////std::cout << "Valid: " << possibilitiesCurrent << std::endl;

		//possibilities+= possibilitiesCurrent;
	}

	//std::cout << "result: " << possibilities << std::endl;
	std::cout << "result dyn: " << possDyn << std::endl;
}