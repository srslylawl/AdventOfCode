#pragma once
#include "Utils.h"
#include <map>
#include <functional>


struct range {
	long long begin;
	long long end;

	range(long long b, long long e) : begin(b), end(e) {
	}

	range() : begin(0), end(0) {
	}

	bool operator==(const range& other) {
		return begin == other.begin && end == other.end;
	}
};

#define rangeTS(r) r.begin << "-" << r.end


long long GetSum(std::vector<range>& input) {
	long long res = 0;
	for (auto& n : input) {
		res += (n.end - n.begin + 1);
	};

	return res;

}

std::vector<range> RemapRange(range input, range source, range destination) {
	std::vector<range> result;
	if (input.end < source.begin) {
		//std::cout << rangeTS(sourceRange) << " does not include " << rangeTS(input) << std::endl;
		result.push_back(input);
		return result; //doesnt overlap
	}
	if (input.begin > source.end) {
		//std::cout << rangeTS(sourceRange) << " does not include " << rangeTS(input) << std::endl;
		result.push_back(input);
		return result; //doesnt overlap
	}
	auto inputLength = input.end - input.begin + 1;

	//check what range we overlap
	long long offset = destination.begin - source.begin;
	long long overlapBegin = std::max(input.begin, source.begin);
	long long overlapEnd = std::min(input.end, source.end);

	range overlap;
	overlap.begin = overlapBegin;
	overlap.end = overlapEnd;

	range overlapRemapped;
	overlapRemapped.begin = overlapBegin + offset;
	overlapRemapped.end = overlapEnd + offset;

	if (overlapRemapped.begin < 0 || overlapRemapped.end < 0) {
		std::cout << "uhhh" << std::endl;
	}

	result.push_back(overlapRemapped);
	//std::cout << rangeTS(sourceRange) << " includes " << rangeTS(input) << std::endl;

	//if we dont encapsulate, include ends
	if (!(input.begin >= source.begin && input.end <= source.end)) {
		long long smallest = std::min(input.begin, source.begin);
		if (input.begin < overlapBegin) {
			range low;
			low.begin = input.begin;
			low.end = overlapBegin - 1;

			//std::cout << rangeTS(low) << " = lower bound" << std::endl;
			result.push_back(low);
		}
		if (input.end > overlapEnd) {
			range high;
			high.begin = overlapEnd + 1;
			high.end = input.end;
			//std::cout << rangeTS(high) << " = upper bound" << std::endl;
			result.push_back(high);
		}
	}
	//std::cout << rangeTS(overlap) << " remaps to " << rangeTS(overlapRemapped) << std::endl;
	auto resultSum = GetSum(result);
	assert(inputLength == resultSum);

	return result;
}

void DoDay5() {
	auto source = range(45, 50);
	auto dest = range(145, 150); //basically +100

	auto seed1 = range(50, 60); //overlaps only on 50 (upper edge), upper stays same
	auto res1 = RemapRange(seed1, source, dest);
	assert(res1[0] == range(150, 150));
	assert(res1[1] == range(51, 60));
	assert(res1.size() == 2);

	auto seed2 = range(45, 50); //seed completely encapsulated by source
	auto res2 = RemapRange(seed2, source, dest);
	assert(res2[0] == range(145, 150));
	assert(res2.size() == 1);

	auto seed3 = range(40, 60); //seed encapsulates source and has a remapped part as well as upper and lower bounds
	auto res3 = RemapRange(seed3, source, dest);
	assert(res3[0] == range(145, 150));
	assert(res3[1] == range(40, 44));
	assert(res3[2] == range(51, 60));
	assert(res3.size() == 3);

	auto seed4 = range(40, 47); //seed overlaps on lower part of source
	auto res4 = RemapRange(seed4, source, dest);
	assert(res4[0] == range(145, 147));
	assert(res4[1] == range(40, 44));
	assert(res4.size() == 2);

	auto seed5 = range(47, 55); //seed overlaps on upper part of source
	auto res5 = RemapRange(seed5, source, dest);
	assert(res5[0] == range(147, 150));
	assert(res5[1] == range(51, 55));
	assert(res5.size() == 2);





	auto lines = GetLines("input/day5.txt");


	std::vector<std::vector<std::function<std::vector<range>(range, bool*)>>> maps;
	maps.resize(7);

	const std::string beginning = "seeds: ";
	const std::string seedStr = lines[0].substr(beginning.size(), lines[0].size() - beginning.size());

	std::vector<long long> seeds = GetLongNumbersFromString(seedStr);

	std::vector<range> seedRanges;

	std::cout << "Seed Ranges: ";

	for (long long i = 0; i < seeds.size(); i += 2) {
		range r;
		r.begin = seeds[i];
		r.end = seeds[i] + seeds[i + 1];
		std::cout << r.begin << "-" << r.end << std::endl;
		seedRanges.push_back(r);
	}


	int currentLine = 3;
	for (int mapIndex = 0; mapIndex < 7; mapIndex++) {
		//std::cout << "MapIndex: ";
		for (; currentLine < lines.size(); currentLine++) {
			char firstChar = lines[currentLine][0];
			if (!CharIsDigit(firstChar)) {
				currentLine += 2;
				break;
			}
			std::vector<long long> numbers = GetLongNumbersFromString(lines[currentLine]);

			//std::cout << " nums: " << numbers[0] << "-" << numbers[1] << " len: " << numbers[2] << std::endl;

			long long totalRange = numbers[2];
			range sourceRange;
			sourceRange.begin = numbers[1];
			sourceRange.end = sourceRange.begin + totalRange;
			range destRange;
			destRange.begin = numbers[0];
			destRange.end = destRange.begin + totalRange;

			maps[mapIndex].push_back([sourceRange, destRange](range input, bool* includes) -> std::vector<range> {
				*includes = false;
				std::vector<range> result;
				if (input.end < sourceRange.begin) {
					//std::cout << rangeTS(sourceRange) << " does not include " << rangeTS(input) << std::endl;
					result.push_back(input);
					return result; //doesnt overlap
				}
				if (input.begin > sourceRange.end) {
					//std::cout << rangeTS(sourceRange) << " does not include " << rangeTS(input) << std::endl;
					result.push_back(input);
					return result; //doesnt overlap
				}
				*includes = true;
				auto inputLength = input.end - input.begin + 1;

				//check what range we overlap
				long long offset = destRange.begin - sourceRange.begin;
				long long overlapBegin = std::max(input.begin, sourceRange.begin);
				long long overlapEnd = std::min(input.end, sourceRange.end);

				range overlap;
				overlap.begin = overlapBegin;
				overlap.end = overlapEnd;

				range overlapRemapped;
				overlapRemapped.begin = overlapBegin + offset;
				overlapRemapped.end = overlapEnd + offset;

				if (overlapRemapped.begin < 0 || overlapRemapped.end < 0) {
					std::cout << "uhhh" << std::endl;
				}

				result.push_back(overlapRemapped);
				//std::cout << rangeTS(sourceRange) << " includes " << rangeTS(input) << std::endl;

				//if we dont encapsulate, include ends
				if (!(input.begin >= sourceRange.begin && input.end <= sourceRange.end)) {
					long long smallest = std::min(input.begin, sourceRange.begin);
					if (input.begin < overlapBegin) {
						range low;
						low.begin = input.begin;
						low.end = overlapBegin - 1;

						//std::cout << rangeTS(low) << " = lower bound" << std::endl;
						result.push_back(low);
					}
					if (input.end > overlapEnd) {
						range high;
						high.begin = overlapEnd + 1;
						high.end = input.end;
						//std::cout << rangeTS(high) << " = upper bound" << std::endl;
						result.push_back(high);
					}
				}
				//std::cout << rangeTS(overlap) << " remaps to " << rangeTS(overlapRemapped) << std::endl;
				auto resultSum = GetSum(result);
				assert(inputLength == resultSum);

				return result;
			});


			//maps[mapIndex].push_back([sourceRangeStart, destRangeStart, sourceRangeEnd](size_t input, bool* isInRange) -> size_t {
			//	size_t result = input;
			//	*isInRange = false;
			//	if (input >= sourceRangeStart && input <= sourceRangeEnd) {
			//		*isInRange = true;
			//		result += destRangeStart - sourceRangeStart;
			//	}

			//	return result;
			//});
		}
	}

	long long lowestSeed = LONG_LONG_MAX;

	std::vector<range> rangesToCheck = seedRanges;
	std::vector<range> nextLevelRanges;



	long long sum = GetSum(rangesToCheck);

	for (int mapIndex = 0; mapIndex < 7; mapIndex++) {
		//std::cout << "mapindex " << mapIndex << std::endl;
		//check each map

		int rangesCount = rangesToCheck.size();
		std::cout << "RangesCount: " << rangesCount << std::endl;
		for (int rangeIndex = 0; rangeIndex < rangesToCheck.size(); rangeIndex++) {
			bool includes;
			bool includesAtLeastOnce = false;

			for (int mapFunctionIndex = 0; mapFunctionIndex < maps[mapIndex].size(); mapFunctionIndex++) {
				//std::cout << "functionIndex " << mapFunctionIndex << std::endl;
				//each map has an array of functions that spit out new ranges
				std::vector<range> res = maps[mapIndex][mapFunctionIndex](rangesToCheck[rangeIndex], &includes);
				//if no map includes our range, we add our range to the next level
				//if it does, we include the remapped values
				if (includes) {
					assert(!includesAtLeastOnce);
					includesAtLeastOnce = true;
					for (size_t i = 0; i < res.size(); i++) {
						if (i == 0) {
							nextLevelRanges.push_back(res[i]);
						}
						else {
							rangesToCheck.push_back(res[i]);
							//rangesCount++;
						}
					}
				}
			}

			if (!includesAtLeastOnce) {
				std::cout << rangeTS(rangesToCheck[rangeIndex]) << " not included in current map, passing on directly to next map." << std::endl;
				//
				nextLevelRanges.push_back(rangesToCheck[rangeIndex]);
			}
		}

		rangesToCheck = nextLevelRanges;
		nextLevelRanges.clear();
	}

	for (auto ran : rangesToCheck) {
		if (ran.begin < lowestSeed) {
			lowestSeed = ran.begin;
			std::cout << "current smallest: " << lowestSeed << std::endl;
		}
	}

	std::cout << "lowest: " << lowestSeed << std::endl;


	//std::map<int, int> seedToSoil;
	//std::map<int, int> soilToFert;
	//std::map<int, int> fertToWater;
	//std::map<int, int> waterToLight;
	//std::map<int, int> lightToTemp;
	//std::map<int, int> tempToHumid;
	//std::map<int, int> humidToLocation;

}