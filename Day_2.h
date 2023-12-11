#pragma once
#include "Utils.h"
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>

void ParseGame() {
}

void DoDay2() {
	std::vector<std::string> lines;

	HandleLine("input/day2.txt", [&lines](std::string line) {
		lines.push_back(line);
	});

	int result = 0;
	for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
		const std::string& line = lines[lineIndex];

		std::cout << line << std::endl;


		const std::string beginning = "Game 1: ";

		int gameNumber = lineIndex + 1;
		int additionalOffset = log10((double)gameNumber); //add one additional offset for every digit in our game count, 1, 10, 100 etc

		int gameStartCharOffset = beginning.size() + additionalOffset;

		const std::string currentRounds = line.substr(gameStartCharOffset, line.size() - gameStartCharOffset);

		//split the game into rounds
		std::vector<std::string> rounds;
		int roundStartIndex = 0;
		for (size_t charIndex = 0; charIndex < currentRounds.size(); charIndex++) {
			const char& currentChar = currentRounds[charIndex];

			if (currentChar == ';') {
				rounds.push_back(currentRounds.substr(roundStartIndex, charIndex - roundStartIndex));
				roundStartIndex = charIndex + 2; //skip 1 for ; and one for the space after
			}

			if (charIndex == currentRounds.size() - 1) {
				//end of string
				rounds.push_back(currentRounds.substr(roundStartIndex, currentRounds.size() - roundStartIndex));
			}
		}

		//parse the rounds
		bool gamePossible = true;
		for (const auto& round : rounds) {
			if (!gamePossible) break;
			std::cout << " Round: " << round << std::endl;
			for (int charIndex = 0; charIndex < round.size(); charIndex++) {
				//check each character of round, in this loop we parse 1 digit and 1 color name each
				if (!gamePossible) break;

				//parse digit
				int digit = 0;
				int startOfDigit = charIndex;
				for (; charIndex < round.size(); charIndex++) {
					if (round[charIndex] == ' ' || charIndex == round.size() - 1) {
						//reached end of digit
						std::string digitStr = round.substr(startOfDigit, charIndex - startOfDigit);

						//parse digit
						digit = ParseDigitFromString(digitStr);
						break;
					}
				}
				std::cout << "  [count:] " << digit;
				charIndex++;

				//parse color
				const std::vector<std::string> colors = { "red", "green", "blue" };

				//find end of color
				int startOfColorIndex = charIndex;

				charIndex += 3; //since shortest color is red
				for (; charIndex <= round.size(); charIndex++) {
					auto currentSubString = round.substr(startOfColorIndex, charIndex - startOfColorIndex);

					auto res = std::find(colors.begin(), colors.end(), currentSubString);
					if (res != colors.end()) {

						std::cout << " [color:] " << *res;
						//found color
						int index = res - colors.begin();
						switch (index) {
							case 0:
								if (digit > 12) {
									gamePossible = false;
									std::cout << " IMPOSSIBLE" << std::endl;
								}
								break;
							case 1:
								if (digit > 13) {
									gamePossible = false;
									std::cout << " IMPOSSIBLE" << std::endl;
								}
								break;
							case 2:
								if (digit > 14) {
									gamePossible = false;
									std::cout << " IMPOSSIBLE" << std::endl;
								}
								break;
							default:
								break;
						}

						charIndex++;
						break;
					}
				}
				std::cout << std::endl;
			}
		}

		if (gamePossible) {
			result += gameNumber;
			std::cout << " game possible!" << std::endl;
		}
		else {
			std::cout << " game IMPOSSIBLE!" << std::endl;
		}

	}

	std::cout << result << std::endl;
}

void DoDay2_2() {
	std::vector<std::string> lines;

	HandleLine("input/day2.txt", [&lines](std::string line) {
		lines.push_back(line);
	});

	int result = 0;
	for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
		const std::string& line = lines[lineIndex];

		std::cout << line << std::endl;


		const std::string beginning = "Game 1: ";

		int gameNumber = lineIndex + 1;
		int additionalOffset = log10((double)gameNumber); //add one additional offset for every digit in our game count, 1, 10, 100 etc

		int gameStartCharOffset = beginning.size() + additionalOffset;

		const std::string currentRounds = line.substr(gameStartCharOffset, line.size() - gameStartCharOffset);

		//split the game into rounds
		std::vector<std::string> rounds;
		int roundStartIndex = 0;
		for (size_t charIndex = 0; charIndex < currentRounds.size(); charIndex++) {
			const char& currentChar = currentRounds[charIndex];

			if (currentChar == ';') {
				rounds.push_back(currentRounds.substr(roundStartIndex, charIndex - roundStartIndex));
				roundStartIndex = charIndex + 2; //skip 1 for ; and one for the space after
			}

			if (charIndex == currentRounds.size() - 1) {
				//end of string
				rounds.push_back(currentRounds.substr(roundStartIndex, currentRounds.size() - roundStartIndex));
			}
		}

		std::unordered_map<std::string, int> highestPerColor = {
			{"red", 0},
			{"green", 0},
			{"blue", 0}
		};

		//parse the rounds
		bool gamePossible = true;
		for (const auto& round : rounds) {
			if (!gamePossible) break;
			std::cout << " Round: " << round << std::endl;
			for (int charIndex = 0; charIndex < round.size(); charIndex++) {
				//check each character of round, in this loop we parse 1 digit and 1 color name each
				if (!gamePossible) break;

				//parse digit
				int digit = 0;
				int startOfDigit = charIndex;
				for (; charIndex < round.size(); charIndex++) {
					if (round[charIndex] == ' ' || charIndex == round.size() - 1) {
						//reached end of digit
						std::string digitStr = round.substr(startOfDigit, charIndex - startOfDigit);

						//parse digit
						digit = ParseDigitFromString(digitStr);
						break;
					}
				}
				std::cout << "  [count:] " << digit;
				charIndex++;

				//parse color
				const std::vector<std::string> colors = { "red", "green", "blue" };

				//find end of color
				int startOfColorIndex = charIndex;

				charIndex += 3; //since shortest color is red
				for (; charIndex <= round.size(); charIndex++) {
					auto currentSubString = round.substr(startOfColorIndex, charIndex - startOfColorIndex);

					auto res = std::find(colors.begin(), colors.end(), currentSubString);
					if (res != colors.end()) {

						std::cout << " [color:] " << *res;
						//found color
						highestPerColor[*res] = std::max(digit, highestPerColor[*res]);

						charIndex++;
						break;
					}
				}
				std::cout << std::endl;
			}
		}
		int gameResult = 1;
		for (auto& [c, highest] : highestPerColor) {
			gameResult *= highest;
		}

		result += gameResult;
	}

	std::cout << result << std::endl;
}