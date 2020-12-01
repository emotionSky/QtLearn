/**
 * 需要实现的功能：
 * 一个窗口中有三个选项，每次只能选择一个选项；
 * 选择一个选项之后，会在右边的框中显示这个选择的内容。
 * 实现方式：
 * 使用connect来连接信号与槽函数。（两种方法）
 * 使用成员槽函数实现。
 */
#include "baseqtwindow.h"
#include "ui_baseqtwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QRadioButton>

BaseQtWindow::BaseQtWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BaseQtWindow)
{
    ui->setupUi(this);
    connect(ui->select_1, &QRadioButton::clicked,
            [ = ]()
    {
        ui->display->append(tr("选择一"));
    }
           );

    connect(ui->select_2, &QRadioButton::clicked, this, &BaseQtWindow::buttom2CallBack);
}

BaseQtWindow::~BaseQtWindow()
{
    delete ui;
}

void BaseQtWindow::buttom2CallBack()
{
    ui->display->append(tr("选择二"));
}

void BaseQtWindow::on_select_3_clicked()
{
    ui->display->append(tr("选择三"));
}
