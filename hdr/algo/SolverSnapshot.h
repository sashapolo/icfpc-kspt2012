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
	int nextGoalIndex;
	int lambdasCollected;
	int score;
	std::string delta;

	SolverSnapshot(const Field *s, int lambdas, int goalIndex, int score,
				   const std::string& deltaPath) {
		snapshot = new Field(*s);
		lambdasCollected = lambdas;
		this->score = score;
		nextGoalIndex = goalIndex;
		delta = deltaPath;
	}

	~SolverSnapshot() {
		delete snapshot;
	}
};


#endif /* SOLVERSNAPSHOT_H_ */
