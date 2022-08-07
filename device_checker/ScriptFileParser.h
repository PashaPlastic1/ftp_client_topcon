#ifndef SCRIPTFILEPARSER_H
#define SCRIPTFILEPARSER_H

#include <QObject>
/*!
    \brief Класс для парсинга файла со сценарием тестирования устройства

    Файл имеет форма:
    [КОМАНДА]\n
    [ОЖИДАЕМЫЙ ОТВЕТ]\n
    [КОМАНДА]\n
    [ОЖИДАЕМЫЙ ОТВЕТ]\n
    ...
    [КОМАНДА]\n
    [ОЖИДАЕМЫЙ ОТВЕТ]
*/
class ScriptFileParser : public QObject
{
    Q_OBJECT
public:
    explicit ScriptFileParser(QObject *parent = nullptr);

    //! Парсит файла со сценарием тестирования устройства
    //! и эмитет fileParsed() с результатом
    //! \param[in] fileName - полное имя файла
    void parseFile(const QString &fileName);

signals:
    //! Результат парсинга
    //! \param[in] first - команда
    //! \param[in] second - ожидаемый ответ
    void fileParsed(const QVector<QPair<QString, QString>> &);
    //! Сервисное сообщение
    //! \param[in] - сервисное сообщение
    void serviceMsg(const QString &);
};

#endif // SCRIPTFILEPARSER_H
