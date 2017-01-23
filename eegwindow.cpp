#include "eegwindow.h"
#include "ui_eegwindow.h"
#include "qcustomplot.h"
#include "reader.h"
#include "iostream"
#include "montagewindow.h"
#include "xmlparse.h"
#include "QSerialPort"

EEGWindow::EEGWindow(QWidget *parent, QList<QSerialPort*> serial) : QMainWindow(parent),ui(new Ui::EEGWindow)
{

    ui->setupUi(this);
    //Read and set settings written in config file     
    xmlParse::readConfigFile(":settings\\config.xml",SamplingRate,Amplitude,TimeSpan,FileDir);
    Serial = serial;
    DataDimension = new int[2];
    Reader::readAllData(DataDimension,FileDir);
    NumberOfChannels = *DataDimension;

    setupPlot();

    // configure scroll bars:
    ui->timeScrollBar->setRange(0, *(DataDimension+1)-TimeSpan*SamplingRate);

    // create connection between axes and scroll bars:
    connect(ui->timeScrollBar, SIGNAL(valueChanged(int)), this, SLOT(timeScrollBarChanged()));
    connect(ui->timeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(timeSpinBoxChanged()));
    connect(ui->amplitudeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(amplitudeSpinBoxChanged()));

    connect(ui->montageButton, SIGNAL(pressed()), this, SLOT(montagePressed()));
    //connect(Serial.at(0), SIGNAL(QSerialPort::readyRead()), this, SLOT(readData()));
    //connect(Serial.at(0), QSerialPort::readyRead, this, &EEGWindow::readData);
    InitialSample = ui->timeScrollBar->value();
    loadData();
}

void EEGWindow::setFileDir(QString fileDir)
{
    FileDir = fileDir;
}

EEGWindow::~EEGWindow()
{
    delete ui;
    delete DataDimension;
}

void EEGWindow::readData()
{
//    QByteArray dataSerial = Serial.at(0)->readAll();
//    QByteArray* data = &dataSerial;

//    plotData(data);
//    std::cout<<"OUT"<<std::endl;
}


void EEGWindow::setupPlot()
{
    ui->timeSpinBox->setValue(TimeSpan);
    QPen pen;
    QRect rec = QApplication::desktop()->screenGeometry();
    QRect channelando;
    channelando.setHeight(rec.height()*0.7);
    channelando.setWidth(rec.width()*0.9);
    ui->channelsLayout->setGeometry(channelando);
    QRect channelando2;
    channelando2.setHeight(rec.height()*0.2);
    channelando2.setWidth(rec.width()*0.9);
    //ui->labelHorizontalLayout->setGeometry(channelando2);
//    for (int numberChannel = 0; numberChannel < NumberOfChannels; numberChannel++)
//    {
//        QCustomPlot *eegPlot = new QCustomPlot(this);
//        ui->channelsLayout->addWidget(eegPlot);

//        //QCPAxisRect *wideAxisRect = new QCPAxisRect(eegPlot);
//        //wideAxisRect->setupFullAxesBox(true);
//        //ui->channelsLayout->addWidget(eegPlot);//plotLayout()->addElement(numberChannel, 0, wideAxisRect); // insert axis rect in first row
//        eegPlot->addGraph();
//        eegPlot->xAxis->setTickLabels(false);
//        eegPlot->yAxis->setTickLabels(false);
//        pen.setColor(QColor(0,0,1));
//        eegPlot->graph()->setPen(pen);
//       // ui->channelsLayout->addWidget(wideAxisRect);
//    }

    // set blank axis lines:
//    ui->channelsLayouy->xAxis->setTicks(true);
//    ui->plot->yAxis->setTicks(true);
//    ui->plot->xAxis->setTickLabels(true);
//    ui->plot->yAxis->setTickLabels(true);
//    ui->plot->xAxis->setLabel("Time (s)");
//    ui->plot->yAxis->setLabel("Amplitude (V)");
    // make top right axes clones of bottom left axes:

//    ui->plot->axisRect()->setupFullAxesBox(true);
//    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->plot->graph()->rescaleAxes(true);
}

void EEGWindow::loadData()
{
    int numberOfSamples = (TimeSpan+1)*SamplingRate;
    int eegValuesSize = NumberOfChannels*numberOfSamples;

    if (numberOfSamples > *(DataDimension+1))
        numberOfSamples = *(DataDimension+1);

    double array[eegValuesSize];
    double *eegvalue;
    eegvalue=array;

    Reader::getValues(eegvalue, InitialSample, numberOfSamples, NumberOfChannels,FileDir);
    plotData(eegvalue,numberOfSamples);
}

void EEGWindow::plotData(double* eegvalue, int numberOfSamples)
{
    bool timeSet = false;
    QVector<double> t(numberOfSamples);
    while (!ui->channelsLayout->isEmpty())
        ui->channelsLayout->removeItem(ui->channelsLayout->itemAt(0));
    for (int channel = 0; channel<NumberOfChannels;channel++)
    {
        QVector<double> x(numberOfSamples);
        QPen pen;
        for (int sample = 0; sample < numberOfSamples;sample++)
        {
            if (!timeSet)
            {
               // data[sample].key=(sample+InitialSample)/SamplingRate;
                t[sample]=(sample+InitialSample)/SamplingRate;
            }

            //data[sample].value=*(eegvalue + (channel*numberOfSamples)+sample);
            x[sample]=*(eegvalue + (channel*numberOfSamples)+sample);
            //x[sample] = *(&tempx + (channel*numberOfSamples)+sample);
        }

//        QCPAxisRect *wideAxisRect = new QCPAxisRect(ui->plot);
//        wideAxisRect->setupFullAxesBox(true);
        QCustomPlot *plt = new QCustomPlot(this);
        ui->channelsLayout->addWidget(plt);// TODO COMPUTE STRETCH


        plt->addGraph();
        pen.setColor(QColor(0,0,1));
        plt->graph()->setPen(pen);
        plt->graph()->setData(t, x);
        plt->xAxis->setTickLabels(false);
        plt->yAxis->setTickLabels(false);
        //plt->yAxis->setRange(0, ui->amplitudeSpinBox->value(), Qt::AlignLeft);

        //connect(plt->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
        plt->xAxis->setRange(InitialSample/SamplingRate, TimeSpan, Qt::AlignLeft);


       // ui->plot->plotLayout()->addElement(channel, 0, wideAxisRect); // insert axis rect in first row
//        QCPGraph *mainGraphCos = ui->plot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
//        mainGraphCos->data()->set(data);
//        mainGraphCos->rescaleKeyAxis();

        timeSet = true;
        //ui->plot->xAxis->setRange(InitialSample/SamplingRate, TimeSpan, Qt::AlignLeft);
        plt->yAxis->setRange(0, ui->amplitudeSpinBox->value(), Qt::AlignLeft);
        plt->replot();
    }
}

void EEGWindow::plotData(QByteArray* eegvalue)
{
    double tempx = (eegvalue->toDouble());
    int numberOfSamples=eegvalue->length();
    QVector<double> t(numberOfSamples);
    QVector<QCPGraphData> data(numberOfSamples);
    bool timeSet = false;
    for (int channel = 0; channel<NumberOfChannels;channel++)
    {       
        QVector<double> x(numberOfSamples);
           QPen pen;
        for (int sample = 0; sample < numberOfSamples;sample++)
        {
            if (!timeSet)
            {               
               // data[sample].key=(sample+InitialSample)/SamplingRate;
                t[sample]=(sample+InitialSample)/SamplingRate;
            }

            //data[sample].value=*(eegvalue + (channel*numberOfSamples)+sample);
            //x[sample]=*(eegvalue + (channel*numberOfSamples)+sample);
            x[sample] = *(&tempx + (channel*numberOfSamples)+sample);
        }

//        QCPAxisRect *wideAxisRect = new QCPAxisRect(ui->plot);
//        wideAxisRect->setupFullAxesBox(true);
        QCustomPlot *plt = new QCustomPlot(this);
         ui->channelsLayout->addWidget(plt, 20,Qt::AlignLeft);// TODO COMPUTE STRETCH
        plt->addGraph();
        pen.setColor(QColor(0,0,1));
        plt->graph()->setPen(pen);
        plt->graph(channel)->setData(t, x);
        plt->yAxis->setRange(0, ui->amplitudeSpinBox->value(), Qt::AlignLeft);

        connect(plt->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
        plt->xAxis->setRange(InitialSample/SamplingRate, TimeSpan, Qt::AlignLeft);


       // ui->plot->plotLayout()->addElement(channel, 0, wideAxisRect); // insert axis rect in first row
//        QCPGraph *mainGraphCos = ui->plot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
//        mainGraphCos->data()->set(data);
//        mainGraphCos->rescaleKeyAxis();

        timeSet = true;
        //ui->plot->xAxis->setRange(InitialSample/SamplingRate, TimeSpan, Qt::AlignLeft);
//        ui->plot->yAxis->setRange(0, ui->amplitudeSpinBox->value(), Qt::AlignLeft);
        plt->replot();
    }
}

void EEGWindow::timeScrollBarChanged()
{
    InitialSample = ui->timeScrollBar->value();
    if (InitialSample + TimeSpan*SamplingRate < *(DataDimension+1))
        loadData();

//    if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//    {
//      ui->plot->xAxis->setRange(value/SamplingRate, value/SamplingRate+TimeSpan, Qt::AlignLeft);
//      ui->plot->yAxis->setRange(0, 100, Qt::AlignLeft);
//      ui->plot->replot();
    //}
}

void EEGWindow::amplitudeSpinBoxChanged()
{
    //if (InitialSample + TimeSpan*SamplingRate < *(DataDimension+1))
        loadData();

//    if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//    {
//      ui->plot->xAxis->setRange(value/SamplingRate, value/SamplingRate+TimeSpan, Qt::AlignLeft);
//      ui->plot->yAxis->setRange(0, 100, Qt::AlignLeft);
//      ui->plot->replot();
    //}
}

void EEGWindow::xAxisChanged(QCPRange range)
{
//    ui->timeScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
//    ui->timeScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void EEGWindow::timeSpinBoxChanged()
{
    TimeSpan = ui->timeSpinBox->value()+1;
   // if (InitialSample + TimeSpan*SamplingRate < *(DataDimension+1))
        loadData();
}



void EEGWindow::montagePressed()
{
    MontageWindow *montage= new MontageWindow(NumberOfChannels);
    montage->showMaximized();
}
