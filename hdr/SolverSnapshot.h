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
	std::string deltaPath;
	const FieldMember *goal;

	SolverSnapshot(Field *s, std::string delta, const FieldMember* g) {
		snapshot = s;
		deltaPath = delta;
		goal = g;
	}

	~SolverSnapshot() {
		delete snapshot;
	}
};


#endif /* SOLVERSNAPSHOT_H_ */
