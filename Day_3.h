#pragma once
#include "Utils.h"

#define NOMINMAX
#include <windows.h>


bool IsSymbol(char c) {
	return c != '.' && c != CharIsDigit(c);
}

struct number {
	int value = 0;
	int row = 0;
	int startIndex = 0;
	int characterLength = 0;

	bool IsAdjacentToSymbol(const std::vector<std::string>& lines) {

		int rowAbove = row - 1;
		if (rowAbove >= 0) {
			//above
			const auto& line = lines[rowAbove];
			for (int i = 0; i < characterLength + 2 && i < line.size(); ++i) {
				int rowIndex = std::max<int>(0, startIndex - 1 + i);
				if (IsSymbol(line[rowIndex])) {
					return true;
				}
			}
		}

		int rowBelow = row + 1;
		if (rowBelow < lines.size()) {
			const auto& line = lines[rowBelow];
			for (int i = 0; i < characterLength + 2 && i < line.size(); ++i) {
				int rowIndex = std::max(0, startIndex - 1 + i);
				if (IsSymbol(line[rowIndex])) {
					return true;
				}
			}
		}

		//left
		const auto& line = lines[row];
		int left = startIndex - 1;
		if (left >= 0) {
			if (IsSymbol(line[left])) return true;
		}

		//right
		int right = startIndex + characterLength;
		if (right < lines.size()) {
			if (IsSymbol(line[right])) return true;
		}

		return false;
	}

	bool IsAtCoordinate(int row, int index) {
		if (row != this->row) return false;

		if (index < startIndex || index >= startIndex + characterLength) return false;

		return true;
	}

};

void DoDay3() {
	auto lines = GetLines("input/day3.txt");

	//get numbers
	std::vector<number> numbers;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int lineIndex = 0;
	for (const auto& line : lines) {

		int currentNumberStartIndex = 0;
		int currentlyParsingNumber = false;
		for (int i = 0; i < lines.size(); i++) {
			const char& currentChar = line[i];



			bool isNumber = CharIsDigit(currentChar);
			if (!currentlyParsingNumber && isNumber) {
				currentlyParsingNumber = true;
				currentNumberStartIndex = i;
			}

			bool lastCharacter = i + 1 == lines.size();

			if (!currentlyParsingNumber)
				std::cout << currentChar;


			if (currentlyParsingNumber && (!isNumber || lastCharacter)) {
				currentlyParsingNumber = false;
				int digitLength = i - currentNumberStartIndex;
				if (lastCharacter && isNumber) digitLength++;
				std::string numberString = line.substr(currentNumberStartIndex, digitLength);
				number n;
				n.row = lineIndex;
				n.startIndex = currentNumberStartIndex;
				n.characterLength = digitLength;
				n.value = ParseDigitFromString(numberString);

				numbers.push_back(n);

				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED);
				std::cout << n.value;
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

				if (!isNumber) {
					std::cout << currentChar;
				}
			}


		}
		std::cout << std::endl;
		lineIndex++;
	}

	int result = 0;

	for (auto n : numbers) {
		std::cout << "checking " << n.value;
		if (n.IsAdjacentToSymbol(lines)) {
			result += n.value;
			std::cout << " adjacent!\n";
		}
		else {
			std::cout << " not adjacent!\n";
		}
	}

	std::cout << result << std::endl;
}

void DoDay3_2() {
	auto lines = GetLines("input/day3.txt");

	//get numbers
	std::vector<number> numbers;

	int lineIndex = 0;
	for (const auto& line : lines) {

		int currentNumberStartIndex = 0;
		int currentlyParsingNumber = false;
		for (int i = 0; i < lines.size(); i++) {
			const char& currentChar = line[i];



			bool isNumber = CharIsDigit(currentChar);
			if (!currentlyParsingNumber && isNumber) {
				currentlyParsingNumber = true;
				currentNumberStartIndex = i;
			}

			bool lastCharacter = i + 1 == lines.size();

			if (currentlyParsingNumber && (!isNumber || lastCharacter)) {
				currentlyParsingNumber = false;
				int digitLength = i - currentNumberStartIndex;
				if (lastCharacter && isNumber) digitLength++;
				std::string numberString = line.substr(currentNumberStartIndex, digitLength);
				number n;
				n.row = lineIndex;
				n.startIndex = currentNumberStartIndex;
				n.characterLength = digitLength;
				n.value = ParseDigitFromString(numberString);

				numbers.push_back(n);
			}


		}
		std::cout << std::endl;
		lineIndex++;
	}
	int result = 0;

	//find gears
	lineIndex = 0;
	for (const auto& line : lines) {

		for (int i = 0; i < lines.size(); i++) {
			const char& currentChar = line[i];
			if (currentChar != '*') continue;

			std::cout << "X: " << i << " Y:" << lineIndex << " could be a gear..." << std::endl;

			std::vector<number*> adjacentNumbers;
			auto addIfUnique = [&adjacentNumbers](number* n) {
				auto res = std::find(adjacentNumbers.begin(), adjacentNumbers.end(), n);
				if (res == adjacentNumbers.end()) {
					std::cout << " adjacent number: " << n->value << std::endl;
					adjacentNumbers.push_back(n);
				}
			};

			auto addNumberAtCoordinate = [&numbers, &addIfUnique](int x, int y) {
				for (int numberIndex = 0; numberIndex < numbers.size(); numberIndex++) {
					number* currentNumber = &numbers[numberIndex];
					if (currentNumber->IsAtCoordinate(y, x)) {
						addIfUnique(currentNumber);
					}
				}
			};
			//get all surrounding numbers
			//above

			addNumberAtCoordinate(i - 1, lineIndex - 1);
			addNumberAtCoordinate(i, lineIndex - 1);
			addNumberAtCoordinate(i + 1, lineIndex - 1);

			addNumberAtCoordinate(i - 1, lineIndex);
			addNumberAtCoordinate(i + 1, lineIndex);

			addNumberAtCoordinate(i - 1, lineIndex + 1);
			addNumberAtCoordinate(i, lineIndex + 1);
			addNumberAtCoordinate(i + 1, lineIndex + 1);

			//std::cout << "adjacent numbers: " << adjacentNumbers.size() << std::endl;

			if (adjacentNumbers.size() == 2) {
				result += (adjacentNumbers[0]->value * adjacentNumbers[1]->value);
			}
		}
		lineIndex++;
	}


	std::cout << result << std::endl;
}