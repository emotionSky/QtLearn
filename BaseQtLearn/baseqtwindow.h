#ifndef BASEQTWINDOW_H
#define BASEQTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class BaseQtWindow;
}
QT_END_NAMESPACE

class BaseQtWindow : public QMainWindow
{
    Q_OBJECT

public:
    BaseQtWindow(QWidget *parent = nullptr);
    ~BaseQtWindow();

    void buttom2CallBack();

private slots:
    void on_select_3_clicked();

private:
    Ui::BaseQtWindow *ui;
};
#endif // BASEQTWINDOW_H
