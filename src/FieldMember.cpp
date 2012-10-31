/*
 * Class represents a cell of the field.
 */

#include "FieldMember.h"

/**
 * Преобразование типа ячейки в символ.
 * @param CellType type - тип ячейки.
 * @return символ.
 */
char cellTypeToChar(CellType type) {
    switch(type)
    {
        case ROBOT: return 'R';
        case STONE: return '*';
        case WALL:  return '#';
        case CLOSED_LIFT: return 'L';
        case EMPTY:     return ' ';
        case EARTH:     return '.';
        case LAMBDA:    return '\\';
        case OPENED_LIFT: return 'O';
    }
    return 'U';
};

/**
 * Преобразование символа в тип ячейки.
 * @param char c - символ.
 * @return тип ячейки.
 */
CellType charToCellType(char c) {
    switch(c)
    {
        case 'R':  return ROBOT;
        case '*':  return STONE;
        case '#':  return WALL;
        case 'L':  return CLOSED_LIFT;
        case ' ':  return EMPTY;
        case '.':  return EARTH;
        case '\\': return LAMBDA;
        case 'O':  return OPENED_LIFT;
    }
    return EMPTY;
};

/**
 * Элемент поля.
 * @param Point& coordinate - координаты точки.
 * @param CellType cellType - тип ячейки.
 */
FieldMember::FieldMember(const Point& coordinate, CellType cellType): coordinate(coordinate) {
    this->cellType = cellType;
    setDefaultMetric();
}

/**
 * Присвоение метрики по умолчанию.
 */
void FieldMember::setDefaultMetric()
{
    switch (cellType) {
        case ROBOT:
        case STONE:
        case WALL:
        case CLOSED_LIFT:
            this->metric = METRIC_INFINITY;
            break;
        case EMPTY:
        case EARTH:
        case LAMBDA:
        case OPENED_LIFT:
            this->metric = METRIC_NORMAL;
            break;
        }
}

bool FieldMember::operator==(const FieldMember& other) const {
	return (coordinate == other.coordinate &&
			cellType == other.cellType);
}
