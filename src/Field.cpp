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
    for(int i = 0; i < ASCIIMap.length(); i++) {
    	vector<FieldMember> array;
        if(ASCIIMap[i] == '\n') {
            // Jump to the next line
            numOfVector++;
            nestedVectorIndex = 0;
            field.push_back(array);
        } else {
        	FieldMember* tmp;
			CellType cellType;
			Point coor(numOfVector, nestedVectorIndex);
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
            array.push_back(*tmp);
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

Field::Field(const Field& orig): lambdaCache(), stoneCache() {
    field = orig.field;
    int ySize = field.size();
    int xSize = field[0].size();	// так как все массивы одинакового размера
    // перерасчет кэшей
    for (int i = 0; i < ySize; i++) {
    	for (int j = 0; j < xSize; j++) {
    		switch (field[i][j].getType()) {
    		case (ROBOT):
				this->pRobot = &field[i][j];
				break;
    		case (CLOSED_LIFT):
				this->pLift = &field[i][j];
				break;
    		case (OPENED_LIFT):
				this->pLift = &field[i][j];
				break;
			case (LAMBDA):
				this->lambdaCache.push_back(&field[i][j]);
				break;
			case (STONE):
				this->stoneCache.push_back(&field[i][j]);
				break;
			default:
				break;
    		}
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

//TODO: удалить этот метод
/*bool Field::load(string Path)
{
    clear();
    ifstream file;
    char* file_buf;
    long file_size;
    
    file.open(Path.c_str(), ifstream::in);
    
    if(!file.is_open()) {LOGERROR("Can't load map from \"%s\": can't open file",Path.c_str()); return false;};
    
    file.seekg (0, ios::end);
    file_size = file.tellg();
    if(file_size==0) {LOGERROR("Can't load map from \"%s\": file is empty",Path.c_str()); file.close(); return false;}
    
    file_buf = new char [file_size];
    file.seekg (0, ios::beg);
    file.read (file_buf, file_size);
    file.close();
    
    CellType CurrentCell;
    int x_cnt=0,y_cnt=0, x_size=0, y_size=0;
    
    for(int i=0;i<file_size;i++)
    {
        if(file_buf[i]=='\n') 
        {
            if(x_size==0) x_size=x_cnt;
            else if(x_size!=x_cnt)
            {
                LOGERROR("Can't load map from \"%s\": wrong file format",Path.c_str());
                delete [] file_buf;
                return false;
            }
            y_cnt++;
            x_cnt=0;
            
            vector<FieldMember> v;
            field.push_back(v);
        }
        else if(file_buf[i]!='\r') x_cnt++;
    }
    y_size=y_cnt;
    
    bool bSkip=false;
    
    
    for(int i=0;i<file_size;i++)
    {
        bSkip=false;
        switch(file_buf[i])
        {
            case 'R': CurrentCell=ROBOT; break;
            case '#': CurrentCell=WALL; break;
            case '*': CurrentCell=STONE; break;
            case '\\': CurrentCell=LAMBDA; break;
            case 'L': CurrentCell=CLOSED_LIFT; break;
            case '.': CurrentCell=EARTH; break;
            case ' ': CurrentCell=EMPTY; break;
            case '\n': bSkip=true; break;
            case '\r': bSkip=true; break;
            default: CurrentCell=EMPTY; break;
        };
        
        if(!bSkip) 
        {
            field[y_cnt-1].push_back(FieldMember(new Point(x_cnt,y_cnt-1),CurrentCell));
            x_cnt++;
        }
        
        if(file_buf[i]=='\n') 
        {
            y_cnt--;
            x_cnt=0;
        }
    }
  
    delete [] file_buf;
    LOGINFO("Map loaded from \"%s\"",Path.c_str());
    return true;
}*/
