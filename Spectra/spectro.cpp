#include <QtGui>
#include <QApplication>

#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

#include <usb.h>
#include <iostream>
#include "spectro.h"
#include "renderarea.h"
#include "timestamp.h"
#include "predictions.h"

#include <fstream>
#include <string>
#include <QDateTime>
#include <QDir>
#include <QRandomGenerator>
#include <math.h>
#include <cstdlib>
#include <ctime>
using namespace std;

long m_DeviceHandle;
unsigned int l_Time=0;
unsigned short m_NrPixels;
double m_pLambda[MAX_NR_PIXELS];
double l_pSpectrum[MAX_NR_PIXELS];
unsigned int filterOn = 0;
unsigned int chevOn = 0;
unsigned int infiniteLoop = 0;
bool autoDetect = false;
Prediction prediction;

Spectro::Spectro(QWidget *parent)
: QDialog(parent)
{
	OpenCommBtn = new QPushButton("Open Communication");
	SerialNumLabel = new QLabel("Serial Number");
	SerialNum = new QLabel();
	CloseCommBtn = new QPushButton("Close Communication");
	StartMeasBtn = new QPushButton("Start Measurement");
	StopMeasBtn = new QPushButton("Stop Measurement");
    AutomateBtn = new QPushButton("Automate Fuel Detection");
	TimeStampLabel = new QLabel("Timestamp:");
	timeStamp = new TimeStamp;
	IntTimeLabel = new QLabel("Integration Time [ms]:");
	edtIntTime = new QLineEdit("25.0");
	NumAvgLabel = new QLabel("Number of Averages:");
	edtNumAvg = new QLineEdit("1");
	NumMeasLabel = new QLabel("Number of Measurements:");
	edtNumMeas = new QLineEdit("1");
	normalizeButton = new QCheckBox("Normalize");
	chebyButton = new QCheckBox("Smooth");
	infiButton = new QRadioButton("Non-stop mode");
	loadLog = new QPushButton("Load log");
	saveLog = new QPushButton("Save log");

    //Combustibles
    initializeTextItems();
    QHBoxLayout *fuelLayout = new QHBoxLayout;
    fuelLayout->addWidget(diesel_a_percentage);
    fuelLayout->addWidget(diesel_a_plus_percentage);
    fuelLayout->addWidget(diesel_b_percentage);
    fuelLayout->addWidget(diesel_c_percentage);
    fuelLayout->addWidget(gasolina_95_percentage);
    fuelLayout->addWidget(gasolina_98_percentage);

    updateLabelVisibility(autoDetect);

    QVBoxLayout* leftLayout = new QVBoxLayout();
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	QGridLayout *mainLayout = new QGridLayout;
	leftLayout->addWidget(OpenCommBtn);
	leftLayout->addWidget(SerialNumLabel);
	leftLayout->addWidget(SerialNum);
	leftLayout->addWidget(CloseCommBtn);
	leftLayout->addWidget(StartMeasBtn);
	leftLayout->addWidget(StopMeasBtn);
    leftLayout->addWidget(AutomateBtn);
    leftLayout->addLayout(fuelLayout);
	leftLayout->addWidget(TimeStampLabel);
	leftLayout->addWidget(timeStamp);
	leftLayout->addWidget(IntTimeLabel);
	leftLayout->addWidget(edtIntTime);
	leftLayout->addWidget(NumAvgLabel);
	leftLayout->addWidget(edtNumAvg);
	leftLayout->addWidget(infiButton);
	bottomLayout->addWidget(NumMeasLabel);
	bottomLayout->addWidget(edtNumMeas);
	bottomLayout->addWidget(chebyButton);
	bottomLayout->addWidget(normalizeButton);
	bottomLayout->addWidget(loadLog);
	bottomLayout->addWidget(saveLog);
	renderArea = new RenderArea;
	renderArea->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
	mainLayout->addWidget(renderArea,0,1,1,1,0);
	mainLayout->addLayout(leftLayout,0,0,0);
	mainLayout->addLayout(bottomLayout,1,1,0);
	setLayout(mainLayout);
	this->showFullScreen();
	connect(OpenCommBtn, SIGNAL(clicked()), this, SLOT(on_OpenCommBtn_clicked()));
	connect(CloseCommBtn, SIGNAL(clicked()), this, SLOT(on_CloseCommBtn_clicked()));
	connect(StartMeasBtn, SIGNAL(clicked()), this, SLOT(on_StartMeasBtn_clicked()));
	connect(StopMeasBtn, SIGNAL(clicked()), this, SLOT(on_StopMeasBtn_clicked()));
    connect(AutomateBtn, SIGNAL(clicked()), this, SLOT(on_AutomateBtn_clicked()));
	connect(chebyButton, SIGNAL(clicked()), this, SLOT(on_ChebyshevBtn_clicked()));
	connect(normalizeButton, SIGNAL(clicked()), this, SLOT(on_NormalizeBtn_clicked()));
	connect(infiButton, SIGNAL(clicked()), this, SLOT(on_infiBtn_clicked()));
	connect(loadLog, SIGNAL(clicked()), this, SLOT(on_loadLogBtn_clicked()));
	connect(saveLog, SIGNAL(clicked()), this, SLOT(on_saveLogBtn_clicked()));


    // Inicializacion de los punteros
    /*diesel_a_percentage = nullptr;
    diesel_a_plus_percentage = nullptr;
    diesel_b_percentage = nullptr;
    diesel_c_percentage = nullptr;
    gasolina_95_percentage = nullptr;
    gasolina_98_percentage = nullptr;*/
}

Spectro::~Spectro()
{

}

usb_dev_handle *open_dev(void);

usb_dev_handle *open_dev(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;

	for(bus = usb_get_busses(); bus; bus = bus->next)
	{
		for(dev = bus->devices; dev; dev = dev->next)
		{
			if(dev->descriptor.idVendor == USB_VENDOR
				&& dev->descriptor.idProduct == USB_PRODUCT)
				{
					return usb_open(dev);
				}
			}
		}
		return NULL;
	}

	float swapsingle(float floatin)
	{
		union s
		{
			char sa[4];
			float res;
		} temp;
		temp.res = floatin;
		s floatout;
		for (int teller=0; teller<4; ++teller){
			floatout.sa[teller]=temp.sa[3-teller];
		}
		return floatout.res;
	}

	uint32 swap32(uint32 uint32in)
	{
		union s
		{
			char sa[4];
			uint32 res;
		} temp;
		temp.res = uint32in;
		s uint32out;
		for (int teller=0; teller<4; ++teller){
			uint32out.sa[teller]=temp.sa[3-teller];
		}
		return uint32out.res;
	}

	uint16 swap16(uint16 uint16in)
	{
		union s
		{
			char sa[2];
			uint16 res;
		} temp;
		temp.res = uint16in;
		s uint16out;
		for (int teller=0; teller<2; ++teller){
			uint16out.sa[teller]=temp.sa[1-teller];
		}
		return uint16out.res;
	}

	void Spectro::on_OpenCommBtn_clicked()
	{
		unsigned char bufout[6];
		unsigned char bufout2[6];
		usb_init();
		usb_find_busses();
		usb_find_devices();

		if (!(dev = open_dev()))
		QMessageBox::critical(this, tr("Spectro"),
		tr("Device not found"),
		QMessageBox::Ok, QMessageBox::Ok);
		if (usb_set_configuration(dev, 1) < 0){
			QMessageBox::critical(this, tr("Spectro"),
			tr("Error in usb_set_configuration"),
			QMessageBox::Ok, QMessageBox::Ok);
			usb_close(dev);
		}
		if (usb_claim_interface(dev, 0) < 0){
			QMessageBox::critical(this, tr("Spectro"),
			tr("Error in usb_claim_interface"),
			QMessageBox::Ok, QMessageBox::Ok);
			usb_close(dev);
		}
		bufout[00]=0x20;
		bufout[01]=0x00;
		bufout[02]=0x02;  // length of the command
		bufout[03]=0x00;
		bufout[04]=0x13;  // get_ident
		bufout[05]=0x00;

		if (usb_bulk_write(dev,0x02,(char *) bufout,sizeof(bufout),5000) < 0)
		QMessageBox::critical(this, tr("Spectro"),
		tr("Bulk write failed"),
		QMessageBox::Ok, QMessageBox::Ok);
		AvsIdentityType Avs_Id;
		if (usb_bulk_read(dev,0x86,(char*) &Avs_Id,sizeof(Avs_Id),5000) < 0)
		QMessageBox::critical(this, tr("Spectro"),
		tr("Bulk read failed"),
		QMessageBox::Ok, QMessageBox::Ok);
		else SerialNum->setText(QString(Avs_Id.SerialNumber).left(9));
		bufout2[00]=0x20;
		bufout2[01]=0x00;
		bufout2[02]=0x02;  // length of the command
		bufout2[03]=0x00;
		bufout2[04]=0x01;  // get_device_configuration
		bufout2[05]=0x00;
		if (usb_bulk_write(dev,0x02,(char *)bufout2,sizeof(bufout2),5000) < 0)
			QMessageBox::critical(this, tr("Spectro"),
			tr("Bulk write failed"),
			QMessageBox::Ok, QMessageBox::Ok);
		DeviceConfigType devcon;
		if (usb_bulk_read(dev,0x86,(char*) &devcon,sizeof(devcon),5000) < 0)
			QMessageBox::critical(this, tr("Spectro"),
			tr("Bulk read failed"),
			QMessageBox::Ok, QMessageBox::Ok);
		else {
			for (int teller=0;teller<NR_WAVELEN_POL_COEF;++teller) {
				fit[teller]=swapsingle(devcon.m_Detector.m_aFit[teller]);
			}
			extern unsigned short m_NrPixels;
			extern double m_pLambda[MAX_NR_PIXELS];
			m_NrPixels=swap16(devcon.m_Detector.m_NrPixels);
			for (int teller=0;teller<m_NrPixels;++teller) {
				m_pLambda[teller]=	fit[0] +
				fit[1]*teller*1.0 +
				fit[2]*teller*teller*1.0 +
				fit[3]*teller*teller*teller*1.0 +
				fit[4]*teller*teller*teller*teller*1.0;

			}
		}
		usb_release_interface(dev,0);
		usb_reset(dev);
	}

	void Spectro::on_CloseCommBtn_clicked()
	{
		usb_release_interface(dev,0);
	  usb_close(dev);
	}

    void Spectro::on_AutomateBtn_clicked()
    {
        autoDetect=!autoDetect;
        updateLabelVisibility(autoDetect);

    }

	void Spectro::on_StartMeasBtn_clicked()
	{
		unsigned char bufin[63483];
		unsigned char bufout2[8];
		unsigned char bufout3[6];
		uint32 numavg;
		bool ok;
		SendMeasConfigType l_PrepareMeasData;
		sony_single_measdatatype sony_single_meas;
		sony_multi_measdatatype sony_multi_meas;
		short l_NrOfScans = QString(edtNumMeas->text()).toInt(&ok);

		l_PrepareMeasData.prefix[00]=0x20;
		l_PrepareMeasData.prefix[01]=0x00;
		l_PrepareMeasData.prefix[02]=0x2B;   // length of the command
		l_PrepareMeasData.prefix[03]=0x00;
		l_PrepareMeasData.prefix[04]=0x05;   // prepare_measurement
		l_PrepareMeasData.prefix[05]=0x00;
		l_PrepareMeasData.m_Meas.m_StartPixel = 0;
		extern unsigned short m_NrPixels;
		l_PrepareMeasData.m_Meas.m_StopPixel = swap16(m_NrPixels-1);
		QLocale::setDefault(QLocale::Spanish);
		double inttime = QString(edtIntTime->text()).toDouble(&ok);
		l_PrepareMeasData.m_Meas.m_IntegrationTime = swapsingle(inttime);
		l_PrepareMeasData.m_Meas.m_IntegrationDelay = 0;
		numavg = QString(edtNumAvg->text()).toInt(&ok);
		l_PrepareMeasData.m_Meas.m_NrAverages = swap32(numavg);
		l_PrepareMeasData.m_Meas.m_CorDynDark.m_Enable = 0;
		l_PrepareMeasData.m_Meas.m_CorDynDark.m_ForgetPercentage = 0;
		l_PrepareMeasData.m_Meas.m_Smoothing.m_SmoothPix = 0;
		l_PrepareMeasData.m_Meas.m_Smoothing.m_SmoothModel = 0;
		l_PrepareMeasData.m_Meas.m_SaturationDetection = 0;
		l_PrepareMeasData.m_Meas.m_Trigger.m_Mode = 0;
		l_PrepareMeasData.m_Meas.m_Trigger.m_Source = 0;
		l_PrepareMeasData.m_Meas.m_Trigger.m_SourceType = 0;
		l_PrepareMeasData.m_Meas.m_Control.m_StrobeControl = 0;
		l_PrepareMeasData.m_Meas.m_Control.m_LaserDelay = 0;
		l_PrepareMeasData.m_Meas.m_Control.m_LaserWidth = 0;
		l_PrepareMeasData.m_Meas.m_Control.m_LaserWaveLength = 0;
		l_PrepareMeasData.m_Meas.m_Control.m_StoreToRam = 0;

		usb_init();
		usb_find_busses();
		usb_find_devices();

		// usb_dev_handle *dev = NULL; /* the device handle */
		if (!(dev = open_dev()))
			QMessageBox::critical(this, tr("Spectro"),
			tr("Device not found"),
			QMessageBox::Ok, QMessageBox::Ok);
		if (usb_set_configuration(dev, 1) < 0){
			QMessageBox::critical(this, tr("Spectro"),
			tr("Error in usb_set_configuration"),
			QMessageBox::Ok, QMessageBox::Ok);
			usb_close(dev);
		}
		if (usb_claim_interface(dev, 0) < 0){
			QMessageBox::critical(this, tr("Spectro"),
			tr("Error in usb_claim_interface"),
			QMessageBox::Ok, QMessageBox::Ok);
			usb_close(dev);
		}
		int pmsize = sizeof(l_PrepareMeasData);
		if (usb_bulk_write(dev,0x02,(char *)&l_PrepareMeasData,pmsize,5000) != pmsize)
			QMessageBox::critical(this, tr("Spectro"),tr("Bulk write failed"),QMessageBox::Ok, QMessageBox::Ok);
		int retval;
		retval = usb_bulk_read(dev,0x86,(char*) bufin,6,5000);
		if (retval < 0)
			QMessageBox::critical(this, tr("Spectro"),tr("Bulk read failed (prepare_measurement)"),QMessageBox::Ok, QMessageBox::Ok);
		else {
			if ((retval!= 6 ) || (bufin[4]!=0x85))
				QMessageBox::critical(this, tr("Spectro"),
				tr("Error in prepare_measurement"),
				QMessageBox::Ok, QMessageBox::Ok);
		}
		do {
		QCoreApplication::processEvents();
		//QApplication::processEvents();
		bufout2[00]=0x20;
		bufout2[01]=0x00;
		bufout2[02]=0x04;   // length of the command
		bufout2[03]=0x00;
		bufout2[04]=0x06;   // start_measurement
		bufout2[05]=0x00;
		bufout2[06]=HIBYTE(l_NrOfScans);
		bufout2[07]=LOBYTE(l_NrOfScans);

		if (usb_bulk_write(dev,0x02,(char *)bufout2,sizeof(bufout2),5000) < 0)
			QMessageBox::critical(this, tr("Spectro"),
			tr("Bulk write failed"),
			QMessageBox::Ok, QMessageBox::Ok);
		retval = usb_bulk_read(dev,0x86,(char *)bufin,6,5000);
		if (retval < 0)
			QMessageBox::critical(this, tr("Spectro"),
			tr("Bulk read failed (start_measurement)"),
			QMessageBox::Ok, QMessageBox::Ok);
		else {
			if ((retval!= 6 ) || (bufin[4]!=0x86))
				QMessageBox::critical(this, tr("Spectro"),
				tr("Error in Start_measurement"),
				QMessageBox::Ok, QMessageBox::Ok);
		}
		extern unsigned int l_Time;
		extern double l_pSpectrum[MAX_NR_PIXELS];
		int request_size;
		if (numavg <=1)
			request_size = sizeof(sony_single_meas);
		else
			request_size = sizeof(sony_multi_meas);
		int retval2;
		int measnr=0;

		while (measnr < l_NrOfScans) {

			retval2 = usb_bulk_read(dev,0x86,(char *) &bufin,request_size,5000);

			if (retval2 < 0) {
				QMessageBox::critical(this, tr("Spectro"),
				tr("Bulk read failed (measurement)"),
				QMessageBox::Ok, QMessageBox::Ok);
			}

			else switch (bufin[4])
			{

				case 0xB0:
				if (retval2 == sizeof(sony_single_meas)) {
					memcpy(&sony_single_meas,bufin,sizeof(sony_single_meas));
					l_Time = swap32(sony_single_meas.timestamp);
					for (int teller=0;teller<m_NrPixels;++teller){
						l_pSpectrum[teller]=(double)swap16(sony_single_meas.pixels[teller]);
					}

					for (int i=m_NrPixels-60;i<m_NrPixels;i++){
						l_pSpectrum[i]=l_pSpectrum[i-120];
					}

					if (filterOn){

						double auxArray[MAX_NR_PIXELS];
						double minimo  = l_pSpectrum[0];
						double maximo  = l_pSpectrum[0];

						for (int teller=0;teller<m_NrPixels;++teller){
							if (minimo > l_pSpectrum[teller]){
							minimo = l_pSpectrum[teller];}
						}
						for (int teller=0;teller<m_NrPixels;++teller){
							if (maximo < l_pSpectrum[teller]){
							maximo = l_pSpectrum[teller];}
						}
						for (int teller=0;teller<m_NrPixels;++teller){
							auxArray[teller] = l_pSpectrum[teller] - minimo;
						}

						for (int teller=0;teller<m_NrPixels;++teller){
							l_pSpectrum[teller] = auxArray[teller] / (maximo - minimo);
						}
					}
					if (chevOn){
						double newPix[MAX_NR_PIXELS];
						int window = 30;
						double numerator = 0.0;
						for (int i = 0;i<m_NrPixels;i++){
							newPix[i] = l_pSpectrum[i];
						}

						for (int i = window;i<m_NrPixels-window;i++){
							numerator = 0.0;
							for (int j = 1 ;j<=window;j++){
								numerator +=l_pSpectrum[i+j]+l_pSpectrum[i-j];
							}
							newPix[i]=numerator/(window*2);
						}

						for (int i = 0;i<m_NrPixels;i++){
							l_pSpectrum[i] = newPix[i];
						}

						double ave = l_pSpectrum[window];
						for (int i=0;i<window;i++) {
							l_pSpectrum[i]=(5*ave+l_pSpectrum[i])/6;

						}
						ave = l_pSpectrum[m_NrPixels-window-1];
						printf("BEFORE SUBSRTACION value: %f\n",l_pSpectrum[m_NrPixels-window-1]);
						printf("BEFORE 2016 value: %f\n",l_pSpectrum[2016]);
						printf("BEFORE 2017 value: %f\n",l_pSpectrum[2017]);
						printf("BEFORE 2018 value: %f\n",l_pSpectrum[2018]);
						printf("BEFORE 2019 value: %f\n",l_pSpectrum[2019]);
						for (int i=m_NrPixels-1;i>=m_NrPixels-window;i--) {
							l_pSpectrum[i]=(5*ave+l_pSpectrum[i])/6;
						}
						printf("AFTER 2016 value: %f\n",l_pSpectrum[2016]);
						printf("AFTER 2017 value: %f\n",l_pSpectrum[2017]);
						printf("AFTER 2018 value: %f\n",l_pSpectrum[2018]);
						printf("AFTER 2019 value: %f\n",l_pSpectrum[2019]);


					}
				}
                saveCSVData();

				break;
				case 0xB1:
				memcpy(&sony_multi_meas,bufin,sizeof(sony_multi_meas));
				l_Time = swap32(sony_multi_meas.timestamp);
				if (numavg != swap16(sony_multi_meas.averages)){
					QMessageBox::critical(this, tr("Spectro"),
					tr("Error in Number of Averages"),
					QMessageBox::Ok, QMessageBox::Ok);
				}else {
						for (int teller=0;teller<m_NrPixels;++teller){
							l_pSpectrum[teller]=swap32(sony_multi_meas.pixels[teller])/numavg;
						}

						for (int i=m_NrPixels-60;i<m_NrPixels;i++){
							l_pSpectrum[i]=l_pSpectrum[i-120];
						}

						if (filterOn){
							double auxArray[MAX_NR_PIXELS];
							double minimo  = l_pSpectrum[0];
							double maximo  = l_pSpectrum[0];

							for (int teller=0;teller<m_NrPixels;++teller){
								if (minimo > l_pSpectrum[teller]){
								minimo = l_pSpectrum[teller];}
							}
							for (int teller=0;teller<m_NrPixels;++teller){
								if (maximo < l_pSpectrum[teller]){
								maximo = l_pSpectrum[teller];}
							}
							for (int teller=0;teller<m_NrPixels;++teller){
								auxArray[teller] = l_pSpectrum[teller] - minimo;
							}

							for (int teller=0;teller<m_NrPixels;++teller){
								l_pSpectrum[teller] = auxArray[teller] / (maximo - minimo);
							}
						}
						if (chevOn){
							double newPix[MAX_NR_PIXELS];
							int window = 30;
							double numerator = 0.0;
							for (int i = 0;i<m_NrPixels;i++){
								newPix[i] = l_pSpectrum[i];
							}

							for (int i = window;i<m_NrPixels-window;i++){
								numerator = 0.0;
								for (int j = 1 ;j<=window;j++){
									numerator +=l_pSpectrum[i+j]+l_pSpectrum[i-j];
								}
								newPix[i]=numerator/(window*2);
							}

							for (int i = 0;i<m_NrPixels;i++){
								l_pSpectrum[i] = newPix[i];
							}

							double ave = l_pSpectrum[window];
							for (int i=0;i<window;i++) {
								l_pSpectrum[i]=((5*ave)+l_pSpectrum[i])/6;

							}
							ave = l_pSpectrum[m_NrPixels-window];
							printf("BEFORE SUBSRTACION value: %f\n",l_pSpectrum[m_NrPixels-window]);
							printf("BEFORE 2017 value: %f\n",l_pSpectrum[2017]);
							printf("BEFORE 2018 value: %f\n",l_pSpectrum[2018]);
							for (int i=m_NrPixels-1;i>m_NrPixels-window;i--) {
								l_pSpectrum[i]=((5*ave)+l_pSpectrum[i])/6;
							}
							printf("AFTER 2017 value: %f\n",l_pSpectrum[2017]);
							printf("AFTER 2018 value: %f\n",l_pSpectrum[2018]);
						}
				}
                saveCSVData();
				break;
			}
			++measnr;
			renderArea->repaint();
			timeStamp->repaint();

			if (measnr<l_NrOfScans){
				bufout3[00]=0x21;
				bufout3[01]=0x00;
				bufout3[02]=0x02;  // length of the command
				bufout3[03]=0x00;
				bufout3[04]=0xC0;  // acknowledge
				bufout3[05]=0x00;
				if (usb_bulk_write(dev,0x02,(char *) bufout3,sizeof(bufout3),5000)< 0)
				QMessageBox::critical(this, tr("Spectro"),
				tr("Writing acknowledgement to COM1 failed"),
				QMessageBox::Ok, QMessageBox::Ok);
			}
		} // while l_NrOfScans
	}while(infiButton->isChecked());
		usb_release_interface(dev,0);
		usb_reset(dev);
	}


	void Spectro::on_StopMeasBtn_clicked()
	{
		infiniteLoop = 0;
		infiButton->setChecked(false);
		//usb_release_interface(dev,0);
		//usb_reset(dev);
	}

	void Spectro::on_loadLogBtn_clicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Log"), "",
        tr("Saved logs (*.log);;All Files (*)"));
		if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);
				for (int i=0;i<m_NrPixels;i++)
					in >> l_pSpectrum[i];
    }
	}

	void Spectro::on_saveLogBtn_clicked()
	{
		QString fileName = QFileDialog::getSaveFileName(this,
			 tr("Save log"), "",
			 tr("Logs (*.log);;All Files (*)"));

	 if (fileName.isEmpty())
			 return;
	 else {
			 QFile file(fileName);
			 if (!file.open(QIODevice::WriteOnly)) {
					 QMessageBox::information(this, tr("Unable to open file"),
							 file.errorString());
					 return;
			 }

			 QDataStream out(&file);
			 out.setVersion(QDataStream::Qt_4_5);
			 for (int i=0;i<m_NrPixels;i++)
			 	out << l_pSpectrum[i];
	 }
	}

	void Spectro::on_ChebyshevBtn_clicked()
	{
		if (chebyButton->isChecked())
			chevOn = 1;
		else
		{
			chevOn = 0;
		}
	}

	void Spectro::on_NormalizeBtn_clicked()
	{
		if (normalizeButton->isChecked())
			filterOn = 1;
		else
		{
			filterOn = 0;
		}
	}

	void Spectro::on_infiBtn_clicked()
	{
		if (infiButton->isChecked()){
			infiniteLoop = 1;
		}else
		{
			infiniteLoop = 0;
		}
	}

    void Spectro::saveCSVData() {
        QString basePath = QCoreApplication::applicationDirPath();
        QDir dir(basePath); dir.cdUp(); QString savingPath = dir.absoluteFilePath("OutputData");
        QString legName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        qDebug() << "legname:" << legName;

        srand(time(0));

        int randomNum = rand() % 1001;
        QString csvFilePath = savingPath + "/Data_" + legName + "_" + QString::number(randomNum) + ".csv";
        std::ofstream saveCSV(csvFilePath.toStdString()); // Archivo CSV
        if (saveCSV.is_open()) {
            saveCSV << "Wavelength,Value\n";
            std::vector<double> wavelengths = {410.123758, 435.411255, 460.050202, 485.21249, 510.309324, 535.339285};
            for (double wavelength : wavelengths) {
                int roundedWavelength = round(wavelength); // Redondear la longitud de onda
                for (int i = 0; i < m_NrPixels; i++) {
                    if (round(m_pLambda[i]) == roundedWavelength) {
                        saveCSV << roundedWavelength << "," << l_pSpectrum[i] << "\n";
                        break; // Salir del bucle una vez encontrado el indice
                    }
                }
            }
            saveCSV.close();
        }

        // Prediccion automatica
        if(autoDetect){
            // Crear una instancia de Prediction
            QJsonObject predictionResults = prediction.makePredictions(csvFilePath);

            // Actualizamos los textos con los nuevos valores
            updatePredictionResults(predictionResults);

        }


    }

    void Spectro::initializeTextItems() {

        // Inicializar los QLabels con estilos de fuente y color
        diesel_a_percentage = new QLabel("Diesel A: 0%", this);
        diesel_a_percentage->setStyleSheet("QLabel { color: red; font-size: 10pt; }");
        diesel_a_plus_percentage = new QLabel("Diesel A+: 0%", this);
        diesel_a_plus_percentage->setStyleSheet("QLabel { color: blue; font-size: 10pt; }");
        diesel_b_percentage = new QLabel("Diesel B: 0%", this);
        diesel_b_percentage->setStyleSheet("QLabel { color: green; font-size: 10pt; }");
        diesel_c_percentage = new QLabel("Diesel C: 0%", this);
        diesel_c_percentage->setStyleSheet("QLabel { color: gray; font-size: 10pt; }");
        gasolina_95_percentage = new QLabel("Gasoline 95: 0%", this);
        gasolina_95_percentage->setStyleSheet("QLabel { color: magenta; font-size: 10pt; }");
        gasolina_98_percentage = new QLabel("Gasoline 98: 0%", this);
        gasolina_98_percentage->setStyleSheet("QLabel { color: cyan; font-size: 10pt; }");


    }

    void Spectro::updatePredictionResults(const QJsonObject& predictionResults) {
        // Extraer los porcentajes de los combustibles del QJsonObject
        double diesel_a = predictionResults["Diesel A"].toDouble();
        double diesel_a_plus = predictionResults["Diesel A+"].toDouble();
        double diesel_b = predictionResults["Diesel B"].toDouble();
        double diesel_c = predictionResults["Diesel C"].toDouble();
        double gasolina_95 = predictionResults["Gasolina 95"].toDouble();
        double gasolina_98 = predictionResults["Gasolina 98"].toDouble();

        // Actualizar los textos con los nuevos valores en los QLabel
        diesel_a_percentage->setText(QString("Diesel A: %1%").arg(diesel_a));
        diesel_a_plus_percentage->setText(QString("Diesel A+: %1%").arg(diesel_a_plus));
        diesel_b_percentage->setText(QString("Diesel B: %1%").arg(diesel_b));
        diesel_c_percentage->setText(QString("Diesel C: %1%").arg(diesel_c));
        gasolina_95_percentage->setText(QString("Gasoline 95: %1%").arg(gasolina_95));
        gasolina_98_percentage->setText(QString("Gasoline 98: %1%").arg(gasolina_98));

    }

    void Spectro::updateLabelVisibility(bool visible) {
        diesel_a_percentage->setVisible(visible);
        diesel_a_plus_percentage->setVisible(visible);
        diesel_b_percentage->setVisible(visible);
        diesel_c_percentage->setVisible(visible);
        gasolina_95_percentage->setVisible(visible);
        gasolina_98_percentage->setVisible(visible);
    }

	int main(int argc, char *argv[])
	{
		QApplication app(argc, argv);
		Spectro *w = new Spectro;
		w->show();
		app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
		return app.exec();
	}
