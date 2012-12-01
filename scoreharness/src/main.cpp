#include <fstream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "algo/Solver.h"

using namespace std;

static const int sleepTime = 150;

bool solveDone = false;
void* timer(void*);

/**
 * Подсчет количества очков на тестовой выборке карт.
 */
int main(int argc, char** argv) {
    // Инициализация файла с результатами
    // timestamp для имени отчета
    time_t curTimestampEpoch = time(NULL);
    tm* curTimestamp = gmtime(&curTimestampEpoch);
    string pathToReport = "scoreharness/reports/ScoreHarness-";
    // Приведение к long long int - иначе неоднозначность выбора перегрузок
    pathToReport += to_string(1900+(long long int)curTimestamp->tm_year);
    pathToReport += "-";
    long long int mon = curTimestamp->tm_mon;
    // +1: month is from 0 to 11
    pathToReport += to_string(mon+1);
    pathToReport += "-";
    long long int mday = curTimestamp->tm_mday;
    pathToReport += to_string(mday);
    pathToReport += "-";
    long long int hour = curTimestamp->tm_hour;
    pathToReport += to_string(hour);
    pathToReport += "-";
    long long int min = curTimestamp->tm_min;
    pathToReport += to_string(min);
    pathToReport += ".txt";

    ofstream report(pathToReport.c_str());
    if (!report.is_open()) {
		cout << "Can't create report file: " << pathToReport << "\n";
		return -1;
    }

    // Итерируемся по именам карт
    for (int i = 1; i < argc; i++) {
    	SignalHandler::setupSignalHandler();
    	solveDone = false;

    	pthread_t timerThread;
		int err = pthread_create(&timerThread, NULL, timer, NULL);
		if (err) {
			cout << "Error: can't create new thread\n";
			return -1;
		}

        // Отыскиваем путь для данной карты
        Field field(string(argv[i]));
        Solver solver(&field);
        string path = solver.solve();

        // Запускаем симулятор для нахождения количества очков
        sSimResult simRes;
        const Field *pField = &field;    // указатель на карту нужен постоянно + мелкая оптимизация
        pField = FieldSim::calcNextStateEx(pField, path[0], &simRes);
        for(unsigned int j = 1; j < path.length(); j++) {
            const Field *oldField = pField;
            pField = FieldSim::calcNextStateEx(oldField, path[j], &simRes);
            delete oldField;
        }
        delete pField;

        solveDone = true;
        err = pthread_join(timerThread, NULL);
        if (err) {
        	cout << "Error: can't join timer thread\n";
        	return -1;
        }

        string msg = argv[i];
        msg += ": ";
        // Преобразование score из int в string
        // Приведение к long long int - иначе неоднозначность выбора перегрузок
        msg += std::to_string((long long)simRes.score);
        msg += "\n";

        report << msg;
    }

    report.close();
    return 0;
}


void* timer(void *param) {
	for (int i = 0; i < sleepTime && !solveDone; i++) {
		sleep(1);
	}
	if (!solveDone) {
		kill(getpid(), SIGINT);
	}
	return NULL;
}
