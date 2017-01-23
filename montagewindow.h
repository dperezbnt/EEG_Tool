#ifndef MONTAGEWINDOW_H
#define MONTAGEWINDOW_H
#include "ui_montage.h"

class MontageWindow : public QWidget, private Ui::Montage
{
    Q_OBJECT
public:
    MontageWindow();
    MontageWindow(int numberOfChannels);
};
#endif // MONTAGEWINDOW_H
