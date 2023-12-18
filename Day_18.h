#pragma once
#include "Utils.h"


void DoDay18() {
	auto lines = GetLines("input/day18.txt");

	std::vector<std::string> dugMap;

	int width = 800;
	int height = 800;

	for (size_t i = 0; i < height; i++) {
		std::string str;
		str.append(width, '.');
		dugMap.push_back(str);
	}

	coord currentPos(0, 0);


	coord boundsMin(width/2, height/2);
	coord boundsMax(0, 0);

	coord actualBoundsMin(0,0);

	int edgeCount = 0;

	std::vector<coord> points {currentPos};

	char previous = 'R';
	for (size_t i = 0; i < lines.size(); i++) {
		char direction = lines[i][0];

		int count = 0;
		for (size_t charIndex = 2; charIndex < lines.size(); charIndex++) {
			if (lines[i][charIndex] == ' ') {
				std::string numStr = lines[i].substr(2, charIndex - 2);
				count = ParseDigitFromString(numStr);
				break;
			}
		}

		edgeCount += count;

		switch (direction) {
			case 'R':
			{
				if(previous == 'D') {
					dugMap[currentPos.y+boundsMin.y][currentPos.x + boundsMin.x] = '#';
				}
				for (size_t len = 0; len < count; len++) {
					dugMap[currentPos.y+boundsMin.y][++currentPos.x+boundsMin.x] = '#';
				}
			}
			break;
			case 'D':
			{
				if(previous == 'R' || previous == 'L') {
					dugMap[currentPos.y+boundsMin.y][currentPos.x + boundsMin.x] = '|';
				}

				for (size_t len = 0; len < count; len++) {
					dugMap[++currentPos.y+boundsMin.y][currentPos.x+boundsMin.x] = '|';
				}
			}
			break;
			case 'U':
			{
				//if(previous == 'R' || previous == 'L') {
				//	dugMap[currentPos.y+boundsMin.y][currentPos.x + boundsMin.x] = '|';
				//}

				for (size_t len = 0; len < count; len++) {
					dugMap[--currentPos.y+boundsMin.y][currentPos.x+boundsMin.x] = '|';
				}
			}
			break;
			case 'L':
			{
				if(previous == 'D') {
					dugMap[currentPos.y+boundsMin.y][currentPos.x + boundsMin.x] = '#';
				}
				for (size_t len = 0; len < count; len++) {
					dugMap[currentPos.y+boundsMin.y][--currentPos.x+boundsMin.x] = '#';
				}
			}
			break;
			default:
				break;
		}

		previous = direction;

		if(currentPos.x >boundsMax.x) {
			boundsMax.x = currentPos.x;
		}

		if(currentPos.x <= actualBoundsMin.x) {
			actualBoundsMin.x = currentPos.x;
		}

		if(currentPos.y > boundsMax.y) {
			boundsMax.y = currentPos.y;
		}

		if(currentPos.y < actualBoundsMin.y) {
			actualBoundsMin.y = currentPos.y;
		}
	}
	int insideCount = 0;
	int topCutoff = boundsMin.y + actualBoundsMin.y;
	int bottomCutOff = height/2 - boundsMax.y;

	int leftCutoff = boundsMin.x + actualBoundsMin.x;
	int rightCutoff = width/2 - boundsMax.x;

	dugMap.erase(dugMap.begin(), dugMap.begin()+topCutoff);
	dugMap.erase(dugMap.end()-bottomCutOff+1, dugMap.end());


	//dugMap.erase(dugMap.begin()+boundsMax.y+boundsMin.y+1-topCutoff, dugMap.end());
	for (int i = 0; i < dugMap.size(); i++) {
		dugMap[i].erase(dugMap[i].begin(), dugMap[i].begin()+leftCutoff);
		dugMap[i].erase(dugMap[i].end()-rightCutoff+1, dugMap[i].end());
		bool inside = false;
		for (int charIndex = 0; charIndex < dugMap[i].size(); charIndex++) {
			char c = dugMap[i][charIndex];
			if(c == '|') {
				inside = !inside;
			}
			else if (c == '.' && inside) {
				insideCount++;
				dugMap[i][charIndex] = 'o';
			}
		}
	}

	PrintMap(dugMap);

	std:: cout << "edge: " << edgeCount << " inside: " << insideCount << " total: " << edgeCount + insideCount << std::endl;

	std::ofstream file;
	file.open("output.txt");
	for (auto& s : dugMap) {
		file << s << std::endl;
	}
	file.close();
}