#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>

void HandleLine(std::string path, std::function<void(std::string)> perLineCallback) {
	std::ifstream fileStream;
	fileStream.open(path.c_str());
	if (!fileStream.is_open()) {
		std::cout << "unable to open fileStream" << std::endl;

		return;
	}

	std::string input;
	while (std::getline(fileStream, input)) {
		perLineCallback(input);
	}

	fileStream.close();
}

std::vector<std::string> GetLines(std::string path) {
	std::vector<std::string> lines;

	HandleLine(path, [&lines](std::string line) {
		lines.push_back(line);
	});

	return lines;
}



int ParseDigitFromString(const std::string& string) {
	int digit = 0;

	int decimalPlace = 0;
	for (int digitIndex = string.size() - 1; digitIndex >= 0; digitIndex--) {
		int digitIntValue = string[digitIndex] - '0';
		digit += digitIntValue * pow(10, decimalPlace);
		decimalPlace++;
	}
	return digit;
}

long long ParseLongDigitFromString(const std::string& string) {
	long long digit = 0;

	int decimalPlace = 0;
	for (int digitIndex = string.size() - 1; digitIndex >= 0; digitIndex--) {
		long long digitIntValue = string[digitIndex] - '0';
		digit += digitIntValue * pow(10, decimalPlace);
		decimalPlace++;
	}
	return digit;
}

bool CharIsDigit(char c) {
	int intVal = c - '0';

	return intVal >= 0 && intVal <= 9;
}

std::vector<int> GetNumbersFromString(const std::string& string) {
	std::vector<int> result;
	bool isParsingNumber = false;

	int numberStartIndex;
	for (int i = 0; i < string.size(); i++) {
		const char& c = string[i];
		bool isDigit = CharIsDigit(c);

		bool endOfString = i == string.size() - 1;

		if (isDigit && !isParsingNumber) {
			isParsingNumber = true;
			numberStartIndex = i;
		}

		if ((!isDigit || endOfString) && isParsingNumber) {
			int endIndex = isDigit ? i : i - 1;
			std::string numStr = string.substr(numberStartIndex, endIndex - numberStartIndex + 1);

			result.push_back(ParseDigitFromString(numStr));

			isParsingNumber = false;
		}
	}

	return result;
}

std::vector<long long> GetLongNumbersFromString(const std::string& string) {
	std::vector<long long> result;
	bool isParsingNumber = false;

	int numberStartIndex;
	for (int i = 0; i < string.size(); i++) {
		const char& c = string[i];
		bool isDigit = CharIsDigit(c);

		bool endOfString = i == string.size() - 1;

		if (isDigit && !isParsingNumber) {
			isParsingNumber = true;
			numberStartIndex = i;
		}

		if ((!isDigit || endOfString) && isParsingNumber) {
			int endIndex = isDigit ? i : i - 1;
			std::string numStr = string.substr(numberStartIndex, endIndex - numberStartIndex + 1);

			result.push_back(ParseLongDigitFromString(numStr));

			isParsingNumber = false;
		}
	}

	return result;
}

std::vector<long long> GetLongSignedNumbersFromString(const std::string& string) {
	std::vector<long long> result;
	bool isParsingNumber = false;

	int numberStartIndex;
	for (int i = 0; i < string.size(); i++) {
		const char& c = string[i];
		bool isDigit = CharIsDigit(c);

		bool endOfString = i == string.size() - 1;

		if (isDigit && !isParsingNumber) {
			isParsingNumber = true;
			numberStartIndex = i;
		}

		if ((!isDigit || endOfString) && isParsingNumber) {
			int endIndex = isDigit ? i : i - 1;
			std::string numStr = string.substr(numberStartIndex, endIndex - numberStartIndex + 1);

			long long res = ParseLongDigitFromString(numStr);
			int signIndex = numberStartIndex - 1;
			if (signIndex >= 0 && string[signIndex] == '-') {
				res *= -1;
			}
			result.push_back(res);

			isParsingNumber = false;
		}
	}

	return result;
}




