#ifndef SEMAPHOREMUTEX
#define SEMAPHOREMUTEX

#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include <QSemaphore>
//注意:网上很多Semaphore不用mutex的做法都是错误的
#include <QMutex>

using namespace std;

class SemaphoreMutex {
public:
    //预计生产(或消费)数量
    int loopCount;
    //当前产品数量: productSemphore.avaliable()
    //int product;
    //仓库能容纳的最大产品数量
    int capacity;

    QMutex mutex;
    /*
     The QSemaphore class provides a general counting semaphore
     QSemaphore类提供了一个通用的计数信号量
     */
    QSemaphore *productSemphore;
    QSemaphore *leftSpaceSemaphore;

    //生产者
    class Producer : public QThread {
    public:
        Producer(SemaphoreMutex *manager) : QThread() {
            this->manager = manager;
        }
    protected:
        void run() {
            for(int i=0; i<manager->loopCount; i++) {
                /*
                 Tries to acquire n resources guarded by the semaphore. If n > available(), this call will block until enough resources are available.
                 尝试去获取(减去)n个被信号量控制的资源。如果n>可用资源数量，它就会阻塞直到有足够的资源为止。
                 */
                manager->leftSpaceSemaphore->acquire();
                //之所以lock要在acquire后面是因为: 如果消费者拿到了锁，那么又没有商品，那么久会导致死锁
                manager->mutex.lock();
                manager->productSemphore->release();
                //cout<<"P";
                cout<<i<<".P="<<manager->productSemphore->available()<<", ";
                manager->mutex.unlock();
            }
        }
    private:
        SemaphoreMutex *manager;
    };

    //消费者
    class Consumer : public QThread {
    public:
        Consumer(SemaphoreMutex *manager) : QThread() {
            this->manager = manager;
        }
    protected:
        void run() {
            for(int i=0; i<manager->loopCount; i++) {
                manager->productSemphore->acquire();
                manager->mutex.lock();
                manager->leftSpaceSemaphore->release();
                //cout<<"C";
                cout<<i<<".C="<<manager->productSemphore->available()<<", ";
                manager->mutex.unlock();
            }
        }
    private:
        SemaphoreMutex *manager;
    };

//无修饰的方法，默认是private的
public:
    void test(int loopCount, int capacity)
    {
        this->loopCount = loopCount;
        this->capacity = capacity;

        //参数为: 信号量的当前值
        productSemphore = new QSemaphore(0);
        leftSpaceSemaphore = new QSemaphore(capacity);

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

#endif // SEMAPHOREMUTEX