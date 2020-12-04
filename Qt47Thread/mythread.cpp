#include "mythread.h"

MyThread::MyThread(QThread *parent) : QThread(parent)
{

}

void MyThread::run()
{
    // 很复杂的处理
    // 需要耗时5秒
    sleep(5);
    emit isDone();
}
