/*
 * SignalHandler.cpp
 *
 *  Created on: Nov 8, 2012
 *      Author: alexander
 */

#include "SignalHandler.h"

bool sigIntFlag = false;


void SignalHandler::setupSignalHandler() {
	signal(SIGINT, sigIntSignalHandler);
}


void SignalHandler::sigIntSignalHandler(int param) {
	sigIntFlag = true;
}


bool SignalHandler::sigIntReceived() {
	return sigIntFlag;
}
