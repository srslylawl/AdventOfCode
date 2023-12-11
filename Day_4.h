#pragma once
#include <queue>
#include <chrono>
#include <vector>
#include "Utils.h"


void DoDay4() {
	auto lines = GetLines("input/day4.txt");

	int lineIndex = 0;

	int result = 0;
	for (const auto& line : lines) {
		const std::string beginning = "Card 1: ";
		int cardNumber = lineIndex + 1;
		int additionalOffset = log10((double)cardNumber); //add one additional offset for every digit in our game count, 1, 10, 100 etc

		int gameStartCharOffset = beginning.size() + additionalOffset;

		const std::string gameStr = line.substr(gameStartCharOffset, line.size() - gameStartCharOffset);

		//split into winning numbers and playing numbers

		std::string winningNumberStr;
		std::string playNumberStr;

		std::cout << line << std::endl;

		for (int charIndex = 0; charIndex < gameStr.size(); charIndex++) {
			//find | separator

			char c = gameStr[charIndex];
			if (c != '|') continue;

			winningNumberStr = gameStr.substr(0, charIndex - 1);
			playNumberStr = gameStr.substr(charIndex + 1, gameStr.size() - charIndex + 1);
		}

		std::vector<int> winningNumbers = GetNumbersFromString(winningNumberStr);
		std::vector<int> playingNumbers = GetNumbersFromString(playNumberStr);

		std::cout << "Winning numbers: ";

		std::cout << winningNumberStr << std::endl;

		for (auto n : winningNumbers) {
			std::cout << " " << n;
		}

		std::cout << "\n";

		std::cout << "Playing numbers: ";
		std::cout << playNumberStr << std::endl;

		for (auto n : playingNumbers) {
			std::cout << " " << n;
		}
		std::cout << "\n";

		//count matches
		int matches = 0;
		for (int playNumber : playingNumbers) {
			for (int winI = 0; winI < winningNumbers.size(); winI++) {
				if (playNumber == winningNumbers[winI]) matches++;
			}
		}

		int value = matches <= 2 ? matches : pow(2, matches - 1);
		result += value;
		lineIndex++;
	}

	std::cout << result << std::endl;

}

struct card {
	int matches = 0;
	int amount = 1;
};

void DoDay4_2() {

	auto start = std::chrono::high_resolution_clock::now();


	auto lines = GetLines("input/day4.txt");



	card* cardsWithMatches = new card[lines.size()];
	int lineIndex = 0;
	for (const auto& line : lines) {
		const std::string beginning = "Card 1: ";
		int cardNumber = lineIndex + 1;
		int additionalOffset = log10((double)cardNumber); //add one additional offset for every digit in our game count, 1, 10, 100 etc

		int gameStartCharOffset = beginning.size() + additionalOffset;

		const std::string gameStr = line.substr(gameStartCharOffset, line.size() - gameStartCharOffset);

		//split into winning numbers and playing numbers

		std::string winningNumberStr;
		std::string playNumberStr;

		for (int charIndex = 0; charIndex < gameStr.size(); charIndex++) {
			//find | separator

			char c = gameStr[charIndex];
			if (c != '|') continue;

			winningNumberStr = gameStr.substr(0, charIndex - 1);
			playNumberStr = gameStr.substr(charIndex + 1, gameStr.size() - charIndex + 1);
		}

		std::vector<int> winningNumbers = GetNumbersFromString(winningNumberStr);
		std::vector<int> playingNumbers = GetNumbersFromString(playNumberStr);

		//count matches
		int matches = 0;
		for (int playNumber : playingNumbers) {
			for (int winI = 0; winI < winningNumbers.size(); winI++) {
				if (playNumber == winningNumbers[winI]) matches++;
			}
		}

		cardsWithMatches[lineIndex].matches = matches;

		for (int i = 0; i < matches; i++) {
			cardsWithMatches[lineIndex + 1 + i].amount += cardsWithMatches[lineIndex].amount;
		}

		lineIndex++;
	}

	auto endOfParsing = std::chrono::high_resolution_clock::now();

	int cardsResult = 0;
	for (int i = 0; i < lines.size(); i++) {
		const card c = cardsWithMatches[i];
		cardsResult += c.amount;
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << cardsResult << std::endl;

	std::cout << "time spent parsing: " << std::chrono::duration_cast<std::chrono::nanoseconds>(endOfParsing - start).count() / 1e+9 << "s \n";
	std::cout << "elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e+9 << "s \n";

	std::cin.get();
}