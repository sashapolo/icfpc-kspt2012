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
	const FieldMember *collectedLambda;

	SolverSnapshot(Field *s, std::string delta, const FieldMember* lambda) {
		snapshot = s;
		deltaPath = delta;
		collectedLambda = lambda;
	}
};


#endif /* SOLVERSNAPSHOT_H_ */
