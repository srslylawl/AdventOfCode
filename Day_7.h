#pragma once

#include "Utils.h"
#include <unordered_map>
#include <vector>
#include <algorithm>


enum class hand_type {
	five_of_a_kind = 0,
	four_of_a_kind = 1,
	full_house = 3,
	three_of_a_kind = 4,
	two_pair = 5,
	one_pair = 6,
	high_card = 7
};

std::ostream& operator<< (std::ostream& os, hand_type ht) {
	switch (ht) {
		case hand_type::five_of_a_kind: return os << "five of a kind";
		case hand_type::full_house: return os << "full house";
		case hand_type::four_of_a_kind: return os << "four of a kind";
		case hand_type::three_of_a_kind: return os << "three of a kind";
		case hand_type::two_pair: return os << "two pair";
		case hand_type::one_pair: return os << "one pair";
		case hand_type::high_card: return os << "high card";
			// omit default case to trigger compiler warning for missing cases
	};
	return os << static_cast<std::uint16_t>(ht);
}

#define htTS(ht) switch((int)ht)

struct hand {
	char cards[5];
	hand_type type;
	int value;


};

bool isAWeaker(const hand& a, const hand& b) {
	if ((int)b.type > (int)a.type) {
		return false;
	}

	if ((int)b.type < (int)a.type) {
		return true;
	}

	const std::unordered_map<char, int> cardValue = {
		{'J', 0},{'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5},
		{'7', 6},{'8', 7}, {'9', 8}, {'T', 9},{'Q', 11}, {'K', 12}, {'A', 13}
	};

	for (int i = 0; i < 5; i++) {
		char bC = b.cards[i];
		char aC = a.cards[i];
		int cardValueB = cardValue.find(bC)->second;
		int cardValueA = cardValue.find(aC)->second;
		if (cardValueB == cardValueA) continue;

		return cardValueB > cardValueA;
	}

	return false;
}

hand_type GetHandType(std::string& handString) {
	std::unordered_map<char, int> cardCounts;

	int jokerCount = 0;
	for (int i = 0; i < handString.size(); i++) {
		char currentCard = handString[i];
		if (currentCard == 'J') jokerCount++;

		if (cardCounts.find(currentCard) != cardCounts.end()) {
			cardCounts[currentCard]++;
		}
		else {
			cardCounts[currentCard] = 1;
		}
	}

	int uniques = cardCounts.size();
	int mostOfNonJokers = 0;
	int pairs = 0;
	int threes = 0;
	for (auto it = cardCounts.begin(); it != cardCounts.end(); it++) {
		if (it->first != 'J') {
			if (it->second > mostOfNonJokers) {
				mostOfNonJokers = it->second;
			}

			if (it->second == 2) pairs++;
			if (it->second == 3) threes++;
		}
	}
	int mostOfOneCard = mostOfNonJokers + jokerCount;

	switch (mostOfOneCard) {
		case 5:
			return hand_type::five_of_a_kind;
		case 4:
			return hand_type::four_of_a_kind;
		case 3:
		{
			if (jokerCount == 0) {
				return pairs == 1 && threes == 1 ? hand_type::full_house : hand_type::three_of_a_kind;
			}

			if (pairs == 2)
				return hand_type::full_house;


			return hand_type::three_of_a_kind;
		}
		case 2:
		{
			//could be pair or 2 pair
			if (pairs == 2) return hand_type::two_pair;
			return hand_type::one_pair;
		}
		case 1:
		{
			return hand_type::high_card;
		}
	}
	return hand_type::high_card;
}

void DoDay7() {
	auto lines = GetLines("input/day7.txt");

	std::vector<hand> hands;
	hands.reserve(lines.size());

	for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
		std::string currentLine = lines[lineIndex];

		std::string handStr = currentLine.substr(0, 5);
		std::string valueStr = currentLine.substr(6, currentLine.size() - 5);

		int value = ParseDigitFromString(valueStr);

		hand h;

		h.type = GetHandType(handStr);
		for (int i = 0; i < 5; i++) {
			h.cards[i] = handStr[i];
		}
		h.value = value;
		hands.push_back(h);
	}

	std::sort(hands.begin(), hands.end(), isAWeaker);

	int result = 0;
	for (int i = 0; i < hands.size(); i++) {
		std::cout << hands[i].cards << " " << hands[i].type << " " << hands[i].value << std::endl;
		result += hands[i].value * (i + 1);
	}

	std::cout << result << std::endl;
}