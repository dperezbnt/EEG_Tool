#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include "qcustomplot.h"
#include <QLineEdit>
#include "eegwindow.h"
#include "settingsdialog.h"

// TODO: CLOSE Main window when eeg window is closed
mainWindow::mainWindow(QWidget *parent)//, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    //ui->actionConnectToolBar->setEnabled(true);

//    connect(ui->refreshPushButton, SIGNAL(pressed()), this, SLOT(refreshDeviceList()));
//    connect(ui->actionConnectToolBar, &QAction::triggered, this, &mainWindow::openSerialPort);
//    connect(ui->refreshPushButton, SIGNAL(pressed()), this, SLOT(refreshDeviceList()));
//    connect(ui->toolButton, SIGNAL(pressed()), this, SLOT(settingsPressed()));
    displayInfoPorts();

    Settings = new SettingsDialog;
}

// Display and store information about the available devices
void mainWindow::displayInfoPorts()
{
   QRect rec = QApplication::desktop()->screenGeometry();
    //ui->centralwidget->setMaximumSize(rec.width(), rec.height());
    const auto infoPort = QSerialPortInfo::availablePorts();
    int widthRefresh = 200;
    QHBoxLayout *row = new QHBoxLayout();
    general = new QVBoxLayout();

    general->addLayout(row);

setLayout(general);
    //ui->refreshPushButton->setGeometry((rec.width()-widthRefresh)/2,60,widthRefresh,50);

    QLabel *portStringLabel = new QLabel(this);
    QLabel *manufacturerStringLabel = new QLabel(this);
    QLabel *serialnumberStringLabel = new QLabel(this);
    QLabel *baudRateStringLabel = new QLabel(this);
    QLabel *dataStringLabel = new QLabel(this);
    QLabel *parityStringLabel = new QLabel(this);

    QFont font = manufacturerStringLabel->font();
    font.setPointSize(14);

    portStringLabel->setText("#COM");
    portStringLabel->setFont(font);
    portStringLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 10 }");
   // portStringLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    row->addWidget(portStringLabel);

    manufacturerStringLabel->setText("Manufacturer");

    manufacturerStringLabel->setFont(font);
    manufacturerStringLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 10 }");
   // manufacturerStringLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    row->addWidget(manufacturerStringLabel);

    serialnumberStringLabel->setText("Serial Number");
    serialnumberStringLabel->setFont(font);
    serialnumberStringLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 10 }");
    //serialnumberStringLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    row->addWidget(serialnumberStringLabel);

    baudRateStringLabel->setText("Baud Rate");
    baudRateStringLabel->setFont(font);
    baudRateStringLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 10 }");
    //baudRateStringLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    row->addWidget(baudRateStringLabel);

    dataStringLabel->setText("Data");
    dataStringLabel->setFont(font);
    dataStringLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 10 }");
    //dataStringLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    row->addWidget(dataStringLabel);

    parityStringLabel->setText("Parity");
    parityStringLabel->setFont(font);
    parityStringLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 10 }");
    //parityStringLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    row->addWidget(parityStringLabel);

    int portIndex =1;     

    for (const QSerialPortInfo &info : infoPort)
    {
        QHBoxLayout *rowPort = new QHBoxLayout();

        QCheckBox *checkBox = new QCheckBox(this);

        QLabel *portLabel = new QLabel(this);
        QLabel *manufacturerLabel = new QLabel(this);
        QLabel *serialnumberLabel = new QLabel(this);
        QComboBox *baudRateBox = new QComboBox(this);
        QComboBox *dataBox = new QComboBox(this);
        QComboBox *parityBox = new QComboBox(this);

        checkBox->setFixedSize(40,40);
        checkBox->setChecked(true);
        rowPort->addWidget(checkBox);
        deviceInfo.isSelectedDevice=checkBox->isChecked();        

        connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked(bool)));

        portLabel->setText((!info.portName().isEmpty() ? info.portName() : "NULL"));
        portLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 12 }");
        rowPort->addWidget(portLabel);
        deviceInfo.name=info.portName();

        manufacturerLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 12 }");
        manufacturerLabel->setText((!info.manufacturer().isEmpty() ? info.manufacturer() : "NULL"));
        rowPort->addWidget(manufacturerLabel);

        serialnumberLabel->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 12 }");
        serialnumberLabel->setText((!info.serialNumber().isEmpty() ? info.serialNumber() : "NULL"));
        rowPort->addWidget(serialnumberLabel);
        fillBaudRateBox(baudRateBox);

        baudRateBox->setEditable(true);
        baudRateBox->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 12 }");
        baudRateBox->clearEditText();
        baudRateBox->setEditText("460800");
        baudRateBox->setFixedSize(baudRateBox->sizeHint());

        rowPort->addWidget(baudRateBox);
        deviceInfo.baudRate = (baudRateBox->itemData((baudRateBox->currentIndex()))).toInt();

        fillDataBox(dataBox);
        dataBox->setFixedSize(dataBox->sizeHint());
        dataBox->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 12 }");
        rowPort->addWidget(dataBox);
        deviceInfo.data=variantToData(dataBox->itemData((dataBox->currentIndex())));
        fillParityBox(parityBox);
        parityBox->setFixedSize(parityBox->sizeHint());
        parityBox->setStyleSheet("QLabel { color : blue; font-weight: bold; font-size: 12 }");
        rowPort->addWidget(parityBox);

        deviceInfo.parity=variantToParity(parityBox->itemData((parityBox->currentIndex())));

        portIndex ++;
        deviceList.append(deviceInfo);

        general->addLayout(rowPort);
    }

    // Display message if no device has been connected
    if (infoPort.length()==0)
    {
        QHBoxLayout *errorBox = new QHBoxLayout();
        general->addLayout(errorBox);
        QPlainTextEdit *text = new QPlainTextEdit();
        text->setPlainText("No USB device detected");
       // text->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        errorBox->addWidget(text);
    }
}

// Triggered from the connection  button
void mainWindow::openSerialPort()
{
    bool selectedDevice = false;
     for (const DeviceType  &device : deviceList)
     {
         serial = new QSerialPort(this);
         serial->setBaudRate(device.baudRate);
         serial->setPortName(device.name);
         serial->setDataBits(device.data);
         serial->setParity(device.parity);
         serial->setStopBits(device.stopBits);
         if (device.isSelectedDevice)
         {
            SerialList.append(serial);
            selectedDevice = true;
         }
     }

     bool isConnectionSucceded = false;
     for (int index =0; index <SerialList.length(); index++)
     {         
        if ((SerialList.at(index))->open(QIODevice::ReadWrite))
        {
            isConnectionSucceded = true;
        }
     }

     if (isConnectionSucceded)
     {
        EEGWindow *eegWindow = new EEGWindow(this,SerialList);
        eegWindow->showMaximized();
        //connect(eegWindow, SIGNAL(c), this, SLOT(closeMainWindow()));
     }
     else if (!selectedDevice)
     {

         QHBoxLayout *errorBox = new QHBoxLayout();
         general->addLayout(errorBox);
         QPlainTextEdit *text = new QPlainTextEdit();
         text->setPlainText("No USB device selected");
        // text->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
         errorBox->addWidget(text);
     }
}


void mainWindow::closeMainWindow()
{
this->close();
}

void mainWindow::refreshDeviceList()
{
    //clearLayout(ui->,RowGrid,ColumnGrid,true);
    displayInfoPorts();
}

void mainWindow::checkBoxClicked(bool isChecked)
{
    for (DeviceType  &device : deviceList)
        device.isSelectedDevice = isChecked;
}

void mainWindow::settingsPressed()
{
    Settings->SetFileDir(FileDir);
    Settings->show();
}

// --------------------- GUI HELPERS------------------


// Remove device list after refresh button is clicked
void mainWindow::clearLayout(QGridLayout *layout, int row, int column, bool deleteWidgets)
{
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}


void mainWindow::deleteChildWidgets(QLayoutItem *item)
{
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}


// Load default settings

void mainWindow::fillBaudRateBox(QComboBox* box)
{
    box->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    box->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    box->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    box->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    box->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    box->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    box->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    box->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    box->addItem(tr("Edit"));
}

void mainWindow::fillDataBox(QComboBox* box)
{
    box->addItem(QStringLiteral("5"), QSerialPort::Data5);
    box->addItem(QStringLiteral("6"), QSerialPort::Data6);
    box->addItem(QStringLiteral("7"), QSerialPort::Data7);
    box->addItem(QStringLiteral("8"), QSerialPort::Data8);
    box->setCurrentIndex(3);
}

QSerialPort::DataBits mainWindow::variantToData(QVariant variant)
{
    QString literal =variant.toString();
    if (literal == "5")
        return QSerialPort::Data5;
    else if (literal == "6")
        return QSerialPort::Data6;
    else if (literal == "7")
        return QSerialPort::Data7;
    else if (literal == "8")
        return QSerialPort::Data8;
}

void mainWindow::fillParityBox(QComboBox* box)
{
    box->addItem(tr("None"), QSerialPort::NoParity);
    box->addItem(tr("Even"), QSerialPort::EvenParity);
    box->addItem(tr("Odd"), QSerialPort::OddParity);
    box->addItem(tr("Mark"), QSerialPort::MarkParity);
    box->addItem(tr("Space"), QSerialPort::SpaceParity);
}

QSerialPort::Parity mainWindow::variantToParity(QVariant variant)
{
    QString literal =variant.toString();
    if (literal == "None")
        return QSerialPort::NoParity;
    else if (literal == "Even")
        return QSerialPort::EvenParity;
    else if (literal == "Odd")
        return QSerialPort::OddParity;
    else if (literal == "Mark")
        return QSerialPort::MarkParity;
    else if (literal == "Space")
        return QSerialPort::SpaceParity;
}

