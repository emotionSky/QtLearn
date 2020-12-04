#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QThread>
#include "mythread.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MyWidget;
}
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

signals:
    void startThread(); // 启动子线程的信号

private slots:
    void on_buttomStart_clicked();
    void dealSignal();
    void dealClose();

    void on_buttomStop_clicked();

private:
    Ui::MyWidget *ui;
    MyThread *myT;
    QThread *thread;
};
#endif // MYWIDGET_H
