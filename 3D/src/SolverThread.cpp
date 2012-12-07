/* 
 * File:   SolverThread.cpp
 * Author: kirill
 * 
 * Created on December 5, 2012, 10:22 PM
 */

#include "stdinclude.h"
#include "SolverThread.h"

void* solverThreadFunc(void* pParam);


SolverThread::SolverThread(){
    funcHandle=0;
    
}


SolverThread::~SolverThread() {
}

void SolverThread::release()
{
    stop();
}

void SolverThread::start(Field* pField)
{
    stop();
    pthread_attr_init(&funcAttr);
    pthread_attr_setdetachstate(&funcAttr, PTHREAD_CREATE_JOINABLE);
    
    solverData.path="";
    solverData.pSolver=new Solver(pField);
    solverData.bThreadProcess=true;
    clock_gettime(CLOCK_MONOTONIC,&startTime);
    
    pthread_create(&funcHandle,&funcAttr,solverThreadFunc,(void *)&solverData);
}
 
void SolverThread::stop()
{
    if(funcHandle==0) return;
    raise( SIGINT);
    pthread_join(funcHandle,0);
    pthread_attr_destroy(&funcAttr);
    funcHandle=0;
    if(solverData.pSolver) 
    {
        delete solverData.pSolver;
        solverData.pSolver=0;
    }
    if(solverData.bThreadProcess) LOGERROR("Solver stop thread error");
    solverData.bThreadProcess=false;
    clock_gettime(CLOCK_MONOTONIC,&endTime);
}

int SolverThread::getTime()
{
    if(isRunning())
    {
        timespec currentTime;
        clock_gettime(CLOCK_MONOTONIC,&currentTime);
        return msecDiff(currentTime, startTime);
    }
    return msecDiff(endTime, startTime);
}

void* solverThreadFunc(void* pParam)
{
    sSolverThreadData* pSolverData=(sSolverThreadData*)pParam;
    pSolverData->bThreadProcess=true;
    pSolverData->path=pSolverData->pSolver->solve();
    pSolverData->bThreadProcess=false;
    return 0;
}