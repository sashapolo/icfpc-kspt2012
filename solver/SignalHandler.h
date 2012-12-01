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
	static bool sigIntFlag;
	/**
	 * Разрешение обработки сигнала sigInt.
	 * @param int param - сообщение.
	 */
	inline static void sigIntSignalHandler(int param) {
		sigIntFlag = true;
	}
public:
	inline static void setupSignalHandler() {
		sigIntFlag = false;
		signal(SIGINT, sigIntSignalHandler);
	}
	/**
	 * Обработчик получения сигнала sigInt.
	 * @return true, если получен сигнал sigInt, false, если нет.
	 */
	static bool sigIntReceived(){
		return sigIntFlag;
	}
};

#endif /* SIGNALHANDLER_H_ */
