#include "ScriptFileParser.h"
#include <QFile>

ScriptFileParser::ScriptFileParser(QObject *parent)
    : QObject{parent}
{

}

void ScriptFileParser::parseFile(const QString &fileName)
{
    QFile file(fileName);
    QVector<QPair<QString, QString>> comands;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        int parity = 1;
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();

            if (parity % 2 != 0)
                comands.append({line, ""});
            else
                comands.back().second = line;

            ++parity;
        }
    }
    else {
        emit serviceMsg("Failed to open file with script.");
        return;
    }

    if (!comands.isEmpty())
        emit fileParsed(comands);
    else
        emit serviceMsg("File has no data.");
}
