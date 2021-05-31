#ifndef TASKTOXMLBUILDER_H
#define TASKTOXMLBUILDER_H
#include <QString>

class QListWidget;

class TaskToXmlBuilder
{
public:
    TaskToXmlBuilder();
    TaskToXmlBuilder& addXmlHeader();
    TaskToXmlBuilder& addXmlFootage();
    TaskToXmlBuilder& addTaskListToXml(QString listName, QListWidget &rQListWidget);
    QString buildXml() const;

private:
    QString m_Xml;
};

#endif // TASKTOXMLBUILDER_H
