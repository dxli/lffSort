/**
  * sort characters in a LFF file
  * Usage:: lffSort foo.lff
  * output in file: foo-output.lff
  * will overwrite existing output file
  */
#include <QCoreApplication>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include "lffsort.h"

void usageMsg()
{
    qDebug()<<"Usage:: lffSort foo.lff";
    qDebug()<<"to sort the characters in foo.lff, and the output will be written to foo-output.lff";
    exit(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString fileName("unicode.lff");
    if(argc==1 && !QFileInfo(fileName).exists()) usageMsg();

    if(argc>1) {
        QString str(argv[1]);
        if(str.endsWith(R"(.lff)", Qt::CaseInsensitive) && QFileInfo(str).exists()){
            fileName=str;
        } else usageMsg();
    }
    if(!QFileInfo(fileName).exists()) exit(0);
    qDebug()<<"Start sorting of "<<fileName;
    LffSort::sortLff(fileName);
    exit(0);

    return a.exec();
}
