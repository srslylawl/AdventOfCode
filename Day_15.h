#pragma once

#include "Utils.h"

struct step {
	std::string full;
	std::string label;
	char operation;
	int focalLength = 0;
};

struct lens {
	std::string label;
	int focalLength;
};

struct box {
	std::vector<lens> lenses;
};

int HashFunction(std::string& input) {
	int result = 0;
	for (size_t i = 0; i < input.size(); i++) {
		char current = input[i];
		int intValue = (int)current;
		result += intValue;
		result *= 17;
		result %= 256;
	}

	return result;
}

void DoDay15() {

	auto lines = GetLines("input/day15.txt");

	std::vector<step> steps;

	for (size_t i = 0; i < lines.size(); i++) {
		auto& line = lines[i];

		int startIndex = 0;
		for (size_t charIndex = 0; charIndex < line.size(); charIndex++) {
			char current = line[charIndex];

			bool atEnd = charIndex == line.size() - 1;
			if (current == ',' || atEnd) {
				int len = charIndex - startIndex;
				if (atEnd) len++;
				step s;
				s.full = line.substr(startIndex, len);

				for (size_t c = 0; c < s.full.size(); c++) {
					if (s.full[c] == '-' || s.full[c] == '=') {
						s.label = s.full.substr(0, c);
						s.operation = s.full[c];
						if (s.full[c] == '=') {
							s.focalLength = ParseDigitFromString(s.full.substr(c + 1, 1));
						}
					}
				}

				steps.push_back(s);
				startIndex = charIndex + 1;
			}
		}
	}

	std::vector<box> boxes;
	boxes.resize(256);

	int result = 0;
	for (size_t i = 0; i < steps.size(); i++) {
		step& step = steps[i];
		int boxIndex = HashFunction(step.label);
		box& box = boxes[boxIndex];

		auto res = std::find_if(box.lenses.begin(), box.lenses.end(), [&step](lens& in) {return in.label == step.label; });
		if (step.operation == '-') {
			if (res != box.lenses.end()) {
				box.lenses.erase(res);
			}
		}
		else {
			lens lens;
			lens.label = step.label;
			lens.focalLength = step.focalLength;

			if (res != box.lenses.end()) {
				*res = lens;
			}
			else {
				box.lenses.push_back(lens);
			}
		}


		//result += HashFunction(steps[i].full);
	}

	for (size_t i = 0; i < boxes.size(); i++) {
		box& box = boxes[i];
		for (size_t lIndex = 0; lIndex < box.lenses.size(); lIndex++) {
			int res = (1+i) * (lIndex+1) * box.lenses[lIndex].focalLength;
			result += res;
		}
		
	}

	std::cout << result << std::endl;
}