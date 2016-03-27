#ifndef LFFSORT_H
#define LFFSORT_H
#include<vector>
#include<QStringList>


struct fontItem {
    unsigned code;
    QStringList lines;
};

class LffSort
{
	static std::vector<fontItem> readFile(const QString& fileName);
	static void writeFile(QString fileName, std::vector<fontItem> const& fonts);
public:
    LffSort()=delete;
    static void sortLff(const QString& fileName);
	static void sortLff(const QStringList& fileNames);
};

#endif // LFFSORT_H
