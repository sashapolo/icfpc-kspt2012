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
enum CellType { EMPTY = ' ',
				CLOSED_LIFT = 'L',
				OPENED_LIFT = 'O',
				EARTH = '.',
				LAMBDA = '\\',
				ROBOT = 'R',
				STONE = '*',
				WALL = '#' };

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
 * Класс Field.<br />
 * Описывает карту.
 */
class Field {
private:
	/**
 	 * Карта.
	 */
	char **field;
	/**
 	 * Размеры карты по горизонтали и по вертикали.
	 */
	unsigned int xSize, ySize;
	/**
 	 * Списки всех лямбд и камней.
	 */
	FieldCache lambdaCache, stoneCache;
	/**
 	 * Координаты робота и лифта.
	 */
	Point *robot, *lift;
	/**
 	 * Булевая переменная, которая отвечает за смерть робота.
	 */
	bool robotKilled;
	/**
 	 * Инициализация карты.
	 * Создаются списки всех лямбд и камней.
	 * @param string &ASCIIMap - карта.
	 */
	void init(const std::string &ASCIIMap);
public:
	/**
 	 * Нормальная метрика.
	 */
	const static int METRIC_NORMAL = 1;
	/**
 	 * Бесконечная метрика.
	 */
	const static int METRIC_INFINITY = INT_MAX;
	/**
 	 * Конструктор класса Field.
	 * @param istream& file - имя файла карты.
	 */
	Field(std::istream& file);
	/**
 	 * Создание поля.
	 * @param string mapFileName - имя файла карты.
	 * @return результат создания поля.
	 */
	Field(const std::string& mapFileName);
	/**
	 * Копирует карту.
	 * @param Field& orig - карта.
	 */
	Field(const Field& orig);
	/**
 	 * Деструктор класса Field.
	 * Очищает карту, списки лямбд и камней, позиции робота и лифта.
	 */
	virtual ~Field();

	/**
 	 * Кэш лямбд.
	 * @return первый элемент этого списка.
	 */
	FieldCache::const_iterator getLambdaCacheIt() const;
	/**
 	 * Кэш лямбд.
	 * @return последний элемент этого списка.
	 */
	FieldCache::const_iterator getLambdaCacheEnd() const;
	/**
 	 * Проверка кэша лямбд на отсутствие элементов.
	 * @return true, если список пуст, false, если еще остались лямбды.
	 */
	bool lambdaCacheEmpty() const;
	/**
 	 * Подсчет элементов в кэше лямбд.
	 * @return количество элементов в кэше лямбд.
	 */
	int getLambdaCount() const;
	/**
 	 * Кэш камней.
	 * @return первый элемент этого списка.
	 */
	FieldCache::const_iterator getStoneCacheIt() const;
	/**
 	 * Кэш камней.
	 * @return последний элемент этого списка.
	 */
	FieldCache::const_iterator getStoneCacheEnd() const;
	/**
 	 * Подсчет элементов в кэше камней.
	 * @return количество элементов в кэше камней.
	 */
	int getStoneCount() const;

	/**
 	 * Получение элемента карты с координатами точки.
	 * @param Point &point - точка.
	 * @return элемент карты с координатами точки.
	 */
	char getXY(const Point &point) const;
	/**
 	 * Получение элемента карты с координатами x и y.
	 * @param int x - координата x.
	 * @param int y - координата y.
	 * @return элемент карты с координатами x и y.
	 */
	char getXY(int x, int y) const;
	/**
 	 * Проверка возможности хода заданную точку.
	 * @param int x - координата x.
	 * @param int y - координата y.
	 * @return true, если ход возможен, false, если не возможен.
	 */
	bool isPassable(int x, int y) const;
	/**
 	 * Проверка возможности хода заданную точку.
	 * @param Point &point - точка.
	 * @return true, если ход возможен, false, если не возможен.
	 */
	bool isPassable(const Point &point) const;
	/**
 	 * todo.
	 * @param ?.
	 * @return todo.
	 */
	std::pair<int, int> getSize() const;
	/**
 	 * Получение координаты робота.
	 * @return координату робота.
	 */
	const Point* getRobot() const;
	/**
 	 * Жив ли робот.
	 * @return true, если робот жив, false, если робот уничтожен.
	 */
	bool isRobotAlive() const;
	/**
 	 * Получение координаты лифта.
	 * @return координату лифта.
	 */
	const Point* getLift() const;
	/**
 	 * Закрыт ли лифт.
	 * @return true, если закрыт, false, если открыт.
	 */
	bool isLiftClosed() const;
	/**
 	 * todo.
	 * @param const Field& - элемент карты.
	 * @return todo.
	 */
	bool operator==(const Field&) const;

	// неконстантные методы
	// предназначены для использования только в классе FieldSim
	/**
	 * Обменивает содержимое двух клеток поля местами.
	 * @param Point &Cell1 - 1 клетка.
	 * @param Point &Cell2 - 2 клетка.
	 */
	void swap(const Point &Cell1, const Point &Cell2);
	/**
	 * Запись нового значения в клетку поля.
	 * @param Point& xy - клетка поля.
	 * @param CellType type - тип клетки.
	 */
	void write(const Point& xy, CellType type);
	/**
	 * Запись нового значения в клетку поля.
	 * @param int y - 1 координата.
	 * @param int x - 2 координата.
	 * @param CellType type - тип клетки.
	 */
	void write(int y, int x, CellType type);
	/**
	 * Убивает робота.
	 */
	void killRobot();
};

#endif	/* FIELD_H */
