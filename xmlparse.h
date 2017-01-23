#ifndef XMLPARSE_H
#define XMLPARSE_H
#include "Qstring"
#include "QXmlStreamReader"

class xmlParse
{
public:

    static readConfigFile(QString filePath,double &samplingRate,int &amplitude,double &timeSpan,QString &filedir);
    static bool parseVisibility(QString name);
};

#endif // XMLPARSE_H
