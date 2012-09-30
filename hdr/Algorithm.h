/*
 * Algorithm.h
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_



class Algorithm {
public:
	virtual Path solve(const Point&, const Point&, const Field&) = 0;
	virtual ~Algorithm();
};

#endif /* ALGORITHM_H_ */
