#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

    void myTimeout();
    void setFlag(bool flag = true);

signals:
    void mySignal();

private:
    bool isStop;
};

#endif // MYTHREAD_H
