#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <pthread.h>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "algo/Solver.h"
#include "base/Field.h"
#include "base/FieldSim.h"
#include "HTMLLogger.h"

using namespace std;

/**
 * Функция с таймером для запуска в отдельной нити.
 * Через 150 с посылает SIGINT процессу с заданным PID.
 * 
 * Возвращаемый указатель указывает на heap,
 * после использования необходимо освободить память с помощью delete.
 * 
 * @param pPID указатель на целое, содержащее PID процесса
 * @return NULL
 */
// ToDo: подумать, как реализовать отправку SIGKILL.
void* timer(void *pPID);

/**
 * Подсчет количества очков на тестовой выборке карт.
 */
int main(int argc, char** argv) {

    // Инициализация обработчика SIGINT
    SignalHandler::setupSignalHandler();
    // PID текущего процесса, чтобы послать самому себе SIGINT.
    pid_t pid = getpid();

    // Инициализация лога
    HTMLLogger logger;
    logger.Init("scoreharnessreports/ScoreHarnessLog.html","ScoreHarness");
    SetLogger(&logger);

    // Инициализация файла с результатами
    // timestamp для имени отчета
    time_t curTimestampEpoch = time(NULL);
    // ToDo: получать время с учетом часового пояса.
    tm* curTimestamp = gmtime(&curTimestampEpoch);
    string pathToReport = "scoreharnessreports/ScoreHarness-";
    /*
     * Придется ввести эту переменную,
     * так как передача int в to_string() приводит к невозможности разрешения
     * между
     * to_string(long long int)
     * и
     * to_string(long long unsigned int)
     */
    long long int year = curTimestamp->tm_year;
    pathToReport += to_string(1900+year);
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

    // ToDo: generify with Boost filesystem
    const int arrSize = 12;
    string mapNames[arrSize] = {
        "test",
//        "bigmap",
//        "lightning6",
//        "lightning8",
//        "lightning7",
//        "lightning9",
        "lightning10",
        "map1",
        "map10",
        "map2",
        "map3",
        "map4",
        "map5",
        "map6",
        "map7",
//        "map8",
//        "map9",
        "map100",
//        "map122",
//        "map177",
//        "map420",
//        "map434",
        "map442"
    };

    // Итерируемся по именам карт
    string pathToMapDir = "res/maps/";
    string pathToMap;
    for(int i = 0; i < arrSize; i++) {
        pathToMap = pathToMapDir;
        pathToMap += mapNames[i];
        // Отыскиваем путь для данной карты
        Field field(pathToMap);
        /*
         * Создаем в отдельной нити таймер, который через 150 с
         * выдаст этому же процессу.
         */
        pthread_t timerTID;
        int retVal;
        retVal = pthread_create(&timerTID, NULL, timer, &pid);
        if(retVal) {
            puts("Error while creating timer thread.");
        }
        Solver solver(&field);
        retVal = pthread_join(timerTID, NULL);
        if(retVal) {
            puts("Error while joining timer thread.");
        }
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
        string msg = mapNames[i];
        msg += ": ";
        // Преобразование score из int в string
        /*
         * Придется ввести эту переменную,
         * так как передача int в to_string() приводит к невозможности разрешения
         * между
         * to_string(long long int)
         * и
         * to_string(long long unsigned int)
         */
        long long int score = simRes.score;
        msg += to_string(score);
        msg += "\n";

        report << msg;
    }

    report.close();
    return 0;

}

void* timer(void *pPID) {
    const int sleepInterval = 150;    // в секундах
    sleep(sleepInterval);
    //int pid = *pPID;
    int retVal = kill(getpid(), SIGINT);
    if(retVal != 0) {
        strerror(errno);
    }
    return NULL;
}
