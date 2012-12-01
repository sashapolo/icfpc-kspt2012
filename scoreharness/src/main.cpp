#include <fstream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "algo/Solver.h"

using namespace std;

static const int sleepTime = 150;

bool solvingDone = false;
void* timer(void*);

/**
 * Подсчет количества очков на тестовой выборке карт.
 */
int main(int argc, char** argv) {
    // Инициализация файла с результатами
    // timestamp для имени отчета
    time_t curTimestampEpoch = time(NULL);
    tm* curTimestamp = localtime(&curTimestampEpoch);
    string pathToReport = "scoreharness/reports/ScoreHarness-";
    pathToReport += to_string(1900 + curTimestamp->tm_year);
    pathToReport += "-";
    // +1: month is from 0 to 11
    pathToReport += to_string(curTimestamp->tm_mon + 1);
    pathToReport += "-";
    pathToReport += to_string(curTimestamp->tm_mday);
    pathToReport += "-";
    pathToReport += to_string(curTimestamp->tm_hour);
    pathToReport += "-";
    pathToReport += to_string(curTimestamp->tm_min);
    pathToReport += ".txt";

    ofstream report(pathToReport.c_str());
    if (!report.is_open()) {
		cout << "Can't create report file: " << pathToReport << "\n";
		return -1;
    }

    // Итерируемся по именам карт
    for (int i = 1; i < argc; i++) {
    	SignalHandler::setupSignalHandler();
    	solvingDone = false;
		try {
			Field field(string(argv[i]));

			pthread_t timerThread;
			int err = pthread_create(&timerThread, NULL, timer, NULL);
			if (err) {
				cout << "Error: can't create new thread\n";
				return -1;
			}

			Solver solver(&field);
			string path = solver.solve();
			// Запускаем симулятор для нахождения количества очков
			sSimResult simRes;
			const Field *pField = FieldSim::calcNextStateEx(&field, path[0], &simRes);
			for (unsigned int j = 1; j < path.length(); j++) {
				const Field *oldField = pField;
				pField = FieldSim::calcNextStateEx(oldField, path[j], &simRes);
				delete oldField;
			}
			delete pField;

			solvingDone = true;
			err = pthread_join(timerThread, NULL);
			if (err) {
				cout << "Error: can't join timer thread\n";
				return -1;
			}

			report << argv[i] << ": " << simRes.score << endl;
		} catch (FieldParseException&) {
			cout << "Error while parsing input file: " << argv[i] << "\n";
			i++;
		}
    }

    report.close();
    return 0;
}


void* timer(void *param) {
	for (int i = 0; i < sleepTime && !solvingDone; i++) {
		sleep(1);
	}
	if (!solvingDone) {
		kill(getpid(), SIGINT);
	}
	return NULL;
}
