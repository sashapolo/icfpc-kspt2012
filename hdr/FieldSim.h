/* 
 * File:   FieldSim.h
 * Author: kirill
 *
 * Created on October 1, 2012, 9:05 PM
 */

#ifndef FIELDSIM_H
#define	FIELDSIM_H

void DrawField(Field* pField, int nStep=0);

class FieldSim {
public:
    FieldSim();
    FieldSim(const FieldSim& orig);
    virtual ~FieldSim();
    
    void CalcNextState(Field* pField);
private:

};

#endif	/* FIELDSIM_H */

