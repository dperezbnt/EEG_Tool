#ifndef EEGWINDOW_H
#define EEGWINDOW_H

#include <QMainWindow>
#include "QCustomPlot/qcustomplot.h"

#include "montagewindow.h"
#include "QSerialPort"

namespace Ui {
class EEGWindow;
}

class SettingsDialog;

class EEGWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EEGWindow(QWidget *parent = 0, QList<QSerialPort*> serial = QList<QSerialPort*>());
    ~EEGWindow();
    double SamplingRate;
    double TimeSpan;
    int NumberOfChannels;
    void setFileDir(QString fileDir);
    int Amplitude;

private:
    Ui::EEGWindow *ui;
    void setupPlot();
    void loadData();
    void plotData(QByteArray *data);
    void plotData(double *data, int numberOfSamples);
    int *DataDimension;
    int InitialSample;
    QString FileDir;
    QList<QSerialPort*> Serial;



private slots:
  void timeScrollBarChanged();
  void timeSpinBoxChanged();
  void amplitudeSpinBoxChanged();
  void xAxisChanged(QCPRange range);

  void montagePressed();
  void readData();

};

#endif // EEGWINDOW_H
