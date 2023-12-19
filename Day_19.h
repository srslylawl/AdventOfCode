#pragma once
#include "Utils.h"




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

	long long length() {
		return 1+end - begin;
	}
};

struct machinepart {
	int x, m, a, s;

	int& operator[](int index) {
		switch (index) {
			case 0:
				return x;
			case 1:
				return m;
			case 2: return a;
			case 3: return s;

			default:
				throw;
		}
	}
};

struct workflow {
	std::string name;

	std::function<std::string(machinepart)> evaluation;
};

struct workflowpt2 {
	std::string name;

	std::function<long long(std::vector<range>)> evaluation;
};

std::vector<workflow> workFlows;
std::vector<workflowpt2> workFlowsPt2;

bool IsFunction(std::string input, int startParseIndex, std::string& result) {
	for (size_t i = startParseIndex; i < input.size(); i++) {
		char c = input[i];

		if (c == ',' || c == '}') {
			result = input.substr(startParseIndex, i - startParseIndex);
			return false;
		}
		if (c == '>' || c == '<') return true;
	}

	throw;
}

std::function<std::string(machinepart)> ParseFunction(std::string input, int startIndex, int& endIndex) {
	char valueToCheck = input[startIndex];

	int partIndex = 0;
	switch (valueToCheck) {
		case 'x':
			partIndex = 0;
			break;
		case 'm':
			partIndex = 1;
			break;
		case 'a':
			partIndex = 2;
			break;
		case 's':
			partIndex = 3;
			break;
		default:
			throw;
	}
	bool isGreater = input[startIndex + 1] == '>';
	int compareValue = 0;
	int currentIndex = startIndex + 2;
	for (size_t i = startIndex + 2; i < input.length(); i++) {
		if (input[i] == ':') {
			compareValue = ParseDigitFromString(input.substr(currentIndex, i - currentIndex));
			currentIndex = i + 1;
			break;
		}
	}

	std::function<std::string(machinepart)> ifTrue;

	//returnvalue 1

	std::string res1;
	if (IsFunction(input, currentIndex, res1)) {
		ifTrue = ParseFunction(input, currentIndex, currentIndex);
	}
	else {
		ifTrue = [res1](machinepart) {return res1; };
		currentIndex += res1.size() + 1;
	}

	std::function<std::string(machinepart)> ifFalse;

	std::string res2;
	if (IsFunction(input, currentIndex, res2)) {
		ifFalse = ParseFunction(input, currentIndex, currentIndex);
	}
	else {
		ifFalse = [res2](machinepart) {return res2; };
		currentIndex += res2.size() + 1;
	}

	endIndex = currentIndex;

	return [ifTrue, ifFalse, partIndex, compareValue, isGreater](machinepart part) {
		if (isGreater) {
			return part[partIndex] > compareValue ? ifTrue(part) : ifFalse(part);
		}

		return part[partIndex] < compareValue ? ifTrue(part) : ifFalse(part);
	};
}

std::function<long long(std::vector<range>)> ParseFunctionPart2(std::string input, int startIndex, int& endIndex) {
	char valueToCheck = input[startIndex];

	int partIndex = 0;
	switch (valueToCheck) {
		case 'x':
			partIndex = 0;
			break;
		case 'm':
			partIndex = 1;
			break;
		case 'a':
			partIndex = 2;
			break;
		case 's':
			partIndex = 3;
			break;
		default:
			throw;
	}
	bool isGreater = input[startIndex + 1] == '>';
	int compareValue = 0;
	int currentIndex = startIndex + 2;
	for (size_t i = startIndex + 2; i < input.length(); i++) {
		if (input[i] == ':') {
			compareValue = ParseDigitFromString(input.substr(currentIndex, i - currentIndex));
			currentIndex = i + 1;
			break;
		}
	}

	std::function<long long(std::vector<range>)> ifTrue;

	std::string res1;
	if (IsFunction(input, currentIndex, res1)) {
		ifTrue = ParseFunctionPart2(input, currentIndex, currentIndex);
	}
	else {
		currentIndex += res1.size() + 1;
	}

	std::function<long long(std::vector<range>)> ifFalse;

	std::string res2;
	if (IsFunction(input, currentIndex, res2)) {
		ifFalse = ParseFunctionPart2(input, currentIndex, currentIndex);
	}
	else {
		currentIndex += res2.size() + 1;
	}

	endIndex = currentIndex;

	return [res1, res2, ifTrue, ifFalse, partIndex, compareValue, isGreater](std::vector<range> ranges) -> long long {
		//we have a vector of ranges and will split it in two based on our result
		std::vector<range> ranges_true = ranges;
		std::vector<range> ranges_false = ranges;

		if (isGreater) {
			ranges_true[partIndex].begin = compareValue + 1;
			ranges_false[partIndex].end = compareValue;
		}
		else {
			ranges_true[partIndex].end = compareValue - 1;
			ranges_false[partIndex].begin = compareValue;
		}

		long long result_true = 0;

		if (res1 == "A") {
			result_true = ranges_true[0].length() * ranges_true[1].length() * ranges_true[2].length() * ranges_true[3].length();
		}
		else if (res1 == "R") {
			result_true = 0;
		}
		else if (res1 != "") {
			//send to next workflow
			auto nextWorkFlow = std::find_if(workFlowsPt2.begin(), workFlowsPt2.end(), [res1](workflowpt2& cmp) {
				return cmp.name == res1;
			});

			if (nextWorkFlow == workFlowsPt2.end()) {
				throw;
			}
			result_true = nextWorkFlow->evaluation(ranges_true);
		}
		else {
			result_true = ifTrue(ranges_true);
		}

		long long result_false = 0;

		if (res2 == "A") {
			result_false = ranges_false[0].length() * ranges_false[1].length() * ranges_false[2].length() * ranges_false[3].length();
		}
		else if (res2 == "R") {
			result_false = 0;
		}
		else if (res2 != "") {
			//send to next workflow
			auto nextWorkFlow = std::find_if(workFlowsPt2.begin(), workFlowsPt2.end(), [res2](workflowpt2& cmp) {
				return cmp.name == res2;
			});

			if (nextWorkFlow == workFlowsPt2.end()) {
				throw;
			}
			result_false = nextWorkFlow->evaluation(ranges_false);
		}
		else {
			result_false = ifFalse(ranges_false);
		}


		return result_false + result_true;
	};
}

void DoDay19() {
	auto lines = GetLines("input/day19.txt");



	std::vector<machinepart> parts;


	//workflow in_workflow;

	workflowpt2 in_workflowp2;

	for (size_t i = 0; i < lines.size(); i++) {
		int charIndex = 0;

		if (lines[i].size() < 2) continue;

		if (lines[i][0] == '{') {
			/*machinepart part;

			charIndex = 3;
			int startIndex = charIndex;
			for (; charIndex < lines[i].size(); charIndex++) {
				char c = lines[i][charIndex];
				if (c == ',') {
					part.x = ParseDigitFromString(lines[i].substr(startIndex, charIndex - startIndex));
					charIndex += 3;
					break;
				}
			}

			startIndex = charIndex;
			for (; charIndex < lines[i].size(); charIndex++) {
				char c = lines[i][charIndex];
				if (c == ',') {
					part.m = ParseDigitFromString(lines[i].substr(startIndex, charIndex - startIndex));
					charIndex += 3;
					break;
				}
			}

			startIndex = charIndex;
			for (; charIndex < lines[i].size(); charIndex++) {
				char c = lines[i][charIndex];
				if (c == ',') {
					part.a = ParseDigitFromString(lines[i].substr(startIndex, charIndex - startIndex));
					charIndex += 3;
					break;
				}
			}

			startIndex = charIndex;
			for (; charIndex < lines[i].size(); charIndex++) {
				char c = lines[i][charIndex];
				if (c == '}') {
					part.s = ParseDigitFromString(lines[i].substr(startIndex, charIndex - startIndex));
					charIndex += 3;
					break;
				}
			}

			parts.push_back(part);*/
		}
		else {
			workflowpt2 newFlow;
			for (; charIndex < lines[i].size(); charIndex++) {
				char currentChar = lines[i][charIndex];
				if (currentChar == '{') {
					newFlow.name = lines[i].substr(0, charIndex);
					int endIndex = 0;
					newFlow.evaluation = ParseFunctionPart2(lines[i], charIndex + 1, endIndex);
					continue;
				}
			}
			workFlowsPt2.push_back(newFlow);

			if (newFlow.name == "in") {
				in_workflowp2 = newFlow;
			}
		}
	}


	long long endRes = 0;

	//part 1
	//for (auto& part : parts) {
	//	std::string result;
	//	workflow* next = &in_workflow;

	//	std::cout << "Part: " << part.x << std::endl;
	//	while (true) {
	//		std::cout << "eval wf: " << next->name << std::endl;

	//		result = next->evaluation(part);
	//		if (result == "A") {
	//			intRes += part.x + part.m + part.a + part.s;
	//			std::cout << "ACCEPTED!" << std::endl;
	//			break;
	//		}
	//		else if (result == "R") {
	//			std::cout << "REJECTED!" << std::endl;
	//			break;
	//		}

	//		auto nextWorkFlow = std::find_if(workFlows.begin(), workFlows.end(), [result](workflow& cmp) {
	//			return cmp.name == result;
	//		});

	//		if(nextWorkFlow == workFlows.end()) {
	//			throw;
	//		}
	//		next = &*nextWorkFlow;
	//	}
	//}

	std::vector<range> ranges = { {1, 4000}, {1, 4000}, {1, 4000}, {1, 4000} };

	endRes = in_workflowp2.evaluation(ranges);

	std::cout << endRes << std::endl;
}