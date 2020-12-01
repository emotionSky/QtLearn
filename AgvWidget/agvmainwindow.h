#ifndef AGVMAINWINDOW_H
#define AGVMAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QString>
#include <QChartView>
#include <QtCharts>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include "common_header.h"

namespace Ui
{
class AgvMainWindow;
}

class AgvMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AgvMainWindow(QWidget *parent = nullptr);
    ~AgvMainWindow();

    // ros的回调函数
    void agvDataCallBack();

    // 界面显示
    void timerLoop(); // 定时器处理的函数

    // 曲线显示
    void initChart();  // 初始化chart的函数
    void drawChart();  // 画图函数
    qreal selectData2Show(int select);

    // 期望值生成
    void desGenerate();

    // 设置消息并发送（状态）
    void sendModeCommand(AgvState mode);
    void sendDesCommand();

    // 初始化函数
    void initPidParameter();
    void initQChartPart();

    void plotAllData();
    qreal selectData2Show(ShowType type);


private slots:
    void on_radioButtom0_1_clicked(bool checked);

    void on_radioButtom0_2_clicked(bool checked);

    void on_radioButtom0_3_clicked(bool checked);

    void on_radioButtom0_4_clicked(bool checked);

    void on_radioButtom0_5_clicked(bool checked);

    void on_radioButtom0_6_clicked(bool checked);

    void on_curveCheckBox1_1_clicked(bool checked);

    void on_curveCheckBox1_2_clicked(bool checked);

    void on_curveCheckBox1_3_clicked(bool checked);

    void on_curveCheckBox1_4_clicked(bool checked);

    void on_curveCheckBox2_1_clicked(bool checked);

    void on_curveCheckBox2_2_clicked(bool checked);

    void on_curveCheckBox2_3_clicked(bool checked);

    void on_curveCheckBox2_4_clicked(bool checked);

    void on_curveSelBox1_1_currentTextChanged(const QString &arg1);

    void on_curveSelBox1_2_currentTextChanged(const QString &arg1);

    void on_curveSelBox1_3_currentTextChanged(const QString &arg1);

    void on_curveSelBox1_4_currentTextChanged(const QString &arg1);

    void on_curveSelBox2_1_currentTextChanged(const QString &arg1);

    void on_curveSelBox2_2_currentTextChanged(const QString &arg1);

    void on_curveSelBox2_3_currentTextChanged(const QString &arg1);

    void on_curveSelBox2_4_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_AxisYMax_textChanged(const QString &arg1);

    void on_AxisYMin_textChanged(const QString &arg1);

    void on_AxisYMax_2_textChanged(const QString &arg1);

    void on_AxisYMin_2_textChanged(const QString &arg1);

    void on_GridLine_clicked(bool checked);

    void on_GridLine_2_clicked(bool checked);

    void on_radioButtom0_7_clicked(bool checked);

    void on_radioButtom0_8_clicked(bool checked);

    void on_radioButtom0_9_clicked(bool checked);

    void on_ampLineEdit0_1_textChanged(const QString &arg1);

    void on_radioButton_10_clicked(bool checked);

    void on_radioButtom0_11_clicked(bool checked);

    void on_radioButtom0_12_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

private:
    Ui::AgvMainWindow *ui;

    QTimer *mpTimer;// 定时器

    // 曲线坐标轴设置
    QtCharts::QChart *mpChart1;
    QtCharts::QLineSeries *mpSeries11;
    QtCharts::QLineSeries *mpSeries12;
    QtCharts::QLineSeries *mpSeries13;
    QtCharts::QLineSeries *mpSeries14;
    QtCharts::QDateTimeAxis *mpXaxis1;
    QtCharts::QValueAxis *mpYaxis1;

    QtCharts::QChart *mpChart2;
    QtCharts::QLineSeries *mpSeries21;
    QtCharts::QLineSeries *mpSeries22;
    QtCharts::QLineSeries *mpSeries23;
    QtCharts::QLineSeries *mpSeries24;
    QtCharts::QDateTimeAxis *mpXaxis2;
    QtCharts::QValueAxis *mpYaxis2;

    ShowType m_series_sel1, m_series_sel2, m_series_sel3, m_series_sel4;
    ShowType m_series_sel5, m_series_sel6, m_series_sel7, m_series_sel8;

    // y轴的最大最小值
    double m_ymax1, m_ymax2, m_ymin1, m_ymin2;
    double m_period;
    double m_amptitude;
    long int m_count;
    CurveType m_type;
    bool m_sendflag;
    double m_command;

    double m_current[8], m_velocity[8], m_position[8];
};

#endif // AGVMAINWINDOW_H
