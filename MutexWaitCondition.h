#ifndef MUTEXWAITCONDITION
#define MUTEXWAITCONDITION

#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

using namespace std;

class MutexWaitCondition {
public:
    //预计生产(或消费)数量
    int loopCount;
    //当前产品数量
    int product;
    //仓库能容纳的最大产品数量
    int capacity;

    QMutex mutex;
    /*
     The QWaitCondition class provides a condition variable for synchronizing threads
     QWaitCondition类为线程同步提供了一个条件变量
     */
    QWaitCondition productIsNotFull;
    QWaitCondition productIsNotEmpty;

    //生产者
    class Producer : public QThread {
    public:
        Producer(MutexWaitCondition *manager) : QThread() {
            this->manager = manager;
        }
    protected:
        void run() {
            for(int i=0; i<manager->loopCount; i++) {
                manager->mutex.lock();
                while(manager->product == manager->capacity) {
                    /*
                     bool QWaitCondition::wait(QReadWriteLock * lockedReadWriteLock, unsigned long time = ULONG_MAX)
                     Releases the lockedReadWriteLock and waits on the wait condition
                     释放该锁，并且阻塞当前线程，直到条件满足（即调用wake方法被唤醒）
                     */
                    manager->productIsNotFull.wait(&manager->mutex);
                }
                manager->product++;
                //cout<<"P";
                cout<<i<<".P="<<manager->product<<", ";
                manager->productIsNotEmpty.wakeAll();
                manager->mutex.unlock();
            }
        }
    private:
        MutexWaitCondition *manager;
    };

    //消费者
    class Consumer : public QThread {
    public:
        Consumer(MutexWaitCondition *manager) : QThread() {
            this->manager = manager;
        }
    protected:
        void run() {
            for(int i=0; i<manager->loopCount; i++) {
                manager->mutex.lock();
                while(manager->product == 0) {
                    manager->productIsNotEmpty.wait(&manager->mutex);
                }
                manager->product--;
                //cout<<"C";
                cout<<i<<".C="<<manager->product<<", ";
                manager->productIsNotFull.wakeAll();
                manager->mutex.unlock();
            }
        }
    private:
        MutexWaitCondition *manager;
    };

//无修饰的方法，默认是private的
public:
    void test(int loopCount, int capacity)
    {
        this->loopCount = loopCount;
        this->capacity = capacity;
        this->product = 0;

        Producer producer(this);
        Consumer consumer(this);
        //thread.start会调用thread内部的run方法
        producer.start();
        consumer.start();
        /*
         Blocks the thread until either of these conditions is met:
         阻塞该线程直到所有条件都满足
         */
        producer.wait();
        consumer.wait();
        cout<<endl;
    }
};
#endif // MUTEXWAITCONDITION