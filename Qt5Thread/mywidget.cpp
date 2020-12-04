#include "mywidget.h"
#include "ui_mywidget.h"
#include <QDebug>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    // 动态分配空间，不能指定父对象
    myT = new MyThread;

    // 创建子线程
    thread = new QThread(this);

    // 把自定义线程加入到子线程中
    myT->moveToThread(thread);

    connect(myT, &MyThread::mySignal, this, &MyWidget::dealSignal);
    connect(this, &MyWidget::startThread, myT, &MyThread::myTimeout);
    connect(this, &QThread::destroyed, this, &MyWidget::dealClose);
    // 线程处理函数内部不允许操作图形界面

    // connect()第五个参数的作用，连接方式：默认、队列、直接
    // 第五个参数在多线程中才有意义
    // 默认连接方式：
    // 多线程，默认使用队列；
    // 单线程，默认使用直接方式；
    // 队列：槽函数所在的线程和接收者一样
    // 直接：槽函数所在的线程和发送者一样

    // 打印线程ID
    qDebug() << "主线程号： " << QThread::currentThreadId();
    //qDebug() << "主线程号" << QThread::currentThread();
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::dealClose()
{
    on_buttomStop_clicked();
    delete myT;
}

void MyWidget::on_buttomStart_clicked()
{
    // 启动线程，但是没有启动线程处理函数
    thread->start();

    // 不能直接调用线程处理函数，
    // 如果直接调用，将导致线程处理函数和主线程在同一个线程
    // myT->myTimeout();

    // 只能通过 signal - slot 方式调用
    emit startThread();
}

void MyWidget::dealSignal()
{
    static int i = 0;
    i++;
    ui->lcdNumber->display(i);
}

void MyWidget::on_buttomStop_clicked()
{
    myT->setFlag(true);
    thread->quit();
    thread->wait();
}
