#include "settingsdialog.h"
#include "ui_settingsDialog.h"
// TODO LOAD PATH WRITTEN IN CONFIG FILE
QT_USE_NAMESPACE
SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(ui->dataDirTextEdit, SIGNAL(textChanged()), this, SLOT(changeText()));
    //ui->dataDirTextEdit->
}

void SettingsDialog::savePressed()
{
    if(textChanged)
        //changeConfigFile();

    this->close();
}

void SettingsDialog::cancelPressed()
{
    this->close();
}

// Simple function called from the text changed event
void SettingsDialog::changeText()
{
    textChanged = true;
}

void SettingsDialog::SetFileDir(QString fileDir)
{
    ui->dataDirTextEdit->setPlainText(fileDir);
}
