#ifndef LFFSORT_H
#define LFFSORT_H
#include<QStringList>


struct fontItem {
    unsigned code;
    QStringList lines;
};

class LffSort
{
public:
    LffSort()=delete;
    static void sortLff(const QString& fileName);
};

#endif // LFFSORT_H
