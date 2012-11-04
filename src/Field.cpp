/**
 * Represents field.
 */

#include <string>

#include "stdinclude.h"
#include "Field.h"


Field::~Field() {
	for (unsigned int i = 0; i < field.size(); i++) {
		for (unsigned int j = 0; j < field[i].size(); j++) {
			delete field[i][j];
		}
	}
}


std::list<FieldMember*>::const_iterator Field::getLambdaCacheIt() const{
   	return lambdaCache.begin();
}
std::list<FieldMember*>::iterator Field::getLambdaCacheIt() {
	return lambdaCache.begin();
}
std::list<FieldMember*>::const_iterator Field::getLambdaCacheEnd() const{
	return lambdaCache.end();
}
bool Field::lambdaCacheEmpty() const {
	return lambdaCache.empty();
}
std::list<FieldMember*>::iterator Field::deleteLambdaFromCache(std::list<FieldMember*>::iterator it) {
	return lambdaCache.erase(it);
}


std::list<FieldMember*>::const_iterator Field::getStoneCacheIt() const{
	return stoneCache.begin();
}
std::list<FieldMember*>::iterator Field::getStoneCacheIt() {
	return stoneCache.begin();
}
std::list<FieldMember*>::const_iterator Field::getStoneCacheEnd() const{
	return stoneCache.end();
}
std::list<FieldMember*>::iterator Field::deleteStoneFromCache(std::list<FieldMember*>::iterator it) {
	return stoneCache.erase(it);
}


FieldMember* const Field::getXY(const Point &point) const {
    return field.at(point.y).at(point.x);
}
FieldMember* Field::getXY(const Point &point) {
    return field.at(point.y).at(point.x);
}


std::pair<int, int> Field::getSize() const {
    if (field.size() == 0) {
    	return std::make_pair(0,0);
    }
    return std::make_pair(field[0].size(), field.size());
}


// TODO: наверное, этот метод надо оптимизировать, так как создание карты
// получается путем наращивания векторов,
// но это будет делаться всего один раз, так что, возможно, это не так уж и накладно

Field::Field(const std::string &ASCIIMap): field(), lambdaCache(), stoneCache() {
    /*
     * Index used when filling vectors while reading the map.
     * At this stage vectors are variable length.
     * Further after determining the maximum length
     * all other vectors will be padded with empty cells.
     */
    pRobot=0;
    pLift=0;
    // Calculating map size
    int maxX=0;
    int maxY=0;
    int currX=0;
    int currY=0;
    bool bEndLine=false;
    bool bEndCycle=false;
    int endLine=0;
    int numSkippedElements=0;
    int numInsertedElements=0;
    int numUnknownCharacters=0;
    for(unsigned int i = 0; i < ASCIIMap.length(); i++) {
        switch(ASCIIMap[i])
        {
            case '\n':
                if(currX==0) {
                    if(!bEndLine) {LOGINFO("Map parse: *Skipped after %d line",currY); bEndLine=true; endLine=currY; break;}
                }
                else if(bEndLine) {LOGERROR("Map parse: Empty string at %d line",endLine);bEndCycle=true; throw FieldParseException(); break;};
                if(maxX<currX) maxX=currX;
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
                currX++;
                break; 
            default:
                LOGWARNING("Map parse: Unknown character \'%c\' at %d,%d.",ASCIIMap[i],currX,currY);
                numUnknownCharacters++;
                continue; 
        }
        if(bEndCycle) break;
    }
    maxY=currY;
    
    LOGINFO("Map parse: Estimated map size: %d,%d",maxX,maxY);

    for(int i=0;i<maxY;i++) 
    {
        std::vector<FieldMember*> tmp;
        tmp.resize(maxX,0);
        field.push_back(tmp);
    };
    currX=0;
    currY=0;
    for(unsigned int i = 0; i < ASCIIMap.length(); i++) {
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
                if((currX<maxX) && (currY<maxY)) {
                    write(Point(currX,currY),charToCellType(ASCIIMap[i]));
                }
                else if(ASCIIMap[i]!=' ') {
                    numSkippedElements++;
                }
                currX++;
                break;
            default:
                continue;
        }
    }
    
    for(unsigned int i=0;i<field.size();i++)
    {
        for(unsigned int j=0;j<field[i].size();j++)
        {
            if(field[i][j]==0) {
                write(Point(j,i),EMPTY);
                numInsertedElements++;
            }
        }
    }
    LOGINFO("Map parse: \n\t\t- Skipped elements: %d\n\t\t- Inserted elements: %d\n\t\t- Wrong characters: %d",
            numSkippedElements,numInsertedElements,numUnknownCharacters);
    // Reading the map
    
    if (!lambdaCache.empty() && pLift->getType() == OPENED_LIFT) {
    	throw FieldParseException();
    }
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
    FieldMember* pOldMember=field[xy.y][xy.x];
    FieldMember* pNewMember=new FieldMember(xy,type);
    field[xy.y][xy.x]=pNewMember;
    
    CellType prevType;
    if(pOldMember)
    {
        prevType=pOldMember->getType();
        if(prevType==type) return;
        
        std::list<FieldMember*>::iterator it;
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
        else if(prevType==ROBOT) pRobot=NULL;
        else if((prevType==CLOSED_LIFT) || (prevType==CLOSED_LIFT)) pLift=NULL;
    }
    
    switch(type)
    {
        case STONE: stoneCache.push_back(pNewMember); break;
        case LAMBDA: lambdaCache.push_back(pNewMember); break;
        case OPENED_LIFT: 
            if(pLift) 
                throw FieldParseException();
            pLift=pNewMember; 
            break;
        case CLOSED_LIFT: 
            if(pLift) 
                throw FieldParseException();
            pLift=pNewMember; 
            break;
        case ROBOT: 
            if(pRobot) 
                throw FieldParseException();
            pRobot=pNewMember; 
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
	return pRobot;
}
FieldMember* Field::getRobot() {
	return pRobot;
}


FieldMember* const Field::getLift() const {
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
}


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

bool Field::operator ==(const Field& f) const {
	int xSize = this->field[0].size();
	int ySize = this->field.size();
	for (int y = 0; y < ySize; y++) {
		for (int x = 0; x < xSize; x++) {
			if (!(field[y][x] == f.field[y][x])) {
				return false;
			}
		}
	}
	return true;
}
