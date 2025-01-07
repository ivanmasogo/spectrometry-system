#include "headers/mainwindow.h"
#include "headers/predictions.h"
#include "headers/savitzky_golay_filter.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QScatterSeries>
#include <QSplineSeries>
#include "qchartview.h"
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <stdio.h>
#include "headers/chart.h"
#include "headers/chartview.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtConcurrent>
#include "importantFiles/compilationType.h"
#include <QDebug>
#ifdef UNIX
#include <unistd.h>
#include "headers/as7265x.h"
#endif // DEBUG
using namespace std; 
int i2c_fd;
bool preserve = false;
bool normalizationEnabled = false;
bool autoDetectEnabled = false;
double maxval = 0;
int step = 0; 
int lasty=25; 
int lastx = 25;
int actx=0, acty = 0; 
int numPlots = 0; 
string savingPath;
#ifdef UNIX
as7265x_channels_t channels;
#endif // DEBUG
Prediction prediction;


string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%H_%M_%S-%d-%m-%Y", &tstruct);

	return buf;
}

vector<string> split(const string& s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> elems;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

mainw::mainw(QWidget* parent) : QMainWindow(parent) {

	widget = new QWidget(parent);
	setCentralWidget(widget);

    QString basePath = QCoreApplication::applicationDirPath();
    QDir dir(basePath);
    dir.cdUp();
    savingPath = dir.absoluteFilePath("OutputData").toStdString();

	addbuttons();
	addChart();
	mainLayout = new QVBoxLayout();
	mainLayout->addLayout(btnLayout);
	mainLayout->addLayout(chtLayout);
	widget->setLayout(mainLayout);
    //widget->setWindowState(Qt::WindowMaximized);
    setWindowTitle("Spectra GUI 3.0");

	QTimer::singleShot(0, this, SLOT(showMaximized()));
	connectDevice();
	//Set buttons state as default:
	singleBtn->setEnabled(false);
	continBtn->setEnabled(true);
	calibrBtn->setEnabled(false);
	rawBtn->setEnabled(true);
	saveBtn->setEnabled(false);

    // Inicializacion de los punteros
    diesel_a_percentage = nullptr;
    diesel_a_plus_percentage = nullptr;
    diesel_b_percentage = nullptr;
    diesel_c_percentage = nullptr;
    gasolina_95_percentage = nullptr;
    gasolina_98_percentage = nullptr;

}

void mainw::addbuttons() {

	exitBtn = new QPushButton(tr("Exit"));
	startBtn = new QPushButton(tr("Start"));
	stopBtn = new QPushButton(tr("Stop"));
	singleBtn = new QPushButton(tr("Single Mode"));
	continBtn = new QPushButton(tr("Continuous Mode"));
	calibrBtn = new QPushButton(tr("Calibrated data"));
	rawBtn = new QPushButton(tr("Raw data"));
	saveBtn = new QPushButton(tr("Save plot"));
	loadBtn = new QPushButton(tr("Load plot"));
	clrBtn = new QPushButton(tr("Crear plot"));
	helpBtn = new QPushButton(tr("Show help"));
	gainBox = new QComboBox; 
	chartBox = new QComboBox();
	iTimeBox = new QComboBox;
	btnLayout = new QVBoxLayout;
	btnupLayout = new QHBoxLayout;
	btndwLayout = new QHBoxLayout;
	retry = new QPushButton;
	device = new QLabel;
	diaLayout = new QVBoxLayout;
    checkboxLayout = new QHBoxLayout;
	keepLog = new QCheckBox(tr("Keep previous plots"));
    normalization = new QCheckBox(tr("Normalize"));
    autoDetect = new QCheckBox(tr("Automate fuel detection"));

    defPath = new QLabel(tr("Current path: ") + QString::fromStdString(savingPath) + "/");
	cPathBtn = new QPushButton(tr("Change path"));
	timer = new QTimer;
	gainBox->addItem(tr("Set gain x1"));
	gainBox->addItem(tr("Set gain x3.7"));
	gainBox->addItem(tr("Set gain x16"));
	gainBox->addItem(tr("Set gain x64"));
	iTimeBox->addItem("2.78ms");//1 
	iTimeBox->addItem("27.8ms");//10
	iTimeBox->addItem("55.6ms");//20
	iTimeBox->addItem("83.4ms");//30
	iTimeBox->addItem("111.2ms");//40
	iTimeBox->addItem("139ms");//50
	iTimeBox->addItem("166.8ms");//60
	iTimeBox->addItem("194.6ms");//70
	iTimeBox->addItem("222.4ms");//80
	iTimeBox->addItem("250.2ms");//90
	iTimeBox->addItem("278ms");//100
	iTimeBox->addItem("305.8ms");//110
	iTimeBox->addItem("333.6ms");//120
	iTimeBox->addItem("361.4ms");//130
	iTimeBox->addItem("389.2ms");//140
	iTimeBox->addItem("417ms");//150
	iTimeBox->addItem("444.8ms");//160
	iTimeBox->addItem("472.6ms");//170
	iTimeBox->addItem("500.4ms");//180
	iTimeBox->addItem("528.2ms");//190
	iTimeBox->addItem("556ms");//200
	iTimeBox->addItem("583.8ms");//210
	iTimeBox->addItem("611.6ms");//220
	iTimeBox->addItem("639.4ms");//230
	iTimeBox->addItem("667.2ms");//240
	iTimeBox->addItem("695ms");//250
	chartBox->addItem(tr("Scatter chart"));
	chartBox->addItem(tr("Line chart"));
	chartBox->setItemIcon(0, QIcon(":/icons/scatter.png"));
	chartBox->setItemIcon(1, QIcon(":/icons/line.png"));
	exitBtn->setIcon(QIcon(":/icons/exit.png"));
	startBtn->setIcon(QIcon(":/icons/start.png"));
	stopBtn->setIcon(QIcon(":/icons/stop.png"));
	continBtn->setIcon(QIcon(":/icons/cont.png"));
	singleBtn->setIcon(QIcon(":/icons/single.png"));
	loadBtn->setIcon(QIcon(":/icons/load.png"));
	saveBtn->setIcon(QIcon(":/icons/save.png"));
	calibrBtn->setIcon(QIcon(":/icons/cal.png"));
	clrBtn->setIcon(QIcon(":/icons/clean.png"));
	rawBtn->setIcon(QIcon(":/icons/raw.png")); 
	helpBtn->setIcon(QIcon(":/icons/info.png")); 
	btnupLayout->addWidget(exitBtn);
	btnupLayout->addWidget(startBtn);
	btnupLayout->addWidget(stopBtn);
	btnupLayout->addWidget(singleBtn);
	btnupLayout->addWidget(continBtn);
	btnupLayout->addWidget(calibrBtn);
	btnupLayout->addWidget(rawBtn);
	btndwLayout->addWidget(gainBox); 
	btndwLayout->addWidget(iTimeBox);
	btndwLayout->addWidget(chartBox);
	btndwLayout->addWidget(clrBtn);
	btndwLayout->addWidget(saveBtn);
	btndwLayout->addWidget(loadBtn);
	btndwLayout->addWidget(helpBtn);
	btndwLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	btnupLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    checkboxLayout->setAlignment(Qt::AlignLeft);
    checkboxLayout->addWidget(keepLog);
    checkboxLayout->addWidget(normalization);
    checkboxLayout->addWidget(autoDetect);

    diaLayout->addLayout(checkboxLayout);
	diaLayout->addWidget(defPath);
	diaLayout->addWidget(cPathBtn); 
	diaLayout->addWidget(device);
	diaLayout->addWidget(retry);
	diaLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	btnLayout->addLayout(btnupLayout);
	btnLayout->addLayout(btndwLayout);
	btnLayout->addLayout(diaLayout);
}

void mainw::addChart() {
	chart = new Chart();
    chart->setTitle(tr("Output Spectra"));
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignRight);
	chart->setAnimationOptions(QChart::SeriesAnimations);
	chartView = new ChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	chtLayout = new QVBoxLayout();
	chtLayout->addWidget(chartView); 

}
void mainw::addConnections() {

	connect(exitBtn, &QPushButton::released, this, &mainw::exit);
	connect(startBtn, &QPushButton::released, this, &mainw::start);
	connect(stopBtn, &QPushButton::released, this, &mainw::stop);
	connect(singleBtn, &QPushButton::released, this, &mainw::singleMode);
	connect(continBtn, &QPushButton::released, this, &mainw::contMode);
	connect(calibrBtn, &QPushButton::released, this, &mainw::calData);
	connect(rawBtn, &QPushButton::released, this, &mainw::rawData);
	connect(saveBtn, &QPushButton::released, this, &mainw::savePlot);
	connect(loadBtn, &QPushButton::released, this, &mainw::loadPlot);
	connect(clrBtn, &QPushButton::released, this, &mainw::clearPlot);
	connect(helpBtn, &QPushButton::released, this, &mainw::showHelp); 
	connect(timer, &QTimer::timeout, this, &mainw::plot);
	connect(cPathBtn, &QPushButton::released, this, &mainw::changePath);
    connect(keepLog, &QCheckBox::stateChanged, this, &mainw::keepLogFn);
    connect(normalization, &QCheckBox::stateChanged, this, &mainw::toggleNormalization);
    connect(autoDetect, &QCheckBox::stateChanged, this, &mainw::toggleAutoDetection);
	connect(gainBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &mainw::setGain); 
	connect(iTimeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &mainw::setIntTime);
    connect(chartView, &ChartView::resized, this, &mainw::updateTextPositions);



}
void mainw::removeConnections() {

	disconnect(startBtn, &QPushButton::released, this, &mainw::start);
	disconnect(stopBtn, &QPushButton::released, this, &mainw::stop);
	disconnect(singleBtn, &QPushButton::released, this, &mainw::singleMode);
	disconnect(continBtn, &QPushButton::released, this, &mainw::contMode);
	disconnect(calibrBtn, &QPushButton::released, this, &mainw::calData);
	disconnect(rawBtn, &QPushButton::released, this, &mainw::rawData);
	disconnect(saveBtn, &QPushButton::released, this, &mainw::savePlot);
	disconnect(loadBtn, &QPushButton::released, this, &mainw::loadPlot);
	disconnect(clrBtn, &QPushButton::released, this, &mainw::clearPlot);
	disconnect(helpBtn, &QPushButton::released, this, &mainw::showHelp);
	disconnect(timer, &QTimer::timeout, this, &mainw::plot);
	disconnect(gainBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &mainw::setGain);
	disconnect(iTimeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &mainw::setIntTime);
    disconnect(chartView, &ChartView::resized, this, &mainw::updateTextPositions);



}
void mainw::exit() {
	//Exit program

	QApplication::exit(); 
}
void mainw::start() {

	saveBtn->setEnabled(true);
	if (singleBtn->isEnabled()) { //Quiere decir que esta en modo continuo
		startBtn->setEnabled(false);
		gainBox->setEnabled(false);
		iTimeBox->setEnabled(false);
		chartBox->setEnabled(false);
		clrBtn->setEnabled(false);
		saveBtn->setEnabled(false);
		loadBtn->setEnabled(false);
		helpBtn->setEnabled(false);

	}
	
	stopBtn->setEnabled(true);
	timer->setSingleShot(true); 
	timer->setInterval(1000);
	timer->start();
	//QFuture<void> future = QtConcurrent::run(plot);  // Thread 2
}
void mainw::stop() {
	timer->stop(); 
	startBtn->setEnabled(true);
	if (continBtn->isEnabled() == false) { // modo continuo
		stopBtn->setEnabled(false);
		gainBox->setEnabled(true);
		iTimeBox->setEnabled(true);
		chartBox->setEnabled(true);
		clrBtn->setEnabled(true);
		saveBtn->setEnabled(true);
		loadBtn->setEnabled(true);
		helpBtn->setEnabled(true);
	}
	// stop data adquisition
}
void mainw::singleMode() {

	singleBtn->setEnabled(false); 
	startBtn->setEnabled(true); 
	continBtn->setEnabled(true); 
	gainBox->setEnabled(true);
	iTimeBox->setEnabled(true);
	chartBox->setEnabled(true);
	clrBtn->setEnabled(true);
	loadBtn->setEnabled(true);
	helpBtn->setEnabled(true);
	//timer->stop(); 
}
void mainw::contMode() {

	singleBtn->setEnabled(true);
	continBtn->setEnabled(false);
	//timer->start(); 
}
void mainw::calData() {
	// set data adquisition to calibrated data
	calibrBtn->setEnabled(false); 
	rawBtn->setEnabled(true); 

}
void mainw::rawData() {
	// set data adquisition to raw data
	calibrBtn->setEnabled(true);
	rawBtn->setEnabled(false);

}
void mainw::savePlot() {
	QPixmap p = chartView->grab();
	QOpenGLWidget* glWidget = chartView->findChild<QOpenGLWidget*>();
	if (glWidget) {
		QPainter painter(&p);
		QPoint d = glWidget->mapToGlobal(QPoint()) - chartView->mapToGlobal(QPoint());
		painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
		painter.drawImage(d, glWidget->grabFramebuffer());
		painter.end();
	}
	QString filename = QFileDialog::getSaveFileName(this, tr("Save plot as"), "", "png (*.png)"); 
	if(filename.size()!=0)
		p.save(filename, "png");
	// save the current plot
}
void mainw::loadPlot() {
    try {
        saveBtn->setEnabled(true);
        QString fileName = QFileDialog::getOpenFileName(this, tr("Select data file"), "", tr("Data file (*.txt)"));
        if (fileName.size() != 0) {
            vector<string> path = split(fileName.toUtf8().constData(), '/');
            string infopath;
            infopath.append(path[path.size() - 1]);
            ifstream readFile(fileName.toUtf8().constData());
            if (readFile.is_open()) {
                string line;
                getline(readFile, line); // leer primera línea
                vector<string> check = split(line, '-');
                if (check.size() == 2) {
                    QScatterSeries* sSc = new QScatterSeries();
                    QSplineSeries* sLn = new QSplineSeries();
                    QVector<QPointF> vec;
                    double maxval = 0;
                    bool normalizedInput = true;
                    while (getline(readFile, line)) {
                        vector<string> ss = split(line, '-');
                        if (ss.size() == 2) {
                            double x = stof(ss[0]);
                            double y = stof(ss[1]);
                            vec.append(QPointF(x, y));
                            if (y > 1) {
                                normalizedInput = false;
                            }
                            if (y > maxval) {
                                maxval = y;
                            }
                        }
                    }
                    readFile.close();

                    if (normalizedInput) {
                        if(!normalization->isChecked()){
                            normalization->setChecked(true);
                            normalizationEnabled = true;
                        }
                    }else{
                        if(normalization->isChecked()){
                            normalization->setChecked(false);
                            normalizationEnabled = false;
                        }
                    }

                    if (chartBox->currentIndex() == 0) {
                        for (int i = 0; i < vec.size(); i++) {
                            sSc->append(vec[i].x(), vec[i].y());
                        }
                        sSc->setName(QString::fromStdString(infopath));
                        chart->addSeries(sSc);
                    } else if (chartBox->currentIndex() == 1){
                        for (int i = 0; i < vec.size(); i++) {
                            sLn->append(vec[i].x(), vec[i].y());
                        }
                        sLn->setName(QString::fromStdString(infopath));
                        chart->addSeries(sLn);
                    }
                    // Configurar los ejes del grafico
                    chart->createDefaultAxes();
                    if(normalizationEnabled){
                        chart->axes(Qt::Vertical).first()->setRange(0, 1.1);
                    }else{
                        chart->axes(Qt::Vertical).first()->setRange(0, maxval + maxval / 10);
                    }

                    chart->axes(Qt::Horizontal).first()->setRange(405, 945);
                }
            }
        }
    } catch (int e) {
        qDebug() << "Exception caught: " << e;
    }
}

void mainw::clearPlot() {
	chart->removeAllSeries();
	saveBtn->setEnabled(false);
	maxval = 0; 
	numPlots = 0; 
}
void mainw::setGain() {
#ifdef UNIX
	if (connectDevice() == 0)
	as7265x_set_gain(i2c_fd, gainBox->currentIndex());
#endif // 
}
void mainw::setIntTime() {
#ifdef UNIX
	if (connectDevice() == 0) 

		as7265x_set_integration_time(i2c_fd, (iTimeBox->currentIndex() + 1) * 10);
#endif // UNIX

//	setRefTime();
}

void mainw::updatePredictionResults(const QJsonObject& predictionResults) {

       // Extraer los porcentajes de los combustibles del QJsonObject
       double diesel_a = predictionResults["Diesel A"].toDouble();
       double diesel_a_plus = predictionResults["Diesel A+"].toDouble();
       double diesel_b = predictionResults["Diesel B"].toDouble();
       double diesel_c = predictionResults["Diesel C"].toDouble();
       double gasolina_95 = predictionResults["Gasolina 95"].toDouble();
       double gasolina_98 = predictionResults["Gasolina 98"].toDouble();

       // Actualizar los textos con los nuevos valores
       diesel_a_percentage->setPlainText(QString("Diesel A: %1%").arg(diesel_a));
       diesel_a_plus_percentage->setPlainText(QString("Diesel A+: %1%").arg(diesel_a_plus));
       diesel_b_percentage->setPlainText(QString("Diesel B: %1%").arg(diesel_b));
       diesel_c_percentage->setPlainText(QString("Diesel C: %1%").arg(diesel_c));
       gasolina_95_percentage->setPlainText(QString("Gasoline 95: %1%").arg(gasolina_95));
       gasolina_98_percentage->setPlainText(QString("Gasoline 98: %1%").arg(gasolina_98));
}

void mainw::initializeTextItems(QChart* chart) {
    if (!diesel_a_percentage) {
        diesel_a_percentage = new QGraphicsTextItem();
        diesel_a_percentage->setFont(QFont("Arial", 8));
        diesel_a_percentage->setDefaultTextColor(Qt::red);
    }

    if (!diesel_a_plus_percentage) {
        diesel_a_plus_percentage = new QGraphicsTextItem();
        diesel_a_plus_percentage->setFont(QFont("Arial", 8));
        diesel_a_plus_percentage->setDefaultTextColor(Qt::blue);
    }

    if (!diesel_b_percentage) {
        diesel_b_percentage = new QGraphicsTextItem();
        diesel_b_percentage->setFont(QFont("Arial", 8));
        diesel_b_percentage->setDefaultTextColor(Qt::green);
    }

    if (!diesel_c_percentage) {
        diesel_c_percentage = new QGraphicsTextItem();
        diesel_c_percentage->setFont(QFont("Arial", 8));
        diesel_c_percentage->setDefaultTextColor(Qt::gray);
    }

    if (!gasolina_95_percentage) {
        gasolina_95_percentage = new QGraphicsTextItem();
        gasolina_95_percentage->setFont(QFont("Arial", 8));
        gasolina_95_percentage->setDefaultTextColor(Qt::magenta);
    }

    if (!gasolina_98_percentage) {
        gasolina_98_percentage = new QGraphicsTextItem();
        gasolina_98_percentage->setFont(QFont("Arial", 8));
        gasolina_98_percentage->setDefaultTextColor(Qt::cyan);
    }

    if (autoDetectEnabled) {
        chart->scene()->addItem(diesel_a_percentage);
        chart->scene()->addItem(diesel_a_plus_percentage);
        chart->scene()->addItem(diesel_b_percentage);
        chart->scene()->addItem(diesel_c_percentage);
        chart->scene()->addItem(gasolina_95_percentage);
        chart->scene()->addItem(gasolina_98_percentage);

        // Actualizar posiciones inicialmente
        updateTextPositions();
    }

}

void mainw::updateTextPositions() {
    if (autoDetectEnabled) {
        QRectF plotArea = chart->plotArea();
        QRectF legendArea = chart->legend()->boundingRect();

        QPointF topRight = plotArea.topRight() + legendArea.topRight();
        int margin = 10;

        diesel_a_percentage->setPos(topRight.x() - 180, margin);
        diesel_a_plus_percentage->setPos(topRight.x() - 180, margin + 15);
        diesel_b_percentage->setPos(topRight.x() - 180, margin + 30);
        diesel_c_percentage->setPos(topRight.x() - 100 , margin);
        gasolina_95_percentage->setPos(topRight.x() - 100, margin + 15);
        gasolina_98_percentage->setPos(topRight.x() - 100, margin + 30);

        if(!chart->scene()->items().contains(diesel_a_percentage)) {
            chart->scene()->addItem(diesel_a_percentage);
        }
        if(!chart->scene()->items().contains(diesel_a_plus_percentage)) {
            chart->scene()->addItem(diesel_a_plus_percentage);
        }
        if(!chart->scene()->items().contains(diesel_b_percentage)) {
            chart->scene()->addItem(diesel_b_percentage);
        }
        if(!chart->scene()->items().contains(diesel_c_percentage)) {
            chart->scene()->addItem(diesel_c_percentage);
        }
        if(!chart->scene()->items().contains(gasolina_95_percentage)) {
            chart->scene()->addItem(gasolina_95_percentage);
        }
        if(!chart->scene()->items().contains(gasolina_98_percentage)) {
            chart->scene()->addItem(gasolina_98_percentage);
        }
      }
}


void mainw::plot() {
	// start data adquisition
	if (!preserve) {
		chart->removeAllSeries();
	}else{
	if (numPlots == 9) {
		chart->removeSeries(chart->series()[0]);
	}
	else
		numPlots++; 
	}
	if (connectDevice() == 0) {
#ifdef UNIX

    while (!as7265x_is_data_available(i2c_fd)) {
        usleep(500);
    }

    // Obtener la ruta completa del directorio de destino
    string basePath = savingPath;
    string legName = currentDateTime();
    qDebug() << "legname:" << QString::fromStdString(legName);

    // Crear la carpeta para esta medición
    string folderPath = basePath + "/" + legName;  // Carpeta con el nombre de la medición
    system(("mkdir -p " + folderPath).c_str());  // Crear la carpeta si no existe

    // Crear rutas para los archivos .txt y .csv dentro de la carpeta
    string txtFilePath = folderPath + "/Data_" + legName + ".txt";
    string csvFilePath = folderPath + "/Data_" + legName + ".csv";


    // Crear y abrir los archivos para guardar los datos
    ofstream saveFile(txtFilePath);   // Archivo de texto
    ofstream saveCSV(csvFilePath);    // Archivo CSV

    QVector<QPointF> vec;

    // Adquirir datos
    if (calibrBtn->isEnabled()) {
        as7265x_get_all_calibrated_values(i2c_fd, &channels);
    } else {
        as7265x_get_all_raw_values(i2c_fd, &channels);
    }

    as7265x_order_channels(i2c_fd, &channels);

    for (int i = 0; i < 18; i++) {
        vec.append(QPointF(as7265x_ordered_channel_wavelength[i], channels.channel[i]));

    }

    double maxvalue=0;

    // Encontrar el valor maximo
    for (const QPointF &point : vec) {
        if (point.y() > maxvalue) {
            maxvalue = point.y();
        }
    }

    QVector<double> yValues;
    for (int i = 0; i < vec.size(); ++i) {
        yValues.append(vec[i].y());
    }

    // Filtrado Savitzky-Golay
    int window_length = 3;
    int polyorder = 2;
    int deriv_order = 0;

    std::vector<double> data(yValues.begin(), yValues.end()); // Convierto a std::vector
    std::vector<double> filtered = applyFilter(data, window_length, polyorder,deriv_order);

    // Encuentra el valor max y min en el vector
    double min = *std::min_element(filtered.begin(), filtered.end());
    double max = *std::max_element(filtered.begin(), filtered.end());

    // Normalizar los valores
    if(normalizationEnabled){
        std::vector<double> normalized_filtered;
        for (const auto& value : filtered) {
            normalized_filtered.push_back((value - min) / (max - min));
        }

        for (int i = 0; i < vec.size(); i++) {
            vec[i].setY(normalized_filtered[i]);
        }
    }


    // Agregar los puntos a la serie
    if (chartBox->currentIndex() == 1) {
        seriesL = new QSplineSeries();
        for (int i = 0; i < vec.size(); i++) {
            seriesL->append(vec[i].x(), vec[i].y());
        }
        seriesL->setName(QString::fromStdString(legName));
        chart->addSeries(seriesL);
    } else if (chartBox->currentIndex() == 0) {
        seriesS = new QScatterSeries();
        for (int i = 0; i < vec.size(); i++) {
            seriesS->append(vec[i].x(), vec[i].y());
        }
        seriesS->setName(QString::fromStdString(legName));
        chart->addSeries(seriesS);
    }

    // Guardar los datos en el archivo .txt
    if (saveFile.is_open()) {
        saveFile << "Wavelength - Value\n";
        for (int i = 0; i < vec.size(); i++) {
            saveFile << vec.at(i).x() << " - " << vec.at(i).y() << "\n";
        }
        saveFile.close();
    }

    // Guardar los datos en el archivo .csv
    if (saveCSV.is_open()) {
        saveCSV << "Wavelength,Value\n";
        for (int i = 0; i < vec.size(); i++) {
            saveCSV << vec.at(i).x() << "," << vec.at(i).y() << "\n";
        }
        saveCSV.close();
    }

    // Prediccion automatica
    if(autoDetectEnabled){
        // Crear una instancia de Prediction
        QJsonObject predictionResults = prediction.makePredictions(QString::fromStdString(csvFilePath));

        // Actualizamos los textos con los nuevos valores
        updatePredictionResults(predictionResults);

    }

    // Configurar los ejes del gráfico
    chart->createDefaultAxes();

    if(normalizationEnabled){
        chart->axes(Qt::Vertical).first()->setRange(0, 1.1);
    }else{
        chart->axes(Qt::Vertical).first()->setRange(0, maxvalue + maxvalue / 10);
    }

    chart->axes(Qt::Horizontal).first()->setRange(405, 945);

    if (singleBtn->isEnabled() && stopBtn->isEnabled()) { // Modo continuo activado
        timer->start();
    }
#endif

#ifdef WIN32

    while (!as7265x_is_data_available(i2c_fd)) {
        Sleep(500);  // Usar Sleep en lugar de usleep
    }

    // Obtener la ruta completa del directorio de destino
    string basePath = savingPath;
    string legName = currentDateTime();
    qDebug() << "legname:" << QString::fromStdString(legName);

    // Crear la carpeta para esta medición
    string folderPath = basePath + "\\" + legName;  // Carpeta con el nombre de la medición
    _mkdir(folderPath.c_str());  // Crear la carpeta si no existe

    // Crear rutas para los archivos .txt y .csv dentro de la carpeta
    string txtFilePath = folderPath + "\\Data_" + legName + ".txt";
    string csvFilePath = folderPath + "\\Data_" + legName + ".csv";

    // Crear y abrir los archivos para guardar los datos
    ofstream saveFile(txtFilePath);   // Archivo de texto
    ofstream saveCSV(csvFilePath);    // Archivo CSV

    QVector<QPointF> vec;

    // Adquirir datos
    if (calibrBtn->isEnabled()) {
        as7265x_get_all_calibrated_values(i2c_fd, &channels);
    } else {
        as7265x_get_all_raw_values(i2c_fd, &channels);
    }

    as7265x_order_channels(i2c_fd, &channels);

    for (int i = 0; i < 18; i++) {
        vec.append(QPointF(as7265x_ordered_channel_wavelength[i], channels.channel[i]));

    }

    double maxvalue=0;

    // Encontrar el valor máximo
    for (const QPointF &point : vec) {
        if (point.y() > maxvalue) {
            maxvalue = point.y();
        }
    }

    QVector<double> yValues;
    for (int i = 0; i < vec.size(); ++i) {
        yValues.append(vec[i].y());
    }

    // Filtrado Savitzky-Golay
    int window_length = 3;
    int polyorder = 2;
    int deriv_order = 0;

    std::vector<double> data(yValues.begin(), yValues.end()); // Convertir a std::vector
    std::vector<double> filtered = applyFilter(data, window_length, polyorder,deriv_order);

    // Encontrar el valor máximo y mínimo en el vector
    double min = *std::min_element(filtered.begin(), filtered.end());
    double max = *std::max_element(filtered.begin(), filtered.end());

    // Normalizar los valores
    if(normalizationEnabled){
        std::vector<double> normalized_filtered;
        for (const auto& value : filtered) {
            normalized_filtered.push_back((value - min) / (max - min));
        }

        for (int i = 0; i < vec.size(); i++) {
            vec[i].setY(normalized_filtered[i]);
        }
    }

    // Agregar los puntos a la serie
    if (chartBox->currentIndex() == 1) {
        seriesL = new QSplineSeries();
        for (int i = 0; i < vec.size(); i++) {
            seriesL->append(vec[i].x(), vec[i].y());
        }
        seriesL->setName(QString::fromStdString(legName));
        chart->addSeries(seriesL);
    } else if (chartBox->currentIndex() == 0) {
        seriesS = new QScatterSeries();
        for (int i = 0; i < vec.size(); i++) {
            seriesS->append(vec[i].x(), vec[i].y());
        }
        seriesS->setName(QString::fromStdString(legName));
        chart->addSeries(seriesS);
    }

    // Guardar los datos en el archivo .txt
    if (saveFile.is_open()) {
        saveFile << "Wavelength - Value\n";
        for (int i = 0; i < vec.size(); i++) {
            saveFile << vec.at(i).x() << " - " << vec.at(i).y() << "\n";
        }
        saveFile.close();
    }

    // Guardar los datos en el archivo .csv
    if (saveCSV.is_open()) {
        saveCSV << "Wavelength,Value\n";
        for (int i = 0; i < vec.size(); i++) {
            saveCSV << vec.at(i).x() << "," << vec.at(i).y() << "\n";
        }
        saveCSV.close();
    }

    // Predicción automática
    if(autoDetectEnabled){
        // Crear una instancia de Prediction
        QJsonObject predictionResults = prediction.makePredictions(QString::fromStdString(csvFilePath));

        // Actualizar los textos con los nuevos valores
        updatePredictionResults(predictionResults);
    }

    // Configurar los ejes del gráfico
    chart->createDefaultAxes();

    if(normalizationEnabled){
        chart->axes(Qt::Vertical).first()->setRange(0, 1.1);
    }else{
        chart->axes(Qt::Vertical).first()->setRange(0, maxvalue + maxvalue / 10);
    }

    chart->axes(Qt::Horizontal).first()->setRange(405, 945);

    if (singleBtn->isEnabled() && stopBtn->isEnabled()) { // Modo continuo activado
        timer->start();
    }

#endif // WIN32



	}
}
void mainw::showHelp() {
	QMessageBox* about = new QMessageBox; 

	//about->setIcon(QIcon(":/icons/info.png"));
	//about->setIcon(QIcon(":/icons/exit.png"));
	about->setWindowTitle(tr("Help")); 
	about->setIconPixmap(QPixmap(":/icons/info.png"));
	//about->setIcon(QMessageBox::Warning); 
	about->setText(tr("<br><b>Exit</b>: Exits the program.</br>"
		"<br><b>Start</b>: Starts data adquisition and plots the result.</br>"
		"<br><b>Stop</b>: Stops data adquisition.</br>"
		"<br><b>Single Mode</b>: Request data from the sensor once, and plots the result. Start button thiggers this action.</br>"
		"<br><b>Continuous Mode</b>: Request data and plots the results periodically.</br>"
		"<br><b>Calibrated data</b>: Data is requestedfrom the calibrated buffers.</br>"
		"<br><b>Raw data</b>: Data is requested from the raw buffers.</br>"
		"<br><b>Set gain box</b>: Sets the gain value into the sensor.</br>"
		"<br><b>Change integration time box</b>: Sets the integration time value into the sensor.</br>"
		"<br><b>Type of chart box</b>: Changes the type of chart.</br>"
		"<br><b>Clear plot</b>: Cleans the chart.</br>"
		"<br><b>Save plot</b>: Saves actual chart.</br>"
		"<br><b>Load plot</b>: Loads a presaved chart.</br>"
		"<br>Click on <b>show details</b> to know more about calibrated data, raw data, gain and integration time</br>")); 
	about->setDetailedText(tr("Calibrated data:\n"
		"Raw data:\n"
		"Gain:\n"
		"Integration time:"));


	about->show(); 

}
int  mainw::connectDevice() {
	vector <string> i2c_drivers = { "/dev/i2c-1" }; 
	connect(exitBtn, &QPushButton::released, this, &mainw::exit);
#ifdef UNIX

	for (int i = 0; i < i2c_drivers.size(); i++) {
		i2c_fd = i2c_init(i2c_drivers[i].c_str()); //init on i2c-1
		int aval = check_if_available(i2c_fd);
		if (aval != 0 && !retry->isHidden()) { //Esta conectado pero se acaba de conectar tras la comprobacion
			QString indicator = "Device connected at: ";
			device->setText(indicator.append(QString::fromStdString(i2c_drivers[i].c_str())));
			retry->hide();
			addConnections();
			disconnect(retry, &QPushButton::released, this, &mainw::connectDevice);
			as7265x_set_measurement_mode(i2c_fd, AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS);
			as7265x_set_gain(i2c_fd, AS7265X_GAIN_64X);
			as7265x_set_integration_time(i2c_fd, 200);
			gainBox->setCurrentIndex(3);
			iTimeBox->setCurrentIndex(20);

			as7265x_indicator_disable(i2c_fd);
			return 0;
		}else if(aval != 0){ 
			return 0;
		}else if (aval == 0 && i == (i2c_drivers.size() - 1)) { //Esta desconectado
			
			device->setText(tr("Device not connected"));
			retry->setText(tr("Search device")); 
			retry->show();
			connect(retry, &QPushButton::released, this, &mainw::connectDevice); 
			removeConnections(); 

			return 1; 
		}
	}
#endif // 
#ifdef WIN32
	device->setText(tr("DEBUG MODE ON WINDOWS"));
	retry->hide();
	addConnections();
	disconnect(retry, &QPushButton::released, this, &mainw::connectDevice);
	return 0;
#endif // WIN32

}

void mainw::changePath() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.size() != 0) {
		savingPath = dir.toUtf8().constData();
        defPath->setText(tr("Current path: ") + QString::fromStdString(savingPath) + "/");
	}
}
void mainw::keepLogFn() {
	if (keepLog->isChecked()) {
		preserve = true; 
	}else {
		preserve = false; 
	}

}

void mainw::toggleNormalization() {
    if (normalization->isChecked()) {
        normalizationEnabled = true;

    }else {
        normalizationEnabled = false;
    }
}

void mainw::toggleAutoDetection() {
    if (autoDetect->isChecked()) {
        autoDetectEnabled = true;
        initializeTextItems(chart);

    }else {

        autoDetectEnabled = false;

        // Borrar los textos de la grafica si existen
        if (diesel_a_percentage) {
            chart->scene()->removeItem(diesel_a_percentage);  // Eliminar el item de la escena
            delete diesel_a_percentage;  // Liberar la memoria del item
            diesel_a_percentage = nullptr;  // Establecer el puntero a null
        }

        if (diesel_a_plus_percentage) {
            chart->scene()->removeItem(diesel_a_plus_percentage);  // Eliminar el item de la escena
            delete diesel_a_plus_percentage;  // Liberar la memoria del item
            diesel_a_plus_percentage = nullptr;  // Establecer el puntero a null
        }

        if (diesel_b_percentage) {
            chart->scene()->removeItem(diesel_b_percentage);  // Eliminar el item de la escena
            delete diesel_b_percentage;  // Liberar la memoria del item
            diesel_b_percentage = nullptr;  // Establecer el puntero a null
        }

        if (diesel_c_percentage) {
            chart->scene()->removeItem(diesel_c_percentage);  // Eliminar el item de la escena
            delete diesel_c_percentage;  // Liberar la memoria del item
            diesel_c_percentage = nullptr;  // Establecer el puntero a null
        }

        if (gasolina_95_percentage) {
            chart->scene()->removeItem(gasolina_95_percentage);  // Eliminar el item de la escena
            delete gasolina_95_percentage;  // Liberar la memoria del item
            gasolina_95_percentage = nullptr;  // Establecer el puntero a null
        }

        if (gasolina_98_percentage) {
            chart->scene()->removeItem(gasolina_98_percentage);  // Eliminar el item de la escena
            delete gasolina_98_percentage;  // Liberar la memoria del item
            gasolina_98_percentage = nullptr;  // Establecer el puntero a null
        }
    }

}
