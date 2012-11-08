/*
 * SignalHandler.h
 *
 *  Created on: Nov 8, 2012
 *      Author: alexander
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>

struct SigIntException {
};

class SignalHandler {
private:
	static void sigIntSignalHandler(int param) {
		throw SigIntException();
	}
public:
	static void setupSignalHandler() {
		signal(SIGINT, sigIntSignalHandler);
	}
};

#endif /* SIGNALHANDLER_H_ */
