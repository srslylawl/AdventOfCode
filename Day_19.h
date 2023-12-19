#pragma once
#include "Utils.h"


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
		currentIndex += res1.size() +1;
	}

	std::function<std::string(machinepart)> ifFalse;

	std::string res2;
	if (IsFunction(input, currentIndex, res2)) {
		ifFalse = ParseFunction(input, currentIndex, currentIndex);
	}
	else {
		ifFalse = [res2](machinepart) {return res2; };
		currentIndex += res2.size()+1;
	}

	endIndex = currentIndex;

	return [ifTrue, ifFalse, partIndex, compareValue, isGreater](machinepart part) {
		if (isGreater) {
			return part[partIndex] > compareValue ? ifTrue(part) : ifFalse(part);
		}

		return part[partIndex] < compareValue ? ifTrue(part) : ifFalse(part);
	};
}

void DoDay19() {
	auto lines = GetLines("input/day19.txt");

	std::vector<workflow> workFlows;

	std::vector<machinepart> parts;


	workflow in_workflow;

	for (size_t i = 0; i < lines.size(); i++) {
		int charIndex = 0;

		if (lines[i].size() < 2) continue;

		if (lines[i][0] == '{') {
			machinepart part;

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

			parts.push_back(part);
		}
		else {
			workflow newFlow;
			for (; charIndex < lines[i].size(); charIndex++) {
				char currentChar = lines[i][charIndex];
				if (currentChar == '{') {
					newFlow.name = lines[i].substr(0, charIndex);
					int endIndex = 0;
					newFlow.evaluation = ParseFunction(lines[i], charIndex + 1, endIndex);
					continue;
				}
			}
			workFlows.push_back(newFlow);

			if (newFlow.name == "in") {
				in_workflow = newFlow;
			}
		}
	}


	int intRes = 0;

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

	std::cout << intRes << std::endl;
}