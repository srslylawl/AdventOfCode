#pragma once
#include <queue>

#include "Utils.h"
#include <unordered_map>
#include <functional>


long ctLowPulses = 0;
long ctHighPulses = 0;
long amountCurrentButtonPresses = 0;
bool rxActivated = false;
struct module;

std::unordered_map<std::string, module*> modules;

std::queue<std::function<void()>> operationQueue;

struct module {
	std::vector<std::string> connectedTo;
	std::string name;

	virtual void ReceiveInput(bool low, std::string& from) = 0;

	virtual void Output(bool low) {
		int count = connectedTo.size();

		//std::cout << name << " sends " << count << (low ? " low" : " high") << " outputs" << std::endl;

		if (low) ctLowPulses += count;
		else ctHighPulses += count;

		operationQueue.push([low, this]() {
			for (const auto& str : this->connectedTo) {
				if (str == "rx") {
					//std::cout << "rx hit with " << (!low ? "high" : "low! RX ACTIVATED!") << std::endl;
					if (low) {
						rxActivated = true;
						break;
					}

					continue;
				}
				//if (!modules.count(str)) continue;

				auto res = modules.at(str);
				res->ReceiveInput(low, this->name);
			}
		});
	}
};

struct flip_flop : public module {
	bool isOn = false;

	void ReceiveInput(bool low, std::string& from) override {
		if (!low) return;

		if (!isOn) {
			//send high pulse to everything
			Output(false);
		}
		else {
			//send low pulse to everything
			Output(true);
		}

		isOn = !isOn;
	}
};

struct conjunction : module {
	std::unordered_map<std::string, bool> receivedInputs; //true == low input

	void ReceiveInput(bool low, std::string& from) override {
		receivedInputs[from] = low;

		if (name == "ft") {
			if(!low) {

				std::cout << "ft received high from " << from << " at press: " << amountCurrentButtonPresses << std::endl;

				//solution is then the LCM of all 4 inputs of FT
			}
		}

		//if all inputs are high, send low input, else high
		bool sendLow = !hasLowInput();
		Output(sendLow);
	}

	bool hasLowInput() {
		for (auto& kvp : receivedInputs) {
			if (kvp.second) return true;
		}
		return false;
	}
};

struct broadcaster : module {

	void ReceiveInput(bool low, std::string& from) override {
		Output(low);
	}
};

std::vector<std::string> parseConnectedModules(std::string input, int startIndex) {
	std::vector<std::string> result;

	int currentStart = startIndex;
	for (int i = startIndex; i < input.size(); ++i) {
		if (i == input.size() - 1) {
			result.push_back(input.substr(currentStart, i - currentStart + 1));
			break;
		}

		if (input[i] == ',') {
			result.push_back(input.substr(currentStart, i - currentStart));

			currentStart = i + 2;
			i++;
		}
	}


	return result;
}

void DoDay20() {
	auto lines = GetLines("input/day20.txt");

	for (int i = 0; i < lines.size(); ++i) {
		char first = lines[i][0];

		int nameLength = 2;

		if (first == '%') {
			flip_flop* ff = new flip_flop;
			ff->name = lines[i].substr(1, nameLength);
			ff->connectedTo = parseConnectedModules(lines[i], 5 + nameLength);

			std::cout << "Adding flipFlop " << ff->name << " ";
			for (auto s : ff->connectedTo) {
				std::cout << s << " ";
			}
			std::cout << std::endl;
			modules[ff->name] = ff;
		}
		else if (first == '&') {
			conjunction* conjunction = new ::conjunction;
			conjunction->name = lines[i].substr(1, nameLength);
			conjunction->connectedTo = parseConnectedModules(lines[i], 5 + nameLength);

			std::cout << "Adding conjunction " << conjunction->name << " ";
			for (auto s : conjunction->connectedTo) {
				std::cout << s << " ";
			}
			std::cout << std::endl;
			modules[conjunction->name] = conjunction;
		}
		else {
			broadcaster* broadcaster = new ::broadcaster;
			broadcaster->name = "broadcaster";

			const std::string offset = "broadcaster -> ";

			broadcaster->connectedTo = parseConnectedModules(lines[i], offset.size());
			std::cout << "Adding broadcaster " << broadcaster->name << " ";
			for (auto s : broadcaster->connectedTo) {
				std::cout << s << " ";
			}
			std::cout << std::endl;
			modules[broadcaster->name] = broadcaster;
		}
	}

	for (auto mod : modules) {
		for (auto connected : mod.second->connectedTo) {
			if (!modules.count(connected)) continue;

			auto res = modules.at(connected);
			auto conj = dynamic_cast<conjunction*>(res);

			if (conj) {
				conj->receivedInputs[mod.second->name] = true;
			}
		}
	}

	int amtButtonPresses = 1000;

	for (int i = 0; true; ++i) {
		ctLowPulses += 1;
		amountCurrentButtonPresses++;
		//std::cout << "buttonpress " << i + 1 << std::endl;
		modules["broadcaster"]->Output(true);
		while (!operationQueue.empty() && !rxActivated) {
			auto& next = operationQueue.front();
			next();
			operationQueue.pop();
		}

		if (rxActivated) break;
	}

	std::cout << "Low: " << ctLowPulses << " High: " << ctHighPulses << " Total: " << ctHighPulses * ctLowPulses << " ButtonPresses: " << amountCurrentButtonPresses << std::endl;
}