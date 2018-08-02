#include <QCoreApplication>
#include "mutexonly.h"
#include "mutexwaitcondition.h"
#include "semaphoremutex.h"
#include <QTime>

int main(int argc, char *argv[])
{
    int loopCount = 100;
    int capacity = 3;
    QTime time;
    int mutexOnlyElapsed;
    int mutexWaitConditionElapsed;
    int SemaphoreMutexElapsed;

    cout << "loopCount = " << loopCount << ", capacity = " << capacity << endl << endl;

    cout << "MutexOnly" << endl;
    MutexOnly mutexOnly;
    time.start();
    mutexOnly.test(loopCount, capacity);
    //milliseconds
    mutexOnlyElapsed = time.elapsed();
    cout << "elapsed " << mutexOnlyElapsed << "ms" << endl;
    cout << endl;

    cout << "MutexWaitCondition" << endl;
    MutexWaitCondition mutexWaitCondition;
    time.restart();
    mutexWaitCondition.test(loopCount, capacity);
    mutexWaitConditionElapsed = time.elapsed();
    cout << "elapsed " << mutexWaitConditionElapsed << "ms" << endl;
    cout << endl;

    cout << "SemaphoreMutex" << endl;
    SemaphoreMutex semaphoreMutex;
    time.restart();
    semaphoreMutex.test(loopCount, capacity);
    SemaphoreMutexElapsed = time.elapsed();
    cout << "elapsed " << SemaphoreMutexElapsed << "ms" << endl;
    cout << endl;
    return 0;
}