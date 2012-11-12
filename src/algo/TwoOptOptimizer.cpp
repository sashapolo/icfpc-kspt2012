/*
 * TwoOptOptimizer.cpp
 *
 *  Created on: Nov 5, 2012
 *      Author: alexander
 */

#include "algo/TwoOptOptimizer.h"

// Оптимизирует путь при помощи эвристики 2-opt
void TwoOptOptimizer::optimize(Path *pPath) {
	// перебираем все возможные ребра в пути
	for (int i = 0; i < pPath->getSize() - 3; i++) {
		for (int j = i + 3; j < pPath->getSize() - 1; j++) {
			doTwoOpt(i, i+1, j, j+1, pPath);
		}
	}
}


void TwoOptOptimizer::doTwoOpt(int start1, int end1, int start2, int end2, Path *pPath) {
	if (start2 == start1 || start2 == end1 || end2 == start1 || end2 == end1) {
		return;
	}
	int oldDistance = pPath->getDistance();
	pPath->swap(end1, start2);
	int newDistance = pPath->getDistance();
	if (newDistance >= oldDistance) {
		pPath->swap(end1, start2);
	}
}
