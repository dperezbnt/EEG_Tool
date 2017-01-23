#ifndef READER_H
#define READER_H

#include <QString>

class Reader
{
public:
    static void getValues(double *array,int initialSample,int size, int numberOfChannles, QString fileDir);
    static void readAllData(int *datadimension, QString fileDir);
};

#endif // READER_H
