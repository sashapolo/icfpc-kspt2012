/**
 * Represents field.
 */

#include <string>

#include "stdinclude.h"
#include "Field.h"

using namespace std;

Field::~Field() {
	for (unsigned int i = 0; i < field.size(); i++) {
		for (unsigned int j = 0; j < field[i].size(); j++) {
			delete field[i][j];
		}
	}
}

list<FieldMember*>::const_iterator Field::getLambdaCacheIt() const{
   	return lambdaCache.begin();
}
list<FieldMember*>::iterator Field::getLambdaCacheIt() {
	return lambdaCache.begin();
}
list<FieldMember*>::const_iterator Field::getLambdaCacheEnd() const{
	return lambdaCache.end();
}
bool Field::lambdaCacheEmpty() const {
	return lambdaCache.empty();
}
list<FieldMember*>::iterator Field::deleteLambdaFromCache(list<FieldMember*>::iterator it) {
	return lambdaCache.erase(it);
}

list<FieldMember*>::const_iterator Field::getStoneCacheIt() const{
	return stoneCache.begin();
}
list<FieldMember*>::iterator Field::getStoneCacheIt() {
	return stoneCache.begin();
}
list<FieldMember*>::const_iterator Field::getStoneCacheEnd() const{
	return stoneCache.end();
}

list<FieldMember*>::iterator Field::deleteStoneFromCache(list<FieldMember*>::iterator it) {
	return stoneCache.erase(it);
}

const FieldMember* Field::getXY(const Point &point) const {
    return field.at(point.y).at(point.x);
}

FieldMember* Field::getXY(const Point &point) {
    return field.at(point.y).at(point.x);
}

pair<int, int> Field::getSize() const {
    if (field.size() == 0) {
    	return make_pair(0,0);
    }
    return make_pair(field[0].size(), field.size() - 1);
}
// TODO: наверное, этот метод надо оптимизировать, так как создание карты
// получается путем наращивания векторов,
// но это будет делаться всего один раз, так что, возможно, это не так уж и накладно

Field::Field(const string &ASCIIMap): field(), lambdaCache(), stoneCache() {
    /*
     * Index used when filling vectors while reading the map.
     * At this stage vectors are variable length.
     * Further after determining the maximum length
     * all other vectors will be padded with empty cells.
     */

    int numOfVector = 0,       // Number of field line (Y coordinate)
        nestedVectorIndex = 0,	// (X coordinate)
        numOfRobots = 0,
        numOfLifts = 0;

    // Reading the map
    vector<FieldMember*> array;
    for(unsigned int i = 0; i < ASCIIMap.length(); i++) {
        if(ASCIIMap[i] == '\n') {
            // Jump to the next line
            numOfVector++;
            nestedVectorIndex = 0;
            field.push_back(array);
            array.clear();
        } else {
        	FieldMember* tmp;
			CellType cellType;
			Point coor(nestedVectorIndex, numOfVector);
            switch (ASCIIMap[i]) {
                case ('R'):
                	if (++numOfRobots > 1) {
                		throw FieldParseException();
                	}
					cellType = ROBOT;
                	tmp = new FieldMember(coor, cellType);
                	this->pRobot = tmp;
                    break;
                case ('\\'):
                    cellType = LAMBDA;
                    tmp = new FieldMember(coor, cellType);
                    this->lambdaCache.push_back(tmp);
                    break;
                case ('#'):
                    cellType = WALL;
                	tmp = new FieldMember(coor, cellType);
                    break;
                case (' '):
                    cellType = EMPTY;
                	tmp = new FieldMember(coor, cellType);
                    break;
                case ('*'):
                    cellType = STONE;
                	tmp = new FieldMember(coor, cellType);
                	this->stoneCache.push_back(tmp);
                    break;
                case ('.'):
					cellType = EARTH;
					tmp = new FieldMember(coor, cellType);
					break;
                case ('L'):
					if (++numOfLifts > 1) {
						throw FieldParseException();
					}
                    cellType = CLOSED_LIFT;
                	tmp = new FieldMember(coor, cellType);
                	this->pLift = tmp;
                    break;
                case ('O'):
					if (++numOfLifts > 1) {
						throw FieldParseException();
					}
                    cellType = OPENED_LIFT;
                	tmp = new FieldMember(coor, cellType);
                	this->pLift = tmp;
                    break;
                default:
                	throw FieldParseException();
                    break;
            }
            array.push_back(tmp);
            nestedVectorIndex++;
        }
    }
    if (!lambdaCache.empty() && pLift->getType() == OPENED_LIFT) {
    	throw FieldParseException();
    }
}

Field::Field(const Field& orig): field(orig.field.size(), vector<FieldMember*>(orig.field[0].size())),
		lambdaCache(), stoneCache() {
    int ySize = field.size() - 1;
    int xSize = field[0].size();
    for (int i = 0; i < ySize; i++) {
    	for (int j = 0; j < xSize; j++) {
    		field[i][j] = new FieldMember(*orig.field[i][j]);
    	    // перерасчет кэшей
    		switch (field[i][j]->getType()) {
    		case (ROBOT):
				this->pRobot = field[i][j];
				break;
    		case (CLOSED_LIFT):
				this->pLift = field[i][j];
				break;
    		case (OPENED_LIFT):
				this->pLift = field[i][j];
				break;
			case (LAMBDA):
				this->lambdaCache.push_back(field[i][j]);
				break;
			case (STONE):
				this->stoneCache.push_back(field[i][j]);
				break;
			default:
				break;
    		}
    	}
    }
}

void Field::write(Point xy, CellType type)
{
    CellType prevType=getXY(xy)->getType();
    if(prevType==type) return;
    
    FieldMember* pOldMember=field[xy.y][xy.x];
    delete field[xy.y][xy.x];
    FieldMember* pNewMember=new FieldMember(xy,type);
    field[xy.y][xy.x]=pNewMember;
    
    list<FieldMember*>::iterator it;
    if(prevType==LAMBDA)
    {
        it=getLambdaCacheIt();
        while(it!=getLambdaCacheEnd())
        {
            if((*it)==pOldMember) {(it)=lambdaCache.erase(it); break;}
            it++;
        }
    }
    else if(prevType==STONE)
    {
        it=getStoneCacheIt();
        while(it!=getStoneCacheEnd())
        {
            if((*it)==pOldMember) {(it)=stoneCache.erase(it); break;}
            it++;
        }
    }
    else if(prevType==ROBOT)
    {
        pRobot=NULL;
    }
    else if((prevType==CLOSED_LIFT) || (prevType==CLOSED_LIFT))
    {
        pLift=NULL;
    }
    
    switch(type)
    {
        case STONE: stoneCache.push_back(pNewMember); break;
        case LAMBDA: lambdaCache.push_back(pNewMember); break;
        case OPENED_LIFT: pLift=pNewMember; break;
        case CLOSED_LIFT: pLift=pNewMember; break;
        case ROBOT: pRobot=pNewMember; break;
        default: break;
    }
}

void Field::swap(const Point &cell1, const Point &cell2) {
    FieldMember* tmp1 = getXY(cell1);
    tmp1->setCoordinate(cell2);
    FieldMember* tmp2 = getXY(cell2);
    tmp2->setCoordinate(cell1);
    
    field[cell1.y][cell1.x]=tmp2;
    field[cell2.y][cell2.x]=tmp1;
    
    //FieldMember t = *tmp2;
    //setFieldMember(*tmp1);
    //setFieldMember(t);
}

CellType Field::getCellType(const Point &point) const {
    return getXY(point)->getType();
}


void Field::setFieldMember(const FieldMember& fieldMember) {
    int x = fieldMember.getCoordinate().x;
    int y = fieldMember.getCoordinate().y;
    *(field.at(y).at(x)) = fieldMember;
}

const FieldMember* Field::getRobot() const {
	return pRobot;
}

FieldMember* Field::getRobot() {
	return pRobot;
}

const FieldMember* Field::getLift() const {
	return pLift;
}

bool Field::isRobotAlive() const {
	return pRobot;
}

int Field::getLambdaCount() const {
	return lambdaCache.size();
}

int Field::getStoneCount() const {
	return stoneCache.size();
};

int Field::getDistance(const Point& from, const Point& to) const {
	int x = from.x - to.x;
	int y = from.y - to.y;
	x = (x < 0)? -x : x;
	y = (y < 0)? -y : y;
	return x + y;
}

Field& Field::operator=(const Field& orig) {
	int ySize = field.size();
	int xSize = field[0].size();
	for (int i = 0; i < ySize; i++) {
		for (int j = 0; j < xSize; j++) {
			field[i][j] = new FieldMember(*orig.field[i][j]);
			switch (field[i][j]->getType()) {
			case (ROBOT):
				this->pRobot = field[i][j];
				break;
			case (CLOSED_LIFT):
				this->pLift = field[i][j];
				break;
			case (OPENED_LIFT):
				this->pLift = field[i][j];
				break;
			case (LAMBDA):
				this->lambdaCache.push_back(field[i][j]);
				break;
			case (STONE):
				this->stoneCache.push_back(field[i][j]);
				break;
			default:
				break;
			}
		}
	}
	return *this;
}
