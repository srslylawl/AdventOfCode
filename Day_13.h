#pragma once

#include "Utils.h"
#include <vector>
#include <algorithm>

struct puzzle {
	std::vector<std::string> puzzle;
	int reflectedRow, reflectedColumn;
};

void DoDay13() {
	auto lines = GetLines("input/day13.txt");

	std::vector<puzzle> puzzles;


	int startIndex = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		auto cLine = lines[i];

		if (cLine.size() == 0 || i == lines.size() - 1) {
			puzzle p;
			p.puzzle.insert(p.puzzle.begin(), lines.begin() + startIndex, lines.begin() + i);
			puzzles.push_back(p);
			startIndex = i + 1;
		}
	}


	long result = 0;

	for (auto& p : puzzles) {
		//check columns
		int width = p.puzzle[0].size();
		for (size_t columnIndex = 1; columnIndex < width; columnIndex++) {
			int maxDepth = std::min(columnIndex, width - columnIndex);

			bool columnIsSymmetric = true;
			int mistakes = 0;
			for (size_t sideOffset = 1; sideOffset <= maxDepth; sideOffset++) {
				if (!columnIsSymmetric)  break;
				for (size_t rowIndex = 0; rowIndex < p.puzzle.size(); rowIndex++) {
					int leftColumn = columnIndex - sideOffset;
					int rightColumn = columnIndex - 1 + sideOffset;

					bool equal = p.puzzle[rowIndex][leftColumn] == p.puzzle[rowIndex][rightColumn];
					if (!equal) {
						mistakes++;
						if (mistakes > 1) {
							columnIsSymmetric = false;
							break;
						}
					}
				}
			}

			if (columnIsSymmetric && mistakes == 1) {
				p.reflectedColumn = columnIndex;
				result += columnIndex;
			}
		}

		//check rows
		for (size_t rowIndex = 1; rowIndex < p.puzzle.size(); rowIndex++) {
			int maxDepth = std::min(rowIndex, p.puzzle.size() - rowIndex);

			bool rowIsSymmetric = true;
			int mistakes = 0;
			for (size_t heightOffset = 1; heightOffset <= maxDepth; heightOffset++) {
				if (!rowIsSymmetric) break;
				for (size_t columnIndex = 0; columnIndex < p.puzzle[rowIndex].size(); columnIndex++) {
					int topRow = rowIndex - heightOffset;
					int bottomRow = rowIndex - 1 + heightOffset;

					bool equal = p.puzzle[topRow][columnIndex] == p.puzzle[bottomRow][columnIndex];
					if (!equal) {
						mistakes++;
						if (mistakes > 1) {
							rowIsSymmetric = false;
							break;
						}
					}
				}
			}
			if (rowIsSymmetric && mistakes == 1) {
				result += 100 * rowIndex;
				p.reflectedRow = rowIndex;
			}
		}
	}
	std::cout << "result: " << result << std::endl;

}