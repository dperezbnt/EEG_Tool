#ifndef SETTINGS_H
#define SETTINGS_H
#include "ui_settings.h"

class SettingsWindow : public QWidget, private Ui::Settings
{
    Q_OBJECT
public:
    SettingsWindow();
    QString GetFileDir();
    void SetFileDir(QString fileDir);


    //~SettingsWindow();
    //double SamplingRate;

private:
    bool textChanged = false;// UiSettings::SettingsWindow *uiSettings;
    void changeConfigFile();

private slots:
    void cancelPressed();
    void savePressed();
    void changeText();

};
#endif // SETTINGS_H
