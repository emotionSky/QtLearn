#include "agvmainwindow.h"
#include "ui_agvmainwindow.h"

#define MAX_POINT 500

#include <cmath>
#include <algorithm>

AgvMainWindow::AgvMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AgvMainWindow), m_series_sel1(ShowType::ZERO), m_series_sel2(ShowType::ZERO),
    m_series_sel3(ShowType::ZERO), m_series_sel4(ShowType::ZERO), m_series_sel5(ShowType::ZERO),
    m_series_sel6(ShowType::ZERO), m_series_sel7(ShowType::ZERO), m_series_sel8(ShowType::ZERO),
    m_ymax1(8), m_ymax2(8), m_ymin1(-8), m_ymin2(-8), m_period(0.0), m_amptitude(0.0), m_count(0),
    m_type(CurveType::NONE_TYPE), m_sendflag(false), m_command(0.0)
{
    ui->setupUi(this);

    initPidParameter();
    initQChartPart();

    std::fill(m_current, m_current + 8, 0.0);
    std::fill(m_velocity, m_velocity + 8, 0.0);
    std::fill(m_position, m_position + 8, 0.0);

    mpTimer = new QTimer(this);
    mpTimer->setInterval(10); // 单位毫秒
    connect(mpTimer, &QTimer::timeout, this, &AgvMainWindow::timerLoop);
    mpTimer->start();
}

AgvMainWindow::~AgvMainWindow()
{
    delete mpXaxis1;
    delete mpXaxis2;
    delete mpYaxis1;
    delete mpYaxis2;

    delete mpSeries11;
    delete mpSeries12;
    delete mpSeries13;
    delete mpSeries14;
    delete mpSeries21;
    delete mpSeries22;
    delete mpSeries23;
    delete mpSeries24;

    delete mpChart1;
    delete mpChart2;
    delete ui;
}

void AgvMainWindow::timerLoop()
{
    /*定时器处理函数*/
    if(m_sendflag)
        sendDesCommand();

    plotAllData();
}

void AgvMainWindow::initPidParameter()
{
    ui->kpLineEdit1_1->setText("2.2");
    ui->kpLineEdit1_2->setText("2.2");
    ui->kpLineEdit1_3->setText("2.2");
    ui->kpLineEdit1_4->setText("2.2");
    ui->kpLineEdit2_1->setText("2.2");
    ui->kpLineEdit2_2->setText("2.2");
    ui->kpLineEdit2_3->setText("2.2");
    ui->kpLineEdit2_4->setText("2.2");

    ui->kiLineEdit1_1->setText("2.2");
    ui->kiLineEdit1_2->setText("2.2");
    ui->kiLineEdit1_3->setText("2.2");
    ui->kiLineEdit1_4->setText("2.2");
    ui->kiLineEdit2_1->setText("2.2");
    ui->kiLineEdit2_2->setText("2.2");
    ui->kiLineEdit2_3->setText("2.2");
    ui->kiLineEdit2_4->setText("2.2");
}

void AgvMainWindow::initQChartPart()
{
    // 开辟内存空间，初始化相关变量
    mpChart1 = new QChart();
    mpXaxis1 = new QDateTimeAxis();
    mpYaxis1 = new QValueAxis();
    mpSeries11 = new QLineSeries();
    mpSeries12 = new QLineSeries();
    mpSeries13 = new QLineSeries();
    mpSeries14 = new QLineSeries();

    mpChart2 = new QChart();
    mpXaxis2 = new QDateTimeAxis();
    mpYaxis2 = new QValueAxis();
    mpSeries21 = new QLineSeries();
    mpSeries22 = new QLineSeries();
    mpSeries23 = new QLineSeries();
    mpSeries24 = new QLineSeries();

    // 1. 添加曲线到chart上  必须先添加曲线
    mpChart1->addSeries(mpSeries11);
    mpChart1->addSeries(mpSeries12);
    mpChart1->addSeries(mpSeries13);
    mpChart1->addSeries(mpSeries14);

    mpChart2->addSeries(mpSeries21);
    mpChart2->addSeries(mpSeries22);
    mpChart2->addSeries(mpSeries23);
    mpChart2->addSeries(mpSeries24);

    // 2. 命名
    mpXaxis1->setTitleText(tr("时间(s)"));
    mpYaxis1->setTitleText(tr("速度(rad/s)"));

    mpXaxis2->setTitleText(tr("时间(s)"));
    mpYaxis2->setTitleText(tr("速度(rad/s)"));

    // 3. 坐标轴显示范围
    mpXaxis1->setMin(QDateTime::currentDateTime().addSecs(-5));
    mpXaxis1->setMax(QDateTime::currentDateTime().addSecs(0));
    mpYaxis1->setMin(-30);
    mpYaxis1->setMax(30);

    mpXaxis1->setMin(QDateTime::currentDateTime().addSecs(-5));
    mpXaxis1->setMax(QDateTime::currentDateTime().addSecs(0));
    mpYaxis1->setMin(-30);
    mpYaxis1->setMax(30);

    // 4. 坐标轴格点、颜色、网格可视化
    mpYaxis1->setTickCount(9);
    mpYaxis1->setGridLineColor(QColor(130, 57, 53));
    mpYaxis1->setGridLineVisible(false);

    mpYaxis2->setTickCount(9);
    mpYaxis2->setGridLineColor(QColor(130, 57, 53));
    mpYaxis2->setGridLineVisible(false);

    // 5. 坐标轴位置
    mpChart1->addAxis(mpXaxis1, Qt::AlignBottom);
    mpChart1->addAxis(mpYaxis1, Qt::AlignLeft);

    mpChart2->addAxis(mpXaxis2, Qt::AlignBottom);
    mpChart2->addAxis(mpYaxis2, Qt::AlignLeft);

    // 6. 线条
    // QPen(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine, Qt::PenCapStyle c = Qt::SquareCap, Qt::PenJoinStyle j = Qt::BevelJoin);
    QPen pen1(QColor(216, 26, 33), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); // 颜色，粗细，线型，线条最后收头样式，线条连接样式
    QPen pen2(QColor(229, 131, 8), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    QPen pen3(QColor(130, 57, 53), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    QPen pen4(QColor(0, 0, 0), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    // 画笔颜色  红色(216, 26, 33)  橙色(229, 131, 8) 棕色(130, 57, 53) 黑色(0, 0, 0)
    mpSeries11->setPen(pen1);
    mpSeries11->setName(tr("通道1")); // 线条名称
    mpSeries12->setPen(pen2);
    mpSeries12->setName(tr("通道2")); // 线条名称
    mpSeries13->setPen(pen3);
    mpSeries13->setName(tr("通道3")); // 线条名称
    mpSeries14->setPen(pen4);
    mpSeries14->setName(tr("通道4")); // 线条名称

    mpSeries21->setPen(pen1);
    mpSeries21->setName(tr("通道5")); // 线条名称
    mpSeries22->setPen(pen2);
    mpSeries22->setName(tr("通道6")); // 线条名称
    mpSeries23->setPen(pen3);
    mpSeries23->setName(tr("通道7")); // 线条名称
    mpSeries24->setPen(pen4);
    mpSeries24->setName(tr("通道8")); // 线条名称

    // 7. 曲线关联坐标轴
    mpSeries11->attachAxis(mpXaxis1);
    mpSeries11->attachAxis(mpYaxis1);
    mpSeries12->attachAxis(mpXaxis1);
    mpSeries12->attachAxis(mpYaxis1);
    mpSeries13->attachAxis(mpXaxis1);
    mpSeries13->attachAxis(mpYaxis1);
    mpSeries14->attachAxis(mpXaxis1);
    mpSeries14->attachAxis(mpYaxis1);

    mpSeries21->attachAxis(mpXaxis2);
    mpSeries21->attachAxis(mpYaxis2);
    mpSeries22->attachAxis(mpXaxis2);
    mpSeries22->attachAxis(mpYaxis2);
    mpSeries23->attachAxis(mpXaxis2);
    mpSeries23->attachAxis(mpYaxis2);
    mpSeries24->attachAxis(mpXaxis2);
    mpSeries24->attachAxis(mpYaxis2);

    // 8. chart显示到窗口
    ui->myChartView1->setChart(mpChart1);
    ui->myChartView2->setChart(mpChart2);

    // 9. 设置曲线隐藏
    mpSeries11->hide();
    mpSeries12->hide();
    mpSeries13->hide();
    mpSeries14->hide();

    mpSeries21->hide();
    mpSeries22->hide();
    mpSeries23->hide();
    mpSeries24->hide();
}

void AgvMainWindow::sendModeCommand(AgvState mode)
{
    /*根据消息类型设置类型并发送*/
}

void AgvMainWindow::sendDesCommand()
{
    switch (m_type)
    {
    case CurveType::NONE_TYPE:
        m_command = 0.0;
        break;
    case CurveType::SIN_TYPE:
        m_command = m_amptitude * sin(2 * M_PI * m_count * 0.01 / m_period);
        break;
    case CurveType::SQUARE_TYPE:
        m_command = m_amptitude * sin(2 * M_PI * m_count * 0.01 / m_period) > 0 ? m_amptitude : -m_amptitude;
        break;
    }

    ++m_count;
}

void AgvMainWindow::plotAllData()
{
    QDateTime bj_time = QDateTime::currentDateTime(); // 获得当前时间
    mpXaxis1->setMin(QDateTime::currentDateTime().addSecs(-5));
    mpXaxis1->setMax(QDateTime::currentDateTime().addSecs(0));

    mpYaxis1->setMax(m_ymax1);
    mpYaxis1->setMin(m_ymin1);

    mpXaxis2->setMin(QDateTime::currentDateTime().addSecs(-5));
    mpXaxis2->setMax(QDateTime::currentDateTime().addSecs(0));

    mpYaxis2->setMax(m_ymax2);
    mpYaxis2->setMin(m_ymin2);

    if (mpSeries11->count() > (MAX_POINT - 1))
    {
        mpSeries11->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }
    if (mpSeries12->count() > (MAX_POINT - 1))
    {
        mpSeries12->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }
    if (mpSeries13->count() > (MAX_POINT - 1))
    {
        mpSeries13->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }
    if (mpSeries14->count() > (MAX_POINT - 1))
    {
        mpSeries14->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }

    if (mpSeries21->count() > (MAX_POINT - 1))
    {
        mpSeries21->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }
    if (mpSeries22->count() > (MAX_POINT - 1))
    {
        mpSeries22->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }
    if (mpSeries23->count() > (MAX_POINT - 1))
    {
        mpSeries23->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }
    if (mpSeries24->count() > (MAX_POINT - 1))
    {
        mpSeries24->removePoints(0, 1);   // removePoints(int index, int count)　从由index指定的位置开始的系列中删除count指定的点数。mpSeries->count() - 999
    }

    // 新增点到曲线末端
    qreal time = bj_time.toMSecsSinceEpoch();
    mpSeries11->append(time, selectData2Show(m_series_sel1));
    mpSeries12->append(time, selectData2Show(m_series_sel2));
    mpSeries13->append(time, selectData2Show(m_series_sel3));
    mpSeries14->append(time, selectData2Show(m_series_sel4));

    mpSeries21->append(time, selectData2Show(m_series_sel5));
    mpSeries22->append(time, selectData2Show(m_series_sel6));
    mpSeries23->append(time, selectData2Show(m_series_sel7));
    mpSeries24->append(time, selectData2Show(m_series_sel8));
}

qreal AgvMainWindow::selectData2Show(ShowType type)
{
    if(type == ShowType::ZERO)
        return 0.0;
    else if(type == ShowType::SIGNAL)
        return m_command;
    else if(type == ShowType::POSITION1)
        return m_position[0];
    else if(type == ShowType::POSITION2)
        return m_position[1];
    else if(type == ShowType::POSITION3)
        return m_position[2];
    else if(type == ShowType::POSITION4)
        return m_position[3];
    else if(type == ShowType::POSITION5)
        return m_position[4];
    else if(type == ShowType::POSITION6)
        return m_position[5];
    else if(type == ShowType::POSITION7)
        return m_position[6];
    else if(type == ShowType::POSITION8)
        return m_position[7];
    else if(type == ShowType::VELOCITY1)
        return m_velocity[0];
    else if(type == ShowType::VELOCITY2)
        return m_velocity[1];
    else if(type == ShowType::VELOCITY3)
        return m_velocity[2];
    else if(type == ShowType::VELOCITY4)
        return m_velocity[3];
    else if(type == ShowType::VELOCITY5)
        return m_velocity[4];
    else if(type == ShowType::VELOCITY6)
        return m_velocity[5];
    else if(type == ShowType::VELOCITY7)
        return m_velocity[6];
    else if(type == ShowType::VELOCITY8)
        return m_velocity[7];
    else if(type == ShowType::CURRENT1)
        return m_current[0];
    else if(type == ShowType::CURRENT2)
        return m_current[1];
    else if(type == ShowType::CURRENT3)
        return m_current[2];
    else if(type == ShowType::CURRENT4)
        return m_current[3];
    else if(type == ShowType::CURRENT5)
        return m_current[4];
    else if(type == ShowType::CURRENT6)
        return m_current[5];
    else if(type == ShowType::CURRENT7)
        return m_current[6];
    else if(type == ShowType::CURRENT8)
        return m_current[7];
    else
        return 0.0;
}

void AgvMainWindow::on_radioButtom0_1_clicked(bool checked)
{
    if(checked)
    {
        sendModeCommand(AgvState::INIT_STATE);
    }
}

void AgvMainWindow::on_radioButtom0_2_clicked(bool checked)
{
    if(checked)
    {
        sendModeCommand(AgvState::SERVO_STATE);
    }
}

void AgvMainWindow::on_radioButtom0_3_clicked(bool checked)
{
    if(checked)
    {
        sendModeCommand(AgvState::CURRENT_STATE);
    }
}

void AgvMainWindow::on_radioButtom0_4_clicked(bool checked)
{
    if(checked)
    {
        sendModeCommand(AgvState::VELOCITY_STATE);
    }
}

void AgvMainWindow::on_radioButtom0_5_clicked(bool checked)
{
    if(checked)
    {
        sendModeCommand(AgvState::POSITION_STATE);
    }
}

void AgvMainWindow::on_radioButtom0_6_clicked(bool checked)
{
    if(checked)
    {
        sendModeCommand(AgvState::OFF_STATE);
    }
}

void AgvMainWindow::on_curveCheckBox1_1_clicked(bool checked)
{
    if(checked)
    {
        mpSeries11->show();
    }
    else
    {
        mpSeries11->hide();
    }
}

void AgvMainWindow::on_curveCheckBox1_2_clicked(bool checked)
{
    if(checked)
    {
        mpSeries12->show();
    }
    else
    {
        mpSeries12->hide();
    }
}

void AgvMainWindow::on_curveCheckBox1_3_clicked(bool checked)
{
    if(checked)
    {
        mpSeries13->show();
    }
    else
    {
        mpSeries13->hide();
    }
}

void AgvMainWindow::on_curveCheckBox1_4_clicked(bool checked)
{
    if(checked)
    {
        mpSeries14->show();
    }
    else
    {
        mpSeries14->hide();
    }
}

void AgvMainWindow::on_curveCheckBox2_1_clicked(bool checked)
{
    if(checked)
    {
        mpSeries21->show();
    }
    else
    {
        mpSeries21->hide();
    }
}

void AgvMainWindow::on_curveCheckBox2_2_clicked(bool checked)
{
    if(checked)
    {
        mpSeries22->show();
    }
    else
    {
        mpSeries22->hide();
    }
}

void AgvMainWindow::on_curveCheckBox2_3_clicked(bool checked)
{
    if(checked)
    {
        mpSeries23->show();
    }
    else
    {
        mpSeries23->hide();
    }
}

void AgvMainWindow::on_curveCheckBox2_4_clicked(bool checked)
{
    if(checked)
    {
        mpSeries24->show();
    }
    else
    {
        mpSeries24->hide();
    }
}

void AgvMainWindow::on_curveSelBox1_1_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel1 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel1 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel1 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel1 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel1 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel1 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel1 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel1 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel1 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel1 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel1 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel1 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel1 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel1 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel1 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel1 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel1 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel1 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel1 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel1 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel1 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel1 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel1 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel1 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel1 = ShowType::CURRENT8;
    else
        m_series_sel1 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox1_2_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel2 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel2 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel2 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel2 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel2 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel2 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel2 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel2 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel2 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel2 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel2 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel2 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel2 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel2 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel2 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel2 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel2 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel2 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel2 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel2 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel2 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel2 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel2 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel2 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel2 = ShowType::CURRENT8;
    else
        m_series_sel2 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox1_3_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel3 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel3 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel3 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel3 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel3 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel3 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel3 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel3 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel3 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel3 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel3 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel3 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel3 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel3 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel3 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel3 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel3 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel3 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel3 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel3 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel3 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel3 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel3 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel3 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel3 = ShowType::CURRENT8;
    else
        m_series_sel3 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox1_4_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel4 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel4 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel4 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel4 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel4 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel4 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel4 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel4 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel4 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel4 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel4 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel4 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel4 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel4 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel4 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel4 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel4 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel4 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel4 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel4 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel4 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel4 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel4 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel4 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel4 = ShowType::CURRENT8;
    else
        m_series_sel4 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox2_1_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel5 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel5 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel5 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel5 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel5 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel5 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel5 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel5 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel5 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel5 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel5 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel5 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel5 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel5 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel5 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel5 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel5 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel5 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel5 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel5 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel5 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel5 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel5 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel5 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel5 = ShowType::CURRENT8;
    else
        m_series_sel5 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox2_2_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel6 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel6 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel6 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel6 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel6 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel6 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel6 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel6 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel6 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel6 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel6 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel6 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel6 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel6 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel6 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel6 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel6 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel6 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel6 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel6 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel6 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel6 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel6 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel6 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel6 = ShowType::CURRENT8;
    else
        m_series_sel6 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox2_3_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel7 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel7 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel7 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel7 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel7 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel7 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel7 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel7 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel7 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel7 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel7 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel7 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel7 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel7 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel7 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel7 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel7 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel7 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel7 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel7 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel7 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel7 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel7 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel7 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel7 = ShowType::CURRENT8;
    else
        m_series_sel7 = ShowType::ZERO;
}

void AgvMainWindow::on_curveSelBox2_4_currentTextChanged(const QString &arg1)
{
    if(arg1 == "signal")
        m_series_sel8 = ShowType::SIGNAL;
    else if(arg1 == "pos1")
        m_series_sel8 = ShowType::POSITION1;
    else if(arg1 == "pos2")
        m_series_sel8 = ShowType::POSITION2;
    else if(arg1 == "pos3")
        m_series_sel8 = ShowType::POSITION3;
    else if(arg1 == "pos4")
        m_series_sel8 = ShowType::POSITION4;
    else if(arg1 == "pos5")
        m_series_sel8 = ShowType::POSITION5;
    else if(arg1 == "pos6")
        m_series_sel8 = ShowType::POSITION6;
    else if(arg1 == "pos7")
        m_series_sel8 = ShowType::POSITION7;
    else if(arg1 == "pos8")
        m_series_sel8 = ShowType::POSITION8;
    else if(arg1 == "vel1")
        m_series_sel8 = ShowType::VELOCITY1;
    else if(arg1 == "vel2")
        m_series_sel8 = ShowType::VELOCITY2;
    else if(arg1 == "vel3")
        m_series_sel8 = ShowType::VELOCITY3;
    else if(arg1 == "vel4")
        m_series_sel8 = ShowType::VELOCITY4;
    else if(arg1 == "vel5")
        m_series_sel8 = ShowType::VELOCITY5;
    else if(arg1 == "vel6")
        m_series_sel8 = ShowType::VELOCITY6;
    else if(arg1 == "vel7")
        m_series_sel8 = ShowType::VELOCITY7;
    else if(arg1 == "vel8")
        m_series_sel8 = ShowType::VELOCITY8;
    else if(arg1 == "cur1")
        m_series_sel8 = ShowType::CURRENT1;
    else if(arg1 == "cur2")
        m_series_sel8 = ShowType::CURRENT2;
    else if(arg1 == "cur3")
        m_series_sel8 = ShowType::CURRENT3;
    else if(arg1 == "cur4")
        m_series_sel8 = ShowType::CURRENT4;
    else if(arg1 == "cur5")
        m_series_sel8 = ShowType::CURRENT5;
    else if(arg1 == "cur6")
        m_series_sel8 = ShowType::CURRENT6;
    else if(arg1 == "cur7")
        m_series_sel8 = ShowType::CURRENT7;
    else if(arg1 == "cur8")
        m_series_sel8 = ShowType::CURRENT8;
    else
        m_series_sel8 = ShowType::ZERO;
}

void AgvMainWindow::on_pushButton_clicked()
{
    /*将数据读取出来并发送*/
}

void AgvMainWindow::on_AxisYMax_textChanged(const QString &arg1)
{
    m_ymax1 = arg1.toDouble();
}

void AgvMainWindow::on_AxisYMin_textChanged(const QString &arg1)
{
    m_ymin1 = arg1.toDouble();
}

void AgvMainWindow::on_AxisYMax_2_textChanged(const QString &arg1)
{
    m_ymax2 = arg1.toDouble();
}

void AgvMainWindow::on_AxisYMin_2_textChanged(const QString &arg1)
{
    m_ymin2 = arg1.toDouble();
}

void AgvMainWindow::on_GridLine_clicked(bool checked)
{
    if(checked)
        mpYaxis1->show();
    else
        mpYaxis1->hide();
}

void AgvMainWindow::on_GridLine_2_clicked(bool checked)
{
    if(checked)
        mpYaxis2->show();
    else
        mpYaxis2->hide();
}

void AgvMainWindow::on_radioButtom0_7_clicked(bool checked)
{
    if(checked)
        m_period = 1.0;
}

void AgvMainWindow::on_radioButtom0_8_clicked(bool checked)
{
    if(checked)
        m_period = 2.0;
}

void AgvMainWindow::on_radioButtom0_9_clicked(bool checked)
{
    if(checked)
        m_period = 4.0;
}

void AgvMainWindow::on_ampLineEdit0_1_textChanged(const QString &arg1)
{
    m_amptitude = arg1.toDouble();
}

void AgvMainWindow::on_radioButton_10_clicked(bool checked)
{
    if(checked)
        m_type = CurveType::SIN_TYPE;
}

void AgvMainWindow::on_radioButtom0_11_clicked(bool checked)
{
    if(checked)
        m_type = CurveType::SQUARE_TYPE;
}

void AgvMainWindow::on_radioButtom0_12_clicked(bool checked)
{
    if(checked)
    {
        m_sendflag = true;
        m_count = 0;
    }
    else
        m_sendflag = false;
}

void AgvMainWindow::on_pushButton_clicked(bool checked)
{
    if(checked)
    {
        double kp1 = ui->kpLineEdit1_1->text().toDouble();
        double kp2 = ui->kpLineEdit1_2->text().toDouble();
        double kp3 = ui->kpLineEdit1_3->text().toDouble();
        double kp4 = ui->kpLineEdit1_4->text().toDouble();
        double kp5 = ui->kpLineEdit2_1->text().toDouble();
        double kp6 = ui->kpLineEdit2_2->text().toDouble();
        double kp7 = ui->kpLineEdit2_3->text().toDouble();
        double kp8 = ui->kpLineEdit2_4->text().toDouble();

        double ki1 = ui->kiLineEdit1_1->text().toDouble();
        double ki2 = ui->kiLineEdit1_2->text().toDouble();
        double ki3 = ui->kiLineEdit1_3->text().toDouble();
        double ki4 = ui->kiLineEdit1_4->text().toDouble();
        double ki5 = ui->kiLineEdit2_1->text().toDouble();
        double ki6 = ui->kiLineEdit2_2->text().toDouble();
        double ki7 = ui->kiLineEdit2_3->text().toDouble();
        double ki8 = ui->kiLineEdit2_4->text().toDouble();
    }
}
