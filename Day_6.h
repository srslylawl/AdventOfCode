#pragma once
#include "Utils.h"


void DoDay6() {
	auto lines = GetLines("input/day6.txt");

	const std::string timeBeginning = "Time:      ";
	const std::string distanceBeginning = "Distance:  ";

	const std::string timesStr = lines[0].substr(timeBeginning.size(), lines[0].size() - timeBeginning.size());
	const std::string distancesStr = lines[1].substr(distanceBeginning.size(), lines[1].size() - distanceBeginning.size());

	std::vector<long long> times = GetLongNumbersFromString(timesStr);
	std::vector<long long> distances = GetLongNumbersFromString(distancesStr);

	std::cout << "times : " << times.size() << std::endl;
	std::cout << "distances : " << distances.size() << std::endl;

	int result = 1;
	for (int raceIndex = 0; raceIndex < times.size(); raceIndex++) {
		int raceResult = 0;
		for (int holdTime = times[raceIndex] - 1; holdTime > 0; holdTime--) {
			int raceTime = times[raceIndex] - holdTime;

			int distance = raceTime * holdTime;

			if (distance > distances[raceIndex]) {
				raceResult++;
			}
		}

		result *= raceResult;
	}

	std::cout << result << std::endl;
}

void DoDay6_2() {
	auto lines = GetLines("input/day6.txt");

	const std::string timeBeginning = "Time:      ";
	const std::string distanceBeginning = "Distance:  ";

	const std::string timesStr = lines[0].substr(timeBeginning.size(), lines[0].size() - timeBeginning.size());
	const std::string distancesStr = lines[1].substr(distanceBeginning.size(), lines[1].size() - distanceBeginning.size());

	std::vector<long long> times = GetLongNumbersFromString(timesStr);
	std::vector<long long> distances = GetLongNumbersFromString(distancesStr);

	std::string actualTimeString;
	std::string actualDistanceString;

	for (auto t : times) {
		actualTimeString += t;
	}

	for (auto d : distances) {
		actualDistanceString += std::to_string(d);
	}

	long long time = ParseLongDigitFromString(actualTimeString);
	long long distance = ParseLongDigitFromString(actualDistanceString);


	std::cout << "time: " << time << std::endl;
	std::cout << "distance: " << distance << std::endl;

	int result = 1;
	int raceResult = 0;
	for (long long holdTime = time - 1; holdTime > 0; holdTime--) {
		long long raceTime = time - holdTime;

		long long currentDist = raceTime * holdTime;

		if (currentDist > distance) {
			raceResult++;
		}
	}

	result *= raceResult;

	std::cout << result << std::endl;
}