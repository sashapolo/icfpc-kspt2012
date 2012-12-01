/*
 * SolverSnapshot.h
 *
 *  Created on: Oct 29, 2012
 *      Author: alexander
 */

#ifndef SOLVERSNAPSHOT_H_
#define SOLVERSNAPSHOT_H_

#include "../base/Field.h"

class SolverSnapshot {
public:
	const Field *snapshot;
	int nextGoalIndex;
	int lambdasCollected;
	int score;
	std::string delta;

	SolverSnapshot(const Field *s, int lambdas, int goalIndex, int score,
				   const std::string& deltaPath) {
		snapshot = s;
		lambdasCollected = lambdas;
		this->score = score;
		nextGoalIndex = goalIndex;
		delta = deltaPath;
	}
};


#endif /* SOLVERSNAPSHOT_H_ */
