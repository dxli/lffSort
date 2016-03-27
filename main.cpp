/**
  * sort characters in a LFF file
  * Usage:: lffSort foo.lff
  * output in file: foo-output.lff
  * will overwrite existing output file
  */
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include "lffsort.h"

void usageMsg()
{
	qDebug()<<"Usage:: lffSort foo1.lff foo2.lff output.lff";
	qDebug()<<"to sort the characters in lff files, and the output will be written to output.lff";
    exit(0);
}

int main(int argc, char *argv[])
{
    QString fileName("unicode.lff");
    if(argc==1 && !QFileInfo(fileName).exists()) usageMsg();

	QStringList files;
	for (int i = 1; i< argc; i++)
		files << QString{argv[i]};

	if(!(files.size() && QFileInfo(files.front()).exists())) {
		usageMsg();
		exit(0);
	}
	qDebug()<<"Start sorting of "<<files;
	LffSort::sortLff(files);
    return 0;
}
