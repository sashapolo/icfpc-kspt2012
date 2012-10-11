/**
 * Represents field.
 */

#include <string>

#include "stdinclude.h"
#include "Field.h"

using namespace std;

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
        nestedVectorIndex = 0; // (X coordinate)

    // Reading the map
    vector<FieldMember*> array;
    for(int i = 0; i < ASCIIMap.length(); i++) {
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
                // ToDo: implement validation that there is only one robot and
            	// one closed OR opened lift on the map
                case ('R'):
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
                    cellType = CLOSED_LIFT;
                	tmp = new FieldMember(coor, cellType);
                	this->pLift = tmp;
                    break;
                case ('O'):
                    cellType = OPENED_LIFT;
                	tmp = new FieldMember(coor, cellType);
                	this->pLift = tmp;
                    break;
                default:
                    // ToDo: throw an exception
                    break;
            }
            array.push_back(tmp);
            nestedVectorIndex++;
        }
    }
    // TODO: нужно ли вообще чем-нибудь заполнять?
    /*
    // Aligning the map
    // Searching the line with the maximum length
    int maxLen = 0;
    for(numOfVector = 0; numOfVector < field.size(); numOfVector++) {
        if(maxLen < field[numOfVector].size() ) {
            maxLen = field[numOfVector].size();
        }
    }
    // Padding other lines with empty cells
    for(numOfVector = 0; numOfVector < field.size(); numOfVector++) {
        int vectSize = field[numOfVector].size();
        if(vectSize != maxLen) {
            int sizeToPad = maxLen - vectSize;
            for(int i = 0; i < sizeToPad; i++) {
                // ToDo: possible bug: +1?
                CellType cellType = WALL;
                field[numOfVector][vectSize + 1 + i] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
            }
        }
    }*/
}

Field::Field(const Field& orig): field(orig.field.size(), vector<FieldMember*>(orig.field[0].size())),
		lambdaCache(), stoneCache() {
    int ySize = field.size();
    int xSize = field[0].size();	// так как все массивы одинакового размера
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
            if((*it)==pOldMember) {(*it)=pNewMember; break;}
            it++;
        }
    }
    else if(prevType==STONE)
    {
        it=getStoneCacheIt();
        while(it!=getStoneCacheEnd())
        {
            if((*it)==pOldMember) {(*it)=pNewMember; break;}
            it++;
        }
    }
}

void Field::swap(const Point &cell1, const Point &cell2) {
    FieldMember* tmp1 = getXY(cell1);
    tmp1->setCoordinate(cell2);
    FieldMember* tmp2 = getXY(cell2);
    tmp2->setCoordinate(cell1);
    
    FieldMember t = *tmp2;
    setFieldMember(*tmp1);
    setFieldMember(t);
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
