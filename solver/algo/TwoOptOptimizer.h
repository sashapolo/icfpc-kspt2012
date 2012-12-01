/*
 * TwoOptOptimizer.h
 *
 *  Created on: Nov 5, 2012
 *      Author: alexander
 */

#ifndef TWOOPTOPTIMIZER_H_
#define TWOOPTOPTIMIZER_H_

#include "../base/Path.h"

class TwoOptOptimizer {
private:
	static void doTwoOpt(int start1, int end1, int start2, int end2, Path *pPath);
public:
	static void optimize(Path *pPath);
};

#endif /* TWOOPTOPTIMIZER_H_ */
