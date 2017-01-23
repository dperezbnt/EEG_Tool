#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtCore/QtGlobal>
#include "qcustomplot.h"
#include "settingsdialog.h"

namespace Ui
{
    class MainWindow;
}

// Device information
struct DeviceType
{
    bool isSelectedDevice;
    QString name;
    QSerialPort::DataBits data;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    int baudRate;
};

class mainWindow:public QWidget
{
    Q_OBJECT
    public:
        explicit mainWindow(QWidget *parent = nullptr);

    private:
        Ui::MainWindow *ui;
        QList<QSerialPort*> SerialList;
        QSerialPort *serial;
        void openSerialPort();
        void displayInfoPorts();
        void clearLayout(QGridLayout *layout, int row, int column, bool deleteWidgets);
        void deleteChildWidgets(QLayoutItem *item);
        const int RowGrid =4;
        const int ColumnGrid =7;
        void fillBaudRateBox(QComboBox* box);
        void fillParityBox(QComboBox* box);
        void fillDataBox(QComboBox* box);
        QList<DeviceType> deviceList;
        DeviceType deviceInfo;
        QSerialPort::DataBits variantToData(QVariant variant);
        QSerialPort::Parity variantToParity(QVariant variant);
        SettingsDialog *Settings;
        QString FileDir;
        QVBoxLayout *general;

    private slots:
        void refreshDeviceList();
        void checkBoxClicked(bool isSelected);
        void closeMainWindow();
        void settingsPressed();

};

#endif // MAINWINDOW_H
