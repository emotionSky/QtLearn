#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QThread *parent = nullptr);

protected:
    // QThread的虚函数
    // 线程处理函数
    // 不能直接调用，通过start()调用
    void run();

signals:
    void isDone();

};

#endif // MYTHREAD_H
