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
    //Ԥ������(������)����
    int loopCount;
    //��ǰ��Ʒ����
    int product;
    //�ֿ������ɵ�����Ʒ����
    int capacity;

    QMutex mutex;
    /*
     The QWaitCondition class provides a condition variable for synchronizing threads
     QWaitCondition��Ϊ�߳�ͬ���ṩ��һ����������
     */
    QWaitCondition productIsNotFull;
    QWaitCondition productIsNotEmpty;

    //������
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
                     �ͷŸ���������������ǰ�̣߳�ֱ���������㣨������wake���������ѣ�
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

    //������
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

//�����εķ�����Ĭ����private��
public:
    void test(int loopCount, int capacity)
    {
        this->loopCount = loopCount;
        this->capacity = capacity;
        this->product = 0;

        Producer producer(this);
        Consumer consumer(this);
        //thread.start�����thread�ڲ���run����
        producer.start();
        consumer.start();
        /*
         Blocks the thread until either of these conditions is met:
         �������߳�ֱ����������������
         */
        producer.wait();
        consumer.wait();
        cout<<endl;
    }
};
#endif // MUTEXWAITCONDITION