#ifndef MUTEXONLY
#define MUTEXONLY

#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

using namespace std;

class MutexOnly
{
public:
    //预计生产(或消费)数量
    int loopCount;
    //当前产品数量
    int product;
    //仓库能容纳的最大产品数量
    int capacity;

    QMutex mutex;
    //生产者
    class Producer : public QThread
    {
    public:
        Producer(MutexOnly *manager) : QThread()
        {
            this->manager = manager;
        }
    protected:
        void run()
        {
            for(int i = 0; i < manager->loopCount; i++)
            {
                manager->mutex.lock();
                manager->product++;
                //cout<<"P";
                cout << i << ".P=" << manager->product << ", ";
                manager->mutex.unlock();
            }
        }
    private:
        MutexOnly *manager;
    };

    //消费者
    class Consumer : public QThread
    {
    public:
        Consumer(MutexOnly *manager) : QThread()
        {
            this->manager = manager;
        }
    protected:
        void run()
        {
            for(int i = 0; i < manager->loopCount; i++)
            {
                manager->mutex.lock();
                manager->product--;
                //cout<<"C";
                cout << i << ".C=" << manager->product << ", ";
                manager->mutex.unlock();
            }
        }
    private:
        MutexOnly *manager;
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
        cout << endl;
    }
};
#endif // MUTEXWAITCONDITION