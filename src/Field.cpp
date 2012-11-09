/**
 * Represents field.
 */

#include <string>

#include "stdinclude.h"
#include "Field.h"


Field::Field(const std::string &ASCIIMap): lambdaCache(), stoneCache() {
    // Calculating map size
    int maxX = 0;
    int maxY = 0;
    int currX = 0;
    int currY = 0;
    bool bEndLine = false;
    bool bEndCycle = false;
    int endLine = 0;
    int numSkippedElements = 0;
    int numInsertedElements = 0;
    int numUnknownCharacters = 0;
    int lastn = 0;

    for (unsigned int i = 0; i < ASCIIMap.length(); i++) {
        switch (ASCIIMap[i]) {
            case '\n':
                if(currX == 0) {
                    if (!bEndLine) {
                    	LOGINFO("Map parse: *Skipped after %d line", currY);
                    	bEndLine = true;
                    	endLine = currY;
                    	break;
                    }
                } else if (bEndLine) {
                	LOGERROR("Map parse: Empty string at %d line", endLine);
                	bEndCycle = true;
                	throw FieldParseException();
                }
                if (maxX < currX)
                	maxX = currX;
                currX = 0;
                currY++;
                lastn = i;
                break;
            case '#':
            case 'R':
            case '*':
            case '\\':
            case 'L':
            case '.':
            case 'O':
            case ' ':
                currX++;
                break; 
            case '\r':
                break;
            default:
                LOGWARNING("Map parse: Unknown character \'%c\' at %d,%d.", ASCIIMap[i], currX, currY);
                numUnknownCharacters++;
                continue; 
        }
        if (bEndCycle)
        	break;
    }
    if (lastn < (ASCIIMap.length() - 1))
    	currY++;
    if (maxX < currX)
    	maxX = currX;
    maxY = currY;

    LOGINFO("Map parse: Estimated map size: %d,%d", maxX, maxY);
    xSize = maxX;
    ySize = maxY;
    field = new char[xSize][ySize];
    
    currX = 0;
    currY = 0;
    for (unsigned int i = 0; i < ASCIIMap.length(); i++) {
        switch(ASCIIMap[i])
        {
            case '\n':
                currX=0;
                currY++;
                break;
            case '#':
            case 'R':
            case '*':
            case '\\':
            case 'L':
            case '.':
            case 'O':
            case ' ':
                if ((currX < maxX) && (currY < maxY)) {
                	field[currX][currY] = charToCellType(ASCIIMap[i]);
                } else if (ASCIIMap[i] != ' ') {
                    numSkippedElements++;
                }
                currX++;
                break;
            default:
                continue;
        }
    }
    
    for (unsigned int i=0; i < xSize; i++) {
        for (unsigned int j=0; j<ySize; j++) {
            if(field[i][j] == 0) {
            	field[i][j] == EMPTY;
                numInsertedElements++;
            }
        }
    }

    LOGINFO("Map parse: \n\t\t- Skipped elements: %d\n\t\t- Inserted elements: %d\n\t\t- Wrong characters: %d",
            numSkippedElements, numInsertedElements, numUnknownCharacters);

    // Reading the map
    if ((!lift) || (!robot)) {
        LOGERROR("Map parse: not all required objects exist");
        throw FieldParseException();
    }
    
    if (field[lift.x][lift.y] == OPENED_LIFT && lambdaCache.empty()) {
        LOGERROR("Map parse: lift can't be opened");
    	throw FieldParseException();
    }
}


Field::~Field() {
	delete field;

	FieldCache::iterator it1 = lambdaCache.begin();
	FieldCache::iterator end1 = lambdaCache.end();
	for (; it1 != end1; it1++) {
		delete *it1;
	}

	FieldCache::iterator it2 = stoneCache.begin();
	FieldCache::iterator end2 = stoneCache.end();
	for (; it2 != end2; it2++) {
		delete *it2;
	}
}


FieldCache::const_iterator Field::getLambdaCacheIt() const{
   	return lambdaCache.begin();
}
FieldCache::iterator Field::getLambdaCacheIt() {
	return lambdaCache.begin();
}
FieldCache::const_iterator Field::getLambdaCacheEnd() const{
	return lambdaCache.end();
}
bool Field::lambdaCacheEmpty() const {
	return lambdaCache.empty();
}
FieldCache::iterator Field::deleteLambdaFromCache(FieldCache::iterator it) {
	return lambdaCache.erase(it);
}


FieldCache::const_iterator Field::getStoneCacheIt() const{
	return stoneCache.begin();
}
FieldCache::iterator Field::getStoneCacheIt() {
	return stoneCache.begin();
}
FieldCache::const_iterator Field::getStoneCacheEnd() const{
	return stoneCache.end();
}
FieldCache::iterator Field::deleteStoneFromCache(FieldCache::iterator it) {
	return stoneCache.erase(it);
}


Field::CellType Field::getXY(const Point &point) const {
    return field[point.y][point.x];
}


std::pair<int, int>& Field::getSize() const {
    if (ySize == 0) {
    	return std::make_pair(0,0);
    }
    return std::make_pair(ySize, xSize);
}


Field::Field(const Field& orig): field(orig.field.size(),
		std::vector<FieldMember*>(orig.field[0].size())),
		lambdaCache(), stoneCache() {
    int ySize = field.size();
    int xSize = field[0].size();
    for (int i = 0; i < ySize; i++) {
    	for (int j = 0; j < xSize; j++) {
    		field[i][j] = new FieldMember(*orig.field[i][j]);
    	    // перерасчет кэшей
    		switch (field[i][j]->getType()) {
    		case (ROBOT):
				this->robot = field[i][j];
				break;
    		case (CLOSED_LIFT):
				this->lift = field[i][j];
				break;
    		case (OPENED_LIFT):
				this->lift = field[i][j];
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
    FieldMember* pOldMember=field[xy.y][xy.x];
    FieldMember* pNewMember=new FieldMember(xy,type);
    field[xy.y][xy.x]=pNewMember;
    
    CellType prevType;
    if(pOldMember)
    {
        prevType=pOldMember->getType();
        if(prevType==type) return;
        
        FieldCache::iterator it;
        if(prevType==LAMBDA){
            it=getLambdaCacheIt();
            while(it!=getLambdaCacheEnd())
            {
                if((*it)==pOldMember) {(it)=lambdaCache.erase(it); break;}
                it++;
            }
        }
        else if(prevType==STONE){
            it=getStoneCacheIt();
            while(it!=getStoneCacheEnd())
            {
                if((*it)==pOldMember) {(it)=stoneCache.erase(it); break;}
                it++;
            }
        }
        else if(prevType==ROBOT) robot=NULL;
        else if((prevType==CLOSED_LIFT) || (prevType==CLOSED_LIFT)) lift=NULL;
    }
    
    switch(type)
    {
        case STONE: stoneCache.push_back(pNewMember); break;
        case LAMBDA: lambdaCache.push_back(pNewMember); break;
        case OPENED_LIFT: 
            if(lift) 
                throw FieldParseException();
            lift=pNewMember; 
            break;
        case CLOSED_LIFT: 
            if(lift) 
                throw FieldParseException();
            lift=pNewMember; 
            break;
        case ROBOT: 
            if(robot) 
                throw FieldParseException();
            robot=pNewMember; 
            break;
        default: break;
    }
    
    delete pOldMember;
}


void Field::swap(const Point &cell1, const Point &cell2) {
    FieldMember* tmp1 = getXY(cell1);
    tmp1->setCoordinate(cell2);
    FieldMember* tmp2 = getXY(cell2);
    tmp2->setCoordinate(cell1);
    
    field[cell1.y][cell1.x]=tmp2;
    field[cell2.y][cell2.x]=tmp1;
}


CellType Field::getCellType(const Point &point) const {
    return getXY(point)->getType();
}


void Field::setFieldMember(const FieldMember& fieldMember) {
    int x = fieldMember.getCoordinate().x;
    int y = fieldMember.getCoordinate().y;
    *(field.at(y).at(x)) = fieldMember;
}


FieldMember* const Field::getRobot() const {
	return robot;
}
FieldMember* Field::getRobot() {
	return robot;
}


FieldMember* const Field::getLift() const {
	return lift;
}


bool Field::isRobotAlive() const {
	return robot;
}


int Field::getLambdaCount() const {
	return lambdaCache.size();
}
int Field::getStoneCount() const {
	return stoneCache.size();
}


Field& Field::operator=(const Field& orig) {
	int ySize = field.size();
	int xSize = field[0].size();
	for (int i = 0; i < ySize; i++) {
		for (int j = 0; j < xSize; j++) {
			field[i][j] = new FieldMember(*orig.field[i][j]);
			switch (field[i][j]->getType()) {
			case (ROBOT):
				this->robot = field[i][j];
				break;
			case (CLOSED_LIFT):
				this->lift = field[i][j];
				break;
			case (OPENED_LIFT):
				this->lift = field[i][j];
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

bool Field::operator ==(const Field& f) const {
	int ySize = this->field.size();
	for (int y = 0; y < ySize; y++) {
		int xSize = this->field[y].size();
		for (int x = 0; x < xSize; x++) {
			if (field[y][x]->getType() != WALL && field[y][x]->getType() != EMPTY) {
				if (field[y][x]->getType() != f.field[y][x]->getType()) {
					return false;
				}
			}
		}
	}
	return true;
}
