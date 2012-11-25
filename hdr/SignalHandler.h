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
	/**
	 * Разрешение обработки сигнала sigInt.
	 * @param int param - сообщение.
	 */
	static void sigIntSignalHandler(int param);
public:
	/**
	 * todo
	 */
	static void setupSignalHandler();
	/**
	 * Обработчик получения сигнала sigInt.
	 * @return true, если получен сигнал sigInt, false, если нет.
	 */
	static bool sigIntReceived();
};

#endif /* SIGNALHANDLER_H_ */
