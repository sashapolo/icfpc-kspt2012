/*
 * SignalHandler.h
 *
 *  Created on: Nov 8, 2012
 *      Author: alexander
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>

class SignalHandler {
private:
	static void sigIntSignalHandler(int param);
public:
	static void setupSignalHandler();
	static bool sigIntReceived();
};

#endif /* SIGNALHANDLER_H_ */
