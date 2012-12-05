#include "base/Field.h"

char cellTypeToChar(CellType type) {
	switch(type) {
		case ROBOT: return 'R';
		case STONE: return '*';
		case WALL: return '#';
		case CLOSED_LIFT: return 'L';
		case EMPTY: return ' ';
		case EARTH: return '.';
		case LAMBDA: return '\\';
		case OPENED_LIFT: return 'O';
	}
	return 'U';
}


CellType charToCellType(char c) {
	switch(c) {
		case 'R': return ROBOT;
		case '*': return STONE;
		case '#': return WALL;
		case 'L': return CLOSED_LIFT;
		case ' ': return EMPTY;
		case '.': return EARTH;
		case '\\': return LAMBDA;
		case 'O': return OPENED_LIFT;
	}
	return EMPTY;
}


Field::Field(std::istream& file): lambdaCache(), stoneCache() {
	char* file_buf;
	int file_size;

	file.seekg (0, std::ios::end);
	file_size = file.tellg();
	if (file_size == 0) {
		LOGERROR("Can't load map : file is empty");
		throw FieldParseException();
	} else if (file_size == -1) {
		LOGERROR("Can't load map : no file specified");
		throw FieldParseException();
	}

	file_buf = new char[file_size + 1];
	file.seekg(0, std::ios::beg);
	file.read(file_buf, file_size);
	file_buf[file_size] = '\0';

	try {
		init(file_buf);
	} catch (const FieldParseException& e) {
		LOGERROR("Can't load map: map is incorrect");
		delete [] file_buf;
		throw FieldParseException();
	}
	delete [] file_buf;
	LOGINFO("Map loaded");
}


Field::Field(const std::string& mapFileName): lambdaCache(), stoneCache() {
	std::ifstream file;
	char* file_buf;
	int file_size;

	file.open(mapFileName.c_str(), std::ifstream::in);

	if(!file.is_open()) {
		LOGERROR("Can't load map from \"%s\": can't open file", mapFileName.c_str());
		throw FieldParseException();
	};

	file.seekg (0, std::ios::end);
	file_size = file.tellg();
	if(file_size==0) {
		LOGERROR("Can't load map from \"%s\": file is empty", mapFileName.c_str());
		file.close();
		throw FieldParseException();
	}

	file_buf = new char[file_size + 1];
	file.seekg(0, std::ios::beg);
	file.read(file_buf, file_size);
    file_buf[file_size]='\0';
	file.close();

	try {
		init(file_buf);
	} catch (const FieldParseException& e) {
		LOGERROR("Can't load map from \"%s\": map is incorrect", mapFileName.c_str());
		delete [] file_buf;
		throw FieldParseException();
	}
	delete [] file_buf;
	LOGINFO("Map loaded from \"%s\"", mapFileName.c_str());
}


void Field::init(const std::string& ASCIIMap) {
	xSize = ySize = 0;
	robotKilled = false;

    // Calculating map size
    int maxX = 0;
    int maxY = 0;
    int currX = 0;
    int currY = 0;
    bool bEndLine = false;
    int endLine = 0;
    bool containsRobot = false;
    bool containsLift = false;
    unsigned int lastn = 0;

    for (unsigned int i = 0; i < ASCIIMap.length(); i++) {
        switch (ASCIIMap[i]) {
			case '\n':
				if (currX == 0) {
					if (!bEndLine) {
						LOGINFO("Map parse: *Skipped after %d line", currY);
						bEndLine = true;
						endLine = currY;
						break;
					}
				} else if (bEndLine) {
					LOGERROR("Map parse: Empty string at %d line", endLine);
					throw FieldParseException();
				}
				if (maxX < currX)
					maxX = currX;
				currX = 0;
				currY++;
				lastn = i;
				break;
			case 'R':
				if (!containsRobot) {
					containsRobot = true;
					currX++;
				} else {
					LOGERROR("Map parse: map can't contain more than one robot");
					throw FieldParseException();
				}
				break;
			case 'L':
				if (!containsLift) {
					containsLift = true;
					currX++;
				} else {
					LOGERROR("Map parse: map can't contain more than one closed lift");
					throw FieldParseException();
				}
				break;
			case '*':
			case '\\':
			case '#':
			case '.':
			case ' ':
				currX++;
				break;
			case '\r':
				break;
			case 'O':
				LOGERROR("Map parse: map can't contain opened lifts");
				throw FieldParseException();
			default:
				LOGWARNING("Map parse: Unknown character \'%c\' at %d,%d.",
						   ASCIIMap[i],
						   currX,
						   currY);
				throw FieldParseException();
        }
    }
	if ((!containsRobot) || (!containsLift)) {
		LOGERROR("Map parse: not all required objects exist");
		throw FieldParseException();
	}
    if (lastn < (ASCIIMap.length() - 1))
    	currY++;
    if (maxX < currX)
    	maxX = currX;
    maxY = currY;

    LOGINFO("Map parse: Estimated map size: %d,%d", maxX, maxY);
    xSize = maxX;
    ySize = maxY;
    field = new char*[ySize];
    for (unsigned int i = 0; i < ySize; i++) {
    	field[i] = new char[xSize];
    	std::fill_n(field[i], xSize, EMPTY);
    }

    currX = 0;
    currY = 0;
    for (unsigned int i = 0; i < ASCIIMap.length(); i++) {
        switch(ASCIIMap[i]) {
			case '\n':
				currX = 0;
				currY++;
				break;
			case '\\':
				lambdaCache.push_back(new Point(currX, currY));
				field[currY][currX] = charToCellType(ASCIIMap[i]);
				currX++;
				break;
			case '*':
				stoneCache.push_back(new Point(currX, currY));
				field[currY][currX] = charToCellType(ASCIIMap[i]);
				currX++;
				break;
			case 'R':
				robot = new Point(currX, currY);
				field[currY][currX] = charToCellType(ASCIIMap[i]);
				currX++;
				break;
			case 'L':
				lift = new Point(currX, currY);
				field[currY][currX] = charToCellType(ASCIIMap[i]);
				currX++;
				break;
			default:
				field[currY][currX] = charToCellType(ASCIIMap[i]);
				currX++;
				break;
        }
    }

    LOGINFO("Map successfully parsed");
}


Field::Field(const Field& orig): lambdaCache(), stoneCache() {
	robotKilled = orig.robotKilled;
	ySize = orig.ySize;
	xSize = orig.xSize;
	robot = new Point(*orig.robot);
	lift = new Point(*orig.lift);

	field = new char*[ySize];
	for (unsigned int i = 0; i < ySize; i++) {
		field[i] = new char[xSize];
	}

    for (unsigned int i = 0; i < ySize; i++) {
    	for (unsigned int j = 0; j < xSize; j++) {
    		field[i][j] = orig.field[i][j];
    	    // перерасчет кэшей
    		switch (field[i][j]) {
				case LAMBDA:
					lambdaCache.push_back(new Point(j, i));
					break;
				case STONE:
					stoneCache.push_back(new Point(j, i));
					break;
				default:
					break;
			}
    	}
    }
}


Field::~Field() {
	for (unsigned int i = 0; i < ySize; i++) {
		delete [] field[i];
	}
	delete [] field;

	FieldCache::iterator it = lambdaCache.begin();
	FieldCache::iterator end = lambdaCache.end();
	for (; it != end; it++) {
		delete *it;
	}

	it = stoneCache.begin();
	end = stoneCache.end();
	for (; it != end; it++) {
		delete *it;
	}

	if (robot != NULL) {
		delete robot;
	}
	if (lift != NULL) {
		delete lift;
	}
}


FieldCache::const_iterator Field::getLambdaCacheIt() const {
   	return lambdaCache.begin();
}


FieldCache::const_iterator Field::getLambdaCacheEnd() const {
	return lambdaCache.end();
}


bool Field::lambdaCacheEmpty() const {
	return lambdaCache.empty();
}


FieldCache::const_iterator Field::getStoneCacheIt() const {
	return stoneCache.begin();
}


FieldCache::const_iterator Field::getStoneCacheEnd() const {
	return stoneCache.end();
}


char Field::getXY(const Point &point) const {
    return field[point.y][point.x];
}


char Field::getXY(int x, int y) const {
	return field[y][x];
}


std::pair<int, int> Field::getSize() const {
    if (ySize == 0) {
    	return std::make_pair(0,0);
    }
    return std::make_pair(xSize, ySize);
}


void Field::write(int x, int y, CellType newCell) {
    char oldCell = field[y][x];
	if (oldCell == newCell)
		return;

	FieldCache::iterator it;
	FieldCache::iterator end;
	switch (oldCell) {
		case LAMBDA:
			it = lambdaCache.begin();
			end = lambdaCache.end();
			while (it != end) {
				if ((*it)->y == y && (*it)->x == x) {
					delete *it;
					lambdaCache.erase(it);
					break;
				}
				it++;
			}
			break;
		case STONE:
			it = stoneCache.begin();
			end = stoneCache.end();
			while (it != end) {
				if ((*it)->y == y && (*it)->x == x) {
					delete *it;
					stoneCache.erase(it);
					break;
				}
				it++;
			}
			break;
		case ROBOT:
			delete robot;
			break;
		case CLOSED_LIFT:
			if (((x != lift->x) && (y != lift->y)) || (newCell != OPENED_LIFT)) {
				throw IllegalWriteException();
			}
			break;
		case OPENED_LIFT:
			if (newCell != ROBOT) {
				throw IllegalWriteException();
			}
			break;
		default:
			break;
	}

    switch (newCell) {
		case STONE:
			stoneCache.push_back(new Point(x, y));
			break;
		case LAMBDA:
			lambdaCache.push_back(new Point(x, y));
			break;
		case ROBOT:
			robot = new Point(x, y);
			break;
		default:
			break;
    }

    field[y][x] = newCell;
}


void Field::write(const Point& xy, CellType newCell) {
    write(xy.x, xy.y, newCell);
}


void Field::swap(const Point &cell1, const Point &cell2) {
	if (cell1 == *lift || cell2 == *lift) {
		throw IllegalWriteException();
	}
	char tmp = field[cell1.y][cell1.x];
    field[cell1.y][cell1.x] = field[cell2.y][cell2.x];
    field[cell2.y][cell2.x] = tmp;
    if (cell1 == *robot) {
		delete robot;
		robot = new Point(cell2);
	} else if (cell2 == *robot) {
		delete robot;
		robot = new Point(cell1);
	}
}


const Point* Field::getRobot() const {
	return robot;
}


const Point* Field::getLift() const {
	return lift;
}


bool Field::isRobotAlive() const {
	return !robotKilled;
}


void Field::killRobot() {
	robotKilled = true;
}


bool Field::isLiftClosed() const {
	return (field[lift->y][lift->x] == CLOSED_LIFT);
}


int Field::getLambdaCount() const {
	return lambdaCache.size();
}


int Field::getStoneCount() const {
	return stoneCache.size();
}


bool Field::isPassable(int x, int y) const {
	switch (field[y][x]) {
		case EARTH:
		case LAMBDA:
		case EMPTY:
			if (field[y - 1][x] == ROBOT && field[y - 2][x] == STONE) {
				return false;
			}
			return true;
		case OPENED_LIFT:
		case ROBOT:
			return true;
		case STONE:
			if ((field[y][x + 1] == ROBOT && field[y][x - 1] == EMPTY) ||
				(field[y][x - 1] == ROBOT && field[y][x + 1] == EMPTY)) {
				return true;
			}
			return false;
		default:
			return false;
	}
}


bool Field::isPassable(const Point& point) const {
	return isPassable(point.x, point.y);
}


bool Field::operator ==(const Field& f) const {
	if (*robot != *f.robot || lambdaCache.size() != f.lambdaCache.size()) {
		return false;
	}

	for (unsigned int y = 0; y < ySize; y++) {
		for (unsigned int x = 0; x < xSize; x++) {
			if (field[y][x] != EARTH && f.field[y][x] != EARTH)
				if (field[y][x] != f.field[y][x]) {
					return false;
				}
		}
	}
	return true;
}
