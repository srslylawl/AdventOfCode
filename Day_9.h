#pragma once

#include "Utils.h"


void DoDay9() {
	auto lines = GetLines("input/day9.txt");

	long long result = 0;
	for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
		std::vector<std::vector<long long>> numberCollection;
		numberCollection.push_back(GetLongSignedNumbersFromString(lines[lineIndex]));


		int iteration = 1;
		while (true) {
			const std::vector<long long>& last = numberCollection[iteration - 1];
			const int lastSize = last.size();
			std::vector<long long> current;

			bool allZeroes = true;
			for (size_t i = 0; i < lastSize - 1; i++) {
				long long difference = last[i + 1] - last[i];
				if (difference != 0) {
					allZeroes = false;
				}
				current.push_back(difference);
			}

			numberCollection.push_back(current);

			if (allZeroes) break;

			iteration++;
		}

		long long extrapolatedSum = 0;
		for (int i = numberCollection.size() - 2; i >= 0; i--) {
			auto& current = numberCollection[i];
			auto& previous = numberCollection[i + 1];
			long long currentLast = current[current.size() - 1];
			long long previousLast = previous[previous.size() - 1];

			long long next = currentLast + previousLast;

			current.push_back(next);
			for (size_t j = 0; j < i + 1; j++) {
				std::cout << " ";
			}

			for (auto n : current) {
				std::cout << n << " ";
			}

			std::cout << std::endl;
			if (i == 0) {
				extrapolatedSum += next;
			}
		}

		std::cout << " total: " << extrapolatedSum << std::endl;

		result += extrapolatedSum;
	}

	std::cout << result << std::endl;
}

void DoDay9_2() {
	auto lines = GetLines("input/day9.txt");

	long long result = 0;
	for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
		std::vector<std::vector<long long>> numberCollection;
		numberCollection.push_back(GetLongSignedNumbersFromString(lines[lineIndex]));


		int iteration = 1;
		while (true) {
			const std::vector<long long>& last = numberCollection[iteration - 1];
			const int lastSize = last.size();
			std::vector<long long> current;

			bool allZeroes = true;
			for (size_t i = 0; i < lastSize - 1; i++) {
				long long difference = last[i + 1] - last[i];
				if (difference != 0) {
					allZeroes = false;
				}
				current.push_back(difference);
			}

			numberCollection.push_back(current);

			if (allZeroes) break;

			iteration++;
		}

		long long extrapolatedSum = 0;
		for (int i = numberCollection.size() - 2; i >= 0; i--) {
			auto& current = numberCollection[i];
			auto& previous = numberCollection[i + 1];
			long long currentFirst = current[0];
			long long previousFirst = previous[0];

			long long next = currentFirst - previousFirst;

			current.insert(current.begin(), next);
			//for (size_t j = 0; j < i + 1; j++) {
			//	std::cout << " ";
			//}

			//for (auto n : current) {
			//	std::cout << n << " ";
			//}

			//std::cout << std::endl;
			if (i == 0) {
				extrapolatedSum += next;
			}
		}

		std::cout << " total: " << extrapolatedSum << std::endl;

		result += extrapolatedSum;
	}

	std::cout << result << std::endl;
}