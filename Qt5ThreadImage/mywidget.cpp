#include "mywidget.h"
#include "ui_mywidget.h"

#include <QPainter>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    // 自定义模块实例化
    myT = new MyThread;

    // 创建子线程
    thread = new QThread(this); // 已经指定父对象了，所以他的释放不用关心

    // 把自定义模块添加到子线程
    myT->moveToThread(thread);

    // 启动子线程，但是，并没有启动线程处理函数
    thread->start();

    // 线程处理函数，必须通过 signal - slot 调用
    connect(ui->pushButton, &QPushButton::pressed, myT, &MyThread::drawImage);
    connect(myT, &MyThread::updateImage, this, &MyWidget::getImage);
    connect(this, &MyWidget::destroyed, this, &MyWidget::dealClose);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::getImage(QImage temp)
{
    image = temp;
    update(); // 更新窗口，间接调用paintEvent()
}

void MyWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this); // 创建画家，指定绘图设备为窗口
    p.drawImage(50, 50, image);
}

void MyWidget::dealClose()
{
    // 推出子线程
    thread->quit();
    // 回收资源
    thread->wait();
    delete myT;
}
