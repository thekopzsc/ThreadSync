#ifndef SEMAPHOREMUTEX
#define SEMAPHOREMUTEX

#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include <QSemaphore>
//ע��:���Ϻܶ�Semaphore����mutex���������Ǵ����
#include <QMutex>

using namespace std;

class SemaphoreMutex {
public:
    //Ԥ������(������)����
    int loopCount;
    //��ǰ��Ʒ����: productSemphore.avaliable()
    //int product;
    //�ֿ������ɵ�����Ʒ����
    int capacity;

    QMutex mutex;
    /*
     The QSemaphore class provides a general counting semaphore
     QSemaphore���ṩ��һ��ͨ�õļ����ź���
     */
    QSemaphore *productSemphore;
    QSemaphore *leftSpaceSemaphore;

    //������
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
                 ����ȥ��ȡ(��ȥ)n�����ź������Ƶ���Դ�����n>������Դ���������ͻ�����ֱ�����㹻����ԴΪֹ��
                 */
                manager->leftSpaceSemaphore->acquire();
                //֮����lockҪ��acquire��������Ϊ: ����������õ���������ô��û����Ʒ����ô�ûᵼ������
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

    //������
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

//�����εķ�����Ĭ����private��
public:
    void test(int loopCount, int capacity)
    {
        this->loopCount = loopCount;
        this->capacity = capacity;

        //����Ϊ: �ź����ĵ�ǰֵ
        productSemphore = new QSemaphore(0);
        leftSpaceSemaphore = new QSemaphore(capacity);

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

#endif // SEMAPHOREMUTEX