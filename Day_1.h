#pragma once
#include "Utils.h"
#include <vector>

void DoDay1() {

	std::vector<std::string> lines;

	HandleLine("input/day1.txt", [&lines](std::string line) {
		lines.push_back(line);
	});

	int linesCount = lines.size();
	int* numbers = new int[linesCount] {};

	for (size_t i = 0; i < linesCount; i++) {
		//current line
		numbers[i] = 0;
		int lineSize = lines[i].size();

		//first digit
		for (int charIndex = 0; charIndex < lineSize; charIndex++) {
			//check if is number
			int intVal = lines[i][charIndex] - '0';
			if (intVal <= 0 || intVal > 9) {
				continue;
			}
			numbers[i] += intVal * 10;
			break;
		}

		//second digit

		for (int charIndex = lineSize - 1; charIndex >= 0; charIndex--) {
			//check if is number
			int intVal = lines[i][charIndex] - '0';
			if (intVal <= 0 || intVal > 9) {
				continue;
			}
			numbers[i] += intVal;
			break;
		}
		std::cout << "number for line: " << lines[i] << " is " << numbers[i] << std::endl;
	}

	int result = 0;
	for (size_t i = 0; i < linesCount; i++) {
		result += numbers[i];
	}

	std::cout << "result: " << result << std::endl;
}


bool GetNumberFromString(std::string& in, int* outNumber, int* startIndex) {
	const std::vector<std::string> possibleNumbersList = {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
	};
	for (size_t startCharIndex = 0; startCharIndex < in.size(); startCharIndex++) {
		//for each character, check if something in our dict has the same sequence
		for (int currentDictEntryIndex = 0; currentDictEntryIndex < possibleNumbersList.size(); ++currentDictEntryIndex) {
			const std::string& currentDictEntry = possibleNumbersList[currentDictEntryIndex];
			int remainingSize = in.size() - startCharIndex;
			for (size_t currentChar = 0; currentChar < remainingSize; currentChar++) {
				if (currentDictEntry.size() <= currentChar) {
					//too short
					break;
				}

				if (in[currentChar+startCharIndex] != currentDictEntry[currentChar]) {
					//mismatch
					break;
				}

				//same character, are we at end?
				if (currentChar == currentDictEntry.size() - 1) {
					//found! our number is
					*outNumber = currentDictEntryIndex + 1;
					*startIndex = startCharIndex;

					return true;
				}
			}
		}
	}

	return false;
}

void DoDay1_2() {

	std::vector<std::string> lines;

	HandleLine("input/day1.txt", [&lines](std::string line) {
		lines.push_back(line);
	});

	int linesCount = lines.size();
	int* numbers = new int[linesCount] {};
	for (size_t i = 0; i < linesCount; i++) {
		//current line
		numbers[i] = 0;
		int lineSize = lines[i].size();

		//first digit
		int firstDigitIndex = lineSize;
		int firstDigitValue = 0;
		for (int charIndex = 0; charIndex < lineSize; charIndex++) {
			//check if is number
			int intVal = lines[i][charIndex] - '0';
			if (intVal <= 0 || intVal > 9) {
				continue;
			}
			firstDigitValue = intVal;
			firstDigitIndex = charIndex;
			break;
		}

		//see where our first written number is
		int writtenNumberIndex = lineSize;
		int writtenNumberValue = 0;

		int firstNumber = firstDigitValue;
		if (GetNumberFromString(lines[i], &writtenNumberValue, &writtenNumberIndex)) {
			//we found a number, check if it comes before our first digit
			if (writtenNumberIndex < firstDigitIndex) {
				firstNumber = writtenNumberValue;
			}
		}

		numbers[i] = firstNumber * 10;


		//second digit
		int lastDigitIndex = 0;
		int lastDigitValue = 0;
		for (int charIndex = lineSize - 1; charIndex >= 0; charIndex--) {
			//check if is number
			int intVal = lines[i][charIndex] - '0';
			if (intVal <= 0 || intVal > 9) {
				continue;
			}
			lastDigitValue = intVal;
			lastDigitIndex = charIndex;
			break;
		}

		//iterating from the back until we hit the lastDigitIndex or find a number
		int lastWrittenNumberIndex = 0;
		int lastWrittenNumberValue = 0;


		int secondNumber = lastDigitValue;

		for (int charIndex = lineSize - 1; charIndex >= lastDigitIndex; charIndex--) {
			int offset = charIndex;
			std::string currentString = lines[i].substr(offset, lineSize - charIndex);
			if (GetNumberFromString(currentString, &lastWrittenNumberValue, &lastWrittenNumberIndex)) {
				if (lastWrittenNumberIndex + offset > lastDigitIndex) {
					secondNumber = lastWrittenNumberValue;
				}
				break;
			}
		}

		numbers[i] += secondNumber;
		std::cout << "number for line: " << lines[i] << " is " << numbers[i] << std::endl;
	}

	int result = 0;
	for (size_t i = 0; i < linesCount; i++) {
		result += numbers[i];
	}

	std::cout << "result: " << result << std::endl;
}