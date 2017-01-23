#include "xmlparse.h"
#include "qfile.h"
#include "QXmlStreamReader"
#include "iostream"
#include "channelinfo.h"
#include <QDir>

xmlParse::readConfigFile(QString filePath, double &samplingRate,int &amplitude,double &timeSpan,QString &fileDir)
{
    QFile file(filePath);
    QList<channelInfo*> channelSettings;
    if(file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);

        while (!xmlReader.isEndDocument())
        {
            if (xmlReader.readNextStartElement())
            {
                QString name = xmlReader.name().toString();
                if  (name == "channel")
                {
                    channelInfo *channel = new channelInfo();
                    channel->channelName = xmlReader.attributes().value("name").toString();
                    channel->channelType = xmlReader.attributes().value("type").toString();
                    channel->isVisible = parseVisibility(xmlReader.attributes().value("enabled").toString());

                    channelSettings.append(channel);
                }

                else if  (name == "samplingRate")
                   samplingRate =   xmlReader.readElementText().toDouble();

                else if  (name == "amplitude")
                    amplitude =xmlReader.readElementText().toInt();

                else if  (name == "timeSpan")
                    timeSpan = xmlReader.readElementText().toDouble();

                else if  (name == "dataFileAddress")
                {
                    QString dataFileAddress = xmlReader.readElementText();

                    if(dataFileAddress.isEmpty())
                        fileDir = QDir::currentPath().append("/testData.txt");
                    else
                        fileDir = dataFileAddress;
                }

                else if (xmlReader.isEndElement())
                {
                    xmlReader.readNext();
                }

            }
            else if (xmlReader.hasError())
            {
                break;
            }

        }
        file.close();
    }
}

bool xmlParse::parseVisibility(QString name)
{
    if (name == "False")
        return false;
    else
        return true;
}

