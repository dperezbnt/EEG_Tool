#include "montagewindow.h"
#include "ui_montage.h"
#include "qcustomplot.h"
MontageWindow::MontageWindow(int numberOfChannels)
{
    setupUi(this);
    for (int channel=0; channel< numberOfChannels; channel++)
    {
        QCheckBox *checkBox = new QCheckBox(this);
        QLabel *label = new QLabel(this);
        label->setText("Channel " + QString::number(channel +1));
        checkBox->setChecked(true);
        QComboBox *combo = new QComboBox(this);
        //combo->setAccessibleName("channelType");
        combo->addItem("channelType");
        grid->addWidget(checkBox,channel,0,numberOfChannels,3,Qt::AlignCenter);
        grid->addWidget(label,channel,1,numberOfChannels,3,Qt::AlignCenter);
        grid->addWidget(combo,channel,2,numberOfChannels,3,Qt::AlignCenter);
    }

}


