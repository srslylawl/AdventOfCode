#pragma once

#include "Utils.h"
#include <windows.h>

struct coord {
	int x, y;
};

coord translate(coord start, char pipe, coord pipePos) {
	coord end = start;
	switch (pipe) {
		case '|':
		{
			if (start.x == pipePos.x) {
				if (start.y > pipePos.y) {
					end.y = pipePos.y - 1;
				}
				else {
					end.y = pipePos.y + 1;
				}
			}
		}
		break;
		case '-':
		{
			if (start.y == pipePos.y) {
				if (start.x < pipePos.x) {
					end.x = pipePos.x + 1;
				}
				else {
					end.x = pipePos.x - 1;
				}
			}
		}
		break;
		case 'L':
		{
			if (start.y == pipePos.y + 1 && start.x == pipePos.x) {
				end.y = pipePos.y;
				end.x = pipePos.x + 1;
			}
			else if (start.y == pipePos.y && start.x == pipePos.x + 1) {
				end.y = pipePos.y + 1;
				end.x = pipePos.x;
			}
		}
		break;
		case 'J':
		{
			if (start.y == pipePos.y + 1 && start.x == pipePos.x) {
				end.y = pipePos.y;
				end.x = pipePos.x - 1;
			}
			else if (start.y == pipePos.y && start.x == pipePos.x - 1) {
				end.y = pipePos.y + 1;
				end.x = pipePos.x;
			}
		}
		break;
		case '7':
		{
			if (start.y < pipePos.y && start.x == pipePos.x) {
				end.y = pipePos.y;
				end.x = pipePos.x - 1;
			}
			else if (start.y == pipePos.y && start.x == pipePos.x - 1) {
				end.y = pipePos.y - 1;
				end.x = pipePos.x;
			}
		}
		break;
		case 'F':
		{
			if (start.y < pipePos.y && pipePos.x == start.x) {
				end.y = pipePos.y;
				end.x = pipePos.x + 1;
			}
			else if (start.y == pipePos.y && start.x == pipePos.x + 1) {
				end.y = pipePos.y - 1;
				end.x = pipePos.x;
			}
		}
		break;
		default:
			break;
	}

	return end;
}

char GetCharAtCoord(const std::vector<std::string>& lines, coord c) {
	return lines[lines.size() - c.y - 1][c.x];
}

void encapsulate(coord c, coord* bottomLeft, coord* topRight) {
	if (c.x < bottomLeft->x) {
		bottomLeft->x = c.x;
	}

	if (c.x > topRight->x) {
		topRight->x = c.x;
	}

	if (c.y < bottomLeft->y) {
		bottomLeft->y = c.y;
	}

	if (c.y > topRight->y) {
		topRight->y = c.y;
	}
}

void printMap(const std::vector<std::string>& map) {
	for (int y = 0; y < map.size(); y++) {
		std::cout << map[y] << std::endl;
	}
}

struct pipe {
	coord pos;
	char symbol;
};

void DoDay10() {
	auto lines = GetLines("input/day10.txt");

	coord sPos;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			if (lines[i][j] == 'S') {
				sPos.x = j;
				sPos.y = lines.size() - i - 1;
				break;
			}
		}
	}

	coord startPos = sPos;

	//find first pipe

	coord currentPos = startPos;


	pipe start;
	start.symbol = 'S';
	start.pos = startPos;

	bool found = false;
	for (int x = -1; x <= 1; x++) {
		if (found) break;
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) continue;

			coord check;
			check.x = startPos.x + x;
			check.y = startPos.y + y;

			if (check.x < 0 || check.y < 0) continue;

			char c = GetCharAtCoord(lines, check);
			coord translated = translate(startPos, c, check);

			if (startPos.x != translated.x || startPos.y != translated.y) {
				currentPos = check;
				start.symbol = c;
				found = true;
				break;
			}
		}
	}

	int steps = 1;

	coord bottomLeft;
	bottomLeft.x = startPos.x;
	bottomLeft.y = startPos.y;

	coord topRight;
	topRight.x = startPos.x;
	topRight.y = startPos.y;

	encapsulate(currentPos, &bottomLeft, &topRight);



	pipe current;
	current.symbol = GetCharAtCoord(lines, currentPos);
	current.pos = currentPos;
	std::vector<pipe> loopCoordinates{ start, current };
	std::vector<std::string> newMap = lines;

	coord previousPos = startPos;

	while (true) {
		steps++;
		coord tmp_previousPos = currentPos;
		char currentSymbol = GetCharAtCoord(lines, currentPos);
		currentPos = translate(previousPos, currentSymbol, currentPos);
		previousPos = tmp_previousPos;

		encapsulate(currentPos, &bottomLeft, &topRight);

		if (currentPos.x == startPos.x && currentPos.y == startPos.y) {
			break;
		}

		pipe p;
		p.symbol = GetCharAtCoord(lines, currentPos);;
		p.pos = currentPos;
		loopCoordinates.push_back(p);
	}

	std::cout << "looped in steps: " << steps / 2 << std::endl;

	for (int y = 0; y < lines.size(); y++) {
		for (int x = 0; x < lines[y].size(); x++) {
			newMap[y][x] = ' ';
		}
	}

	for (auto c : loopCoordinates) {
		int flippedY = lines.size() - c.pos.y - 1;
		newMap[flippedY][c.pos.x] = c.symbol;
	}

	printMap(newMap);


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED);
	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

	int insides = 0;
	for (int y = 0; y < newMap.size(); y++) { //left to right
		auto& line = newMap[y];
		int edgeCrossed = 0;
		for (int x = 0; x < line.size(); x++) {
			char curr = line[x];

			if (curr != ' ') {
				if (curr == '|' || curr == '7' || curr == 'F') edgeCrossed++; //| J and L work too
			}

			if (curr != ' ') {
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_GREEN);
				std::cout << curr;
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else {
				bool outside = edgeCrossed % 2 == 0;
				if (!outside) {
					insides++;
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED);
					std::cout << line[x];
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
				}
				else {
					std::cout << curr;
				}
			}
		}
		std::cout << std::endl;
	}

	std::cout << insides << std::endl;



	//printMap(newMap);

}