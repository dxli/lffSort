#include "lffsort.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>

std::vector<fontItem> LffSort::readFile(const QString& fileName)
{
	if (!QFileInfo(fileName).exists()) return {};
	QFile fontFile(fileName);

	if (!fontFile.open(QFile::ReadOnly)) return {};
	QTextStream in1(&fontFile);
	in1.setCodec(R"(UTF-8)");
	//header
	QString line;
	std::vector<fontItem> items;
	fontItem item{0, {}};
	while( !in1.atEnd()){
		line=in1.readLine();
		if(!line.contains(QRegExp(R"(^\s*#)"))) break;
		item.lines.append(line);
	}

	items.push_back(item);

	while(!in1.atEnd() && line.contains(QRegExp(R"(^\s*$)")))
		line=in1.readLine();


	do{
		while(!in1.atEnd() && !line.contains(QRegExp(R"(^\s*\[.*$)")))
			line=in1.readLine();
		QStringList fields=line.split(QRegExp(R"([\[\]])"));
		if(fields.size()<2) continue;
		bool ok=true;
		item.code=fields.at(1).toUInt(&ok, 16);
		item.lines.clear();
		if(!ok) {
			qDebug()<<"incorrect file format: "<<line;
			exit(0);
		}
		//qDebug()<<"font char code: "<<QString::number(item.code, 16);
		item.lines<<line;
		do{
			line=in1.readLine();
			if(in1.atEnd() || line.contains(QRegExp(R"(^\s*$)") )) break;
			item.lines<<line;
		}while(true);
		items.push_back(item);
	}while(!in1.atEnd());

	return items;
}

 void LffSort::sortLff(const QStringList& fileNames)
 {
	 std::vector<fontItem> items;
	 for (QString const& file: fileNames) {
		 auto const& fonts = readFile(file);
		 if (fonts.size()) {
			 qDebug()<<"Appending "<<fonts.size() << " from "<<file;
			 std::copy(fonts.begin(), fonts.end(), std::back_inserter(items));
		 } else {
			qDebug()<<"empty "<<file;
		 }
	 }

	 std::sort(items.begin(), items.end(),
			   [](const fontItem & a, const fontItem & b) -> bool
	 {
		 return a.code < b.code;
	 });

	 writeFile(fileNames.back(), items);
 }

void LffSort::writeFile(QString fileName, std::vector<fontItem> const& fonts)
{
	for(int i = 0; QFileInfo(fileName).exists(); i++)
		fileName.replace(QRegExp(R"((-output[0-9]*)*.lff$)",Qt::CaseInsensitive),
						 QString{"-output%1.lff"}.arg(i));
	QFile outputFile(fileName);
	if (!outputFile.open(QFile::WriteOnly)) {
		qDebug()<<"Can not create file "<<fileName;
		return;
	}
	qDebug()<<"Writing sorted fonts in "<<fileName;
	QTextStream out1(&outputFile);
	out1.setCodec(R"(UTF-8)");

	for(fontItem const& item: fonts){
		out1<<'\n';
		for(QString str: item.lines){
			out1<<str<<'\n';
		}
	}
	out1<<'\n';
	outputFile.close();

}


void LffSort::sortLff(const QString& fileName)
{

    if(QFileInfo(fileName).exists()){
        QFile fontFile(fileName);

        if (!fontFile.open(QFile::ReadOnly)) return;
        QTextStream in1(&fontFile);
        in1.setCodec(R"(UTF-8)");
        QString outFile=fileName;
        outFile.replace(QRegExp(R"(.lff$)",Qt::CaseInsensitive), "-output.lff");
        QFile outputFile(outFile);
        if (!outputFile.open(QFile::WriteOnly)) {
            qDebug()<<"Can not create file "<<outFile;
            return;
        }
        QTextStream out1(&outputFile);
        out1.setCodec(R"(UTF-8)");

        //header
        QString line;
        while( !in1.atEnd()){
            line=in1.readLine();
            if(!line.contains(QRegExp(R"(^\s*#)"))) break;
            out1<<line<<'\n';
        }

        while(!in1.atEnd() && line.contains(QRegExp(R"(^\s*$)")))
            line=in1.readLine();

        std::vector<fontItem> items;

        do{
            while(!in1.atEnd() && !line.contains(QRegExp(R"(^\s*\[.*$)")))
                line=in1.readLine();
            fontItem item;
            QStringList fields=line.split(QRegExp(R"([\[\]])"));
            if(fields.size()<2) continue;
            bool ok=true;
            item.code=fields.at(1).toUInt(&ok, 16);
            if(!ok) {
                qDebug()<<"incorrect file format: "<<line;
                exit(0);
            }
            qDebug()<<"font char code: "<<QString::number(item.code, 16);
            item.lines<<line;
            do{
                line=in1.readLine();
                if(in1.atEnd() || line.contains(QRegExp(R"(^\s*$)") )) break;
                item.lines<<line;
            }while(true);
            items.push_back(item);
        }while(!in1.atEnd());

        std::sort(items.begin(), items.end(),
                  [](const fontItem & a, const fontItem & b) -> bool
        {
            return a.code < b.code;
        });


        for(fontItem& item: items){
            out1<<'\n';
            for(QString str: item.lines){
                out1<<str<<'\n';
            }
        }
        out1<<'\n';
        outputFile.close();
        fontFile.close();
    }
}
