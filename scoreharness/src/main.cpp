#include <cstdlib>
#include <cstring>

#include <time.h>

#include "algo/Solver.h"
#include "base/Field.h"
#include "base/FieldSim.h"
#include "HTMLLogger.h"

using namespace std;

/**
 * Подсчет количества очков на тестовой выборке карт.
 */
int main(int argc, char** argv) {

    // Инициализация лога
    HTMLLogger logger;
    logger.Init("scoreharnessingreports/ScoreHarnessLog.html","ScoreHarness");
    SetLogger(&logger);

    // Инициализация файла с результатами
    // timestamp для имени отчета
    time_t curTimestampEpoch = time(NULL);
    tm* curTimestamp = gmtime(&curTimestampEpoch);
    string pathToReport = "scoreharnessingreports/ScoreHarness-";
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
    // &pathToReport[0] : не самый очевидный способ преобразования из string в char *
    FILE *report = fopen(&pathToReport[0], "w");

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
        msg += std::to_string(score);
        msg += "\n";
        // &msg[0] : не самый очевидный способ преобразования из string в char *
        fwrite(&msg[0], sizeof(char), msg.length(), report);
    }

    return 0;
}

