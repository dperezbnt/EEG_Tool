#include "reader.h"
#include <QFile>
#include <QTextStream>

void Reader::getValues(double *eegvalue, int initialValue, int samplesPerChannel, int numberOfChannels, QString fileDir)
{
    QFile file (fileDir);
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       QString line;
       for(int sample =0; sample < samplesPerChannel+ initialValue; sample++)
       {

          line = in.readLine();
          if (sample < initialValue)
            continue;

          QStringList list = line.split(QRegExp("\\t"));
          for(int channelidx =0; channelidx < numberOfChannels; channelidx++)
          {
             *(eegvalue +(sample-initialValue)+channelidx*samplesPerChannel) = QString(list.takeFirst()).toDouble();
          }

       }

       file.close();
    }
}

void Reader::readAllData(int *dataDimension,QString fileDir)
{
    int timeCol=0;
    QFile file (fileDir);
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       QString line;
        bool read = true;
       while (!in.atEnd())
       {
          line = in.readLine();
          if (read)
          {
              QStringList list = line.split(QRegExp("\\t"));
              *dataDimension = list.count();
              read = false;
          }

          timeCol++;
       }
       file.close();
    }
    else
    {
        // TODO THROW AN EXCEPTION AND CATCH IT IN EEGWINDOW TO DISPLAY ERROR MASSAGE
    }
     *(dataDimension+1) =timeCol;
}
