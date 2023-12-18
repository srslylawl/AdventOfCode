#pragma once
#include "Utils.h"
#include "sstream"


void DoDay18() {
	auto lines = GetLines("input/day18.txt");

	longCoord currentPos(0, 0);

	long long edgeCount = 0;
	std::vector<longCoord> points{ currentPos };

	for (size_t i = 0; i < lines.size(); i++) {

		int startIndex = 0;
		for (size_t charIndex = 4; charIndex < lines[i].size(); charIndex++) {
			if (lines[i][charIndex] == '#') {
				startIndex = charIndex + 1;
				continue;
			}

			if (lines[i][charIndex] == ')') {
				std::string fullStr = lines[i].substr(startIndex, charIndex - startIndex);

				int direction = ParseDigitFromString(fullStr.substr(fullStr.size() - 1, 1));

				std::string hexCodeDistance = fullStr.substr(0, 5);

				long long ct;
				std::stringstream ss;

				ss << std::hex << hexCodeDistance;
				ss >> ct;

				edgeCount += ct;

				longCoord newPoint = currentPos;

				switch (direction) {
					case 0:
						newPoint.x += ct;
						break;
					case 1:
						newPoint.y += ct;
						break;
					case 2:
						newPoint.x -= ct;
						break;
					case 3:
						newPoint.y -= ct;
						break;
					default:
						break;
				}

				currentPos = newPoint;
				points.push_back(newPoint);
				break;
			}
		}
	}

	long long shoelaceArea = 0;
	for (int i = 0; i < points.size() - 1; ++i) {
		shoelaceArea += points[i].x * points[i + 1].y - points[i].y * points[i + 1].x;
	}


	shoelaceArea /= 2;
	shoelaceArea = abs(shoelaceArea);

	long long interiorPts = shoelaceArea + 1 - (edgeCount / 2);

	long long area = interiorPts + edgeCount;

	std::cout << area << std::endl;
}