#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QComboBox>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QtCharts>
#include <QChartView>
#include <QTimer>
#include <QJsonObject>
#include "chart.h"
#include "chartview.h"

class mainw : public QMainWindow
{
    Q_OBJECT
       
public:
    explicit mainw(QWidget* parent = nullptr);

private slots:
    void exit(); 
    void start(); 
    void stop(); 
    void singleMode(); 
    void contMode(); 
    void calData(); 
    void rawData(); 
    void savePlot(); 
    void loadPlot(); 
    void clearPlot(); 
    // Sensor control:
    void setIntTime(); 
    void setGain(); 
    void showHelp(); 
    int connectDevice(); 
    void changePath(); 
    void keepLogFn(); 
    void toggleNormalization();
    void toggleAutoDetection();


private:
    QWidget* widget; 
    QPushButton* exitBtn; 
    QPushButton* startBtn;
    QPushButton* stopBtn;
    QPushButton* singleBtn;
    QPushButton* continBtn;
    QPushButton* calibrBtn;
    QPushButton* rawBtn;
    QPushButton* clrBtn; 
    QPushButton* saveBtn;   
    QPushButton* loadBtn;
    QPushButton* helpBtn; 
    QComboBox* chartBox; 
    QComboBox* gainBox; 
    QComboBox* iTimeBox; 
    QBoxLayout* btnLayout;
    QBoxLayout* btnupLayout; 
    QBoxLayout* btndwLayout; 
    QBoxLayout* chtLayout; 
    QBoxLayout* mainLayout; 
    Chart* chart; 
    ChartView* chartView;
    QTimer* timer; 
    QSplineSeries* seriesL;
    QScatterSeries* seriesS; 
    QPainter* savep; 
    QLabel* device; 
    QPushButton* retry; 

    QDialog* dialog;
    QBoxLayout* diaLayout;
    QHBoxLayout* checkboxLayout;
    QPushButton* cPathBtn;
    QCheckBox* keepLog;
    QLabel* defPath;
    QCheckBox* normalization;
    QCheckBox* autoDetect;
    QGraphicsTextItem* diesel_a_percentage;
    QGraphicsTextItem* diesel_a_plus_percentage;
    QGraphicsTextItem* diesel_b_percentage;
    QGraphicsTextItem* diesel_c_percentage;
    QGraphicsTextItem* gasolina_95_percentage;
    QGraphicsTextItem* gasolina_98_percentage;
    void addbuttons(); 
    void addChart(); 
    void addConnections(); 
    void removeConnections(); 
    void setRefTime();
    void plot();
    void updatePredictionResults(const QJsonObject& predictionResults);
    void initializeTextItems(QChart* chart);
    void updateTextPositions();


};
#endif // MAINWINDOW_H
