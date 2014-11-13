#include "lffsort.h"
#include <vector>
#include <algorithm>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>



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
