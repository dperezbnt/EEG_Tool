#include "settings.h"
#include "ui_settings.h"
#include "qcustomplot.h"

SettingsWindow::SettingsWindow()
{
    setupUi(this);

    connect(cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(dataDirTextEdit, SIGNAL(textChanged()), this, SLOT(changeText()));
}


QString SettingsWindow::GetFileDir()
{
    return this->dataDirTextEdit->toPlainText();
}

void SettingsWindow::SetFileDir(QString fileDir)
{
    dataDirTextEdit->setPlainText(fileDir);
}
void SettingsWindow::savePressed()
{
    if(textChanged)
        changeConfigFile();

    samplingRateSpinBox->setValue(samplingRateSpinBox->value());    
    this->close();
}

void SettingsWindow::cancelPressed()
{
    this->close();
}

// Simple function called from the text changed event
void SettingsWindow::changeText()
{
    textChanged = true;
}

// Read and modify the xml file if the text has been changed
void SettingsWindow::changeConfigFile()
{
    QFile file("C:\\Users\\David\\Documents\\EEG_Tool\\copy.txt");

    QByteArray fileData;
    file.open(QIODevice::ReadWrite);
    fileData = file.readAll();
    QString text(fileData);
     QString final = "<dataFileAddress>";
     int beginString = text.indexOf("<dataFileAddress>") + final.length();
     int endString = text.indexOf("</dataFileAddress>",beginString);
     int lengthString = endString-beginString;
     if (lengthString > 2 )
        text.remove(beginString,lengthString);

//    final.append(dataDirTextEdit->toPlainText());
//    text.replace(QString("<dataFileAddress>"), final);
    text.insert(beginString,dataDirTextEdit->toPlainText());
    file.seek(0);
    file.write(text.toUtf8());
    file.close();
   }


