/* 
 * File:   SolverThread.h
 * Author: kirill
 *
 * Created on December 5, 2012, 10:22 PM
 */

#ifndef SOLVERTHREAD_H
#define	SOLVERTHREAD_H

struct sSolverThreadData
{
    sSolverThreadData()
    {
        bThreadProcess=false;
        pSolver=0;
    }
    Solver* pSolver;
    std::string path;
    bool bThreadProcess;
};

class SolverThread {
public:
    SolverThread();
    virtual ~SolverThread();
    
    void release();
    void start(Field* pField);
    void stop();
    int getTime();
    std::string getResult() {return solverData.path;};
    bool isRunning() {return solverData.bThreadProcess;};
private:
    pthread_t funcHandle;
    pthread_attr_t funcAttr;
    sSolverThreadData solverData;
    timespec startTime;
    timespec endTime;
};

#endif	/* SOLVERTHREAD_H */

