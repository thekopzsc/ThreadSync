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
    //Ԥ������(������)����
    int loopCount;
    //��ǰ��Ʒ����
    int product;
    //�ֿ������ɵ�����Ʒ����
    int capacity;

    QMutex mutex;
    //������
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

    //������
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
        cout << endl;
    }
};
#endif // MUTEXWAITCONDITION