/**
 * Represents field.
 */

#include <string>

#include "stdinclude.h"
#include "Field.h"

using namespace std;

Field::Field()
{

}

Field::Field(const string &ASCIIMap) {
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
        if(ASCIIMap[i] == '\n') {
            // Jump to the next line
            numOfVector++;
            nestedVectorIndex = 0;
        } else {
            switch (ASCIIMap[i]) {
                // ToDo: implement validation that there is only one robot in the map
                CellType cellType;
                case ('R'):    
                    cellType = ROBOT;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('\\'):
                    cellType = LAMBDA;
                    // ToDo: Implement cache
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('#'):
                    cellType = WALL;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case (' '):
                    cellType = EMPTY;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('*'):
                    cellType = STONE;
                    // ToDo: Implement stone
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('L'):
                    cellType = CLOSED_LIFT;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('O'):
                    cellType = OPENED_LIFT;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                default:
                    // ToDo: throw an exception
                    break;
                nestedVectorIndex++;
            }
        }
    }
    // Aligning the map
    // Searching the line with the maximum length
    int maxLen = 0;
    for(numOfVector = 0; numOfVector < field.size(); numOfVector) {
        if(maxLen < field[numOfVector].size() ) {
            maxLen = field[numOfVector].size();
        }
    }
    // Padding other lines with empty cells
    int sizeToPad, // Minor optimization: declaration is pulled out from cycle
        vectSize;  // Performance gain?
    for(numOfVector = 0; numOfVector < field.size(); numOfVector) {
        vectSize = field[numOfVector].size(); // Performance gain?
        if(vectSize != maxLen) {
            sizeToPad = maxLen - vectSize;
            for(int i = 0; i < sizeToPad; i++) {
                // ToDo: possible bug: +1?
                CellType cellType = WALL;
                field[numOfVector][vectSize + 1 + i] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
            }
        }
    }
}

Field::Field(const Field& orig) {
    field=orig.field;//Added by Kirill
}

Field::~Field() {
}

void Field::deleteLambdaFromCache(const Point &rPoint) {
    
}

void Field::deleteStoneFromCache(const Point &rPoint) {
    
}

const FieldMember* Field::getXY(const Point &point) const{
    return &field.at(point.getY()).at(point.getX());
}

FieldMember* Field::getXY(const Point &point) {
    return &field.at(point.getY()).at(point.getX());
}

void Field::swap(const Point &cell1, const Point &cell2) {
    FieldMember* tmp1 = getXY(cell1);
    tmp1->setCoordinate(cell2);
    FieldMember* tmp2 = getXY(cell2);
    tmp2->setCoordinate(cell1);
    
    FieldMember ttt=* tmp2;//Added by Kirill
    setFieldMember(tmp1);
    setFieldMember(&ttt);
}

CellType Field::getCellType(const Point &point) {
    return getXY(point)->getType();
}

void Field::setFieldMember(FieldMember* fieldMember) {
    int x = fieldMember->getCoordinate()->getX();
    int y = fieldMember->getCoordinate()->getY();
    field.at(y).at(x) = *fieldMember;
}

void Field::clear()
{
    field.clear();
}

bool Field::load(string Path)
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
            case '*': CurrentCell=ROCK; break;
            case '\\': CurrentCell=LAMBDA; break;
            case 'L': CurrentCell=CLOSED_LIFT; break;
            case '.': CurrentCell=EARTH; break;
            case ' ': CurrentCell=EMPTY; break;
            case '\n': bSkip=true; break;
            case '\r': bSkip=true; break;
            default: CurrentCell=EMPTY;
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
}

pair<int, int> Field::getSize()
{
    if(field.size()==0) return make_pair(0,0);
    return make_pair(field.size(),field[0].size());
}