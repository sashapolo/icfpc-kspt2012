/*
 * Comparators.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef COMPARATORS_H_
#define COMPARATORS_H_

namespace Comparators {
	template <class T>
	struct PointerComparatorMore: public std::binary_function<T, T, bool> {
		bool operator()(const T& pL, const T& pR) const {
			return (*pL > *pR);
		}
	};

	template <class T>
	struct PointerComparatorEquals: public std::binary_function<T, T, bool> {
		bool operator()(const T& l, const T& r) const {
			return (*l == *r);
		}
	};
}


#endif /* COMPARATORS_H_ */
