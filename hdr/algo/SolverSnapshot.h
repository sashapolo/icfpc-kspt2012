/*
 * SolverSnapshot.h
 *
 *  Created on: Oct 29, 2012
 *      Author: alexander
 */

#ifndef SOLVERSNAPSHOT_H_
#define SOLVERSNAPSHOT_H_

class SolverSnapshot {
public:
	Field *snapshot;
	int currentGoalIndex;
	std::string delta;

	SolverSnapshot(Field *s, int goalIndex, const std::string& deltaPath) {
		snapshot = s;
		currentGoalIndex = goalIndex;
		delta = deltaPath;
	}

	~SolverSnapshot() {
		delete snapshot;
	}
};


#endif /* SOLVERSNAPSHOT_H_ */
