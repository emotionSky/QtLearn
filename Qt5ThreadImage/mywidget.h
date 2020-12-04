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

    // 重写绘图事件
    void paintEvent(QPaintEvent *);
    void getImage(QImage); // 接收图片的槽函数
    void dealClose(); // 窗口关闭槽函数

private:
    Ui::MyWidget *ui;

    QThread *thread;
    MyThread *myT;
    QImage image;
};
#endif // MYWIDGET_H
