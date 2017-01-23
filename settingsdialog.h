#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
namespace Ui {
class SettingsDialog;
}
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    void SetFileDir(QString fileDir);
    explicit SettingsDialog(QWidget *parent = nullptr);

signals:

private:
     Ui::SettingsDialog *ui;
     bool textChanged = false;

private slots:
    void cancelPressed();
    void savePressed();
    void changeText();
};

#endif // SETTINGSDIALOG_H
