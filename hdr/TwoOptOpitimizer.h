/*
 * TwoOptOpitimizer.h
 *
 *  Created on: Nov 5, 2012
 *      Author: alexander
 */

#ifndef TWOOPTOPITIMIZER_H_
#define TWOOPTOPITIMIZER_H_

#include "Path.h"

class TwoOptOpitimizer {
private:
	static void doTwoOpt(int start1, int end1, int start2, int end2, Path *pPath);
public:
	static void optimize(Path *pPath);
};

#endif /* TWOOPTOPITIMIZER_H_ */
