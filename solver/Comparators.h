/*
 * Comparators.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef COMPARATORS_H_
#define COMPARATORS_H_

#include <functional>

namespace Comparators {
	template <class T>
	struct PointerComparatorMore: public std::binary_function<T, T, bool> {
		bool operator()(const T& pL, const T& pR) const {
			return (*pL > *pR);
		}
	};

	template <class T>
	struct PointerComparatorEquals: public std::binary_function<T, T, bool> {
		bool operator()(const T& pL, const T& pR) const {
			return (*pL == *pR);
		}
	};

	template <class T>
	struct PointerComparatorLess: public std::binary_function<T, T, bool> {
		bool operator()(const T& pL, const T& pR) const {
			return (*pL < *pR);
		}
	};
}


#endif /* COMPARATORS_H_ */
