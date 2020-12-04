#include "mywidget.h"
#include "ui_mywidget.h"

#include <QThread>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    myTimer = new QTimer(this);
    thread = new MyThread();

    // 只要定时器启动，自动触发timeout()
    connect(myTimer, &QTimer::timeout, this, &MyWidget::dealTimeout);
    connect(thread, &MyThread::isDone, this, &MyWidget::dealDone);
    connect(thread, &MyThread::destroyed, this, &MyWidget::stopThread);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::dealTimeout()
{
    static int i = 0;
    i++;
    ui->lcdNumber->display(i);
}

void MyWidget::on_pushButton_clicked()
{
    if(myTimer->isActive() == false)
    {
        myTimer->start(100);
    }

    // 非常复杂的数据处理，耗时较长
    //QThread::sleep(5);

    thread->start();

    // 处理完数据后，关闭定时器
    //myTimer->stop();
    //qDebug() << "over";
}

void MyWidget::dealDone()
{
    qDebug() << "It is over";
    myTimer->stop();
}

void MyWidget::stopThread()
{
    thread->quit();
    // 调用quit()函数并不能立马关闭
    thread->wait();
}
