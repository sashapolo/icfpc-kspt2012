/**
 * Represents field.
 */

#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <list>
#include <climits>
#include "Point.h"
#include "Logger.h"
#include "HTMLLogger.h"

typedef std::list<Point*> FieldCache;

// ислючение, говорящее о неправильности задания карты
struct FieldParseException {
};

// исключение, возникающее при попытке вызова write для перемещения лифта
struct IllegalWriteException {
};

// возможные значения ячеек поля
enum CellType { EMPTY,
				CLOSED_LIFT,
				OPENED_LIFT,
				EARTH,
				LAMBDA,
				ROBOT,
				STONE,
				WALL };

/**
 * Преобразование типа ячейки в символ.
 * @param CellType type - тип ячейки.
 * @return символ.
 */
char cellTypeToChar(CellType type);
/**
 * Преобразование символа в тип ячейки.
 * @param char c - символ.
 * @return тип ячейки.
 */
CellType charToCellType(char c);



/**
 * Represents field.
 */
class Field {
private:
	char **field;
	unsigned int xSize, ySize;
	FieldCache lambdaCache, stoneCache;
    Point *robot, *lift;
    bool robotKilled;

    void init(const std::string &ASCIIMap);
public:
    // значения метрик
    const static int METRIC_NORMAL = 10;
    const static int METRIC_INFINITY = INT_MAX;


    Field(std::istream& file);
    /**
     * Создание поля.
     * @param string mapFileName - имя файла карты.
     * @return результат создания поля.
     */
    Field(const std::string& mapFileName);
    /**
     * Copy constructor makes a copy of the object referenced by a given reference.
     *
     * param orig reference to the object being copied
     */
    Field(const Field& orig);

    virtual ~Field();

    /**
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     */
    FieldCache::const_iterator getLambdaCacheIt() const;
    FieldCache::const_iterator getLambdaCacheEnd() const;
    bool lambdaCacheEmpty() const;
    int getLambdaCount() const;

    FieldCache::const_iterator getStoneCacheIt() const;
    FieldCache::const_iterator getStoneCacheEnd() const;
    int getStoneCount() const;

    /**
     * Returns cell with given coordinates.
     *
     * param pPoint pointer to the point object representing coordinates of the cell to retrieve
     */
    char getXY(const Point &point) const;
    char getXY(int x, int y) const;

    bool isPassable(int x, int y) const;
    bool isPassable(const Point &point) const;

    std::pair<int, int> getSize() const;

    const Point* getRobot() const;
    bool isRobotAlive() const;

    const Point* getLift() const;
    bool isLiftClosed() const;

    bool operator==(const Field&) const;

    // неконстантные методы
    // предназначены для использования только в классе FieldSim
    /**
	 * Swaps contents of the two cells.
	 *
	 * @param rCell1 coordinates of the first cell
	 * @param rCell2 coordinates of the second cell
	 */
	void swap(const Point &Cell1, const Point &Cell2);
	void write(const Point& xy, CellType type);
	void write(int y, int x, CellType type);
	void killRobot();
};

#endif	/* FIELD_H */
