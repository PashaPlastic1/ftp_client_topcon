#ifndef SCRIPTFILEPARSER_H
#define SCRIPTFILEPARSER_H

#include <QObject>

class ScriptFileParser : public QObject
{
    Q_OBJECT
public:
    explicit ScriptFileParser(QObject *parent = nullptr);

    void parseFile(const QString &fileName);

signals:
    void fileParsed(const QVector<QPair<QString, QString>> &);
    void serviceMsg(const QString &);

};

#endif // SCRIPTFILEPARSER_H
