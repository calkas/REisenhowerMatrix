#ifndef XMLEMATRIXREADER_H
#define XMLEMATRIXREADER_H
#include <QIODevice>
#include <QXmlStreamReader>
#include <QVector>
#include <QPair>

class QListWidget;

class XmlEMatrixReader
{
public:
    XmlEMatrixReader(QIODevice &rIoDevice);
    void Read();
    bool FillList(QListWidget &rList, QString listName);

private:
    QVector<QPair<QString, QString>> *GetParsedXmlTaskList(QString listName);
    void readTaskLists(QVector<QPair<QString, QString>> &rList);
    bool checkRootCorrectness();
    QXmlStreamReader m_XmlReader;
    QVector<QPair<QString, QString>> m_DoList;
    QVector<QPair<QString, QString>> m_ScheduleList;
    QVector<QPair<QString, QString>> m_DelegateList;
    QVector<QPair<QString, QString>> m_EliminateList;
};

#endif // XMLEMATRIXREADER_H
