#include "tasktoxmlbuilder.h"
#include <QListWidget>


TaskToXmlBuilder::TaskToXmlBuilder() : m_Xml("")
{
}

TaskToXmlBuilder &TaskToXmlBuilder::addXmlHeader()
{
    m_Xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    m_Xml += "<!--R_EisenhowerMatrix Xml File-->\n";
    m_Xml += "<EisenhowerMatrix>\n";
    return *this;
}

TaskToXmlBuilder &TaskToXmlBuilder::addXmlFootage()
{
    m_Xml += "</EisenhowerMatrix>";
    return *this;
}

TaskToXmlBuilder &TaskToXmlBuilder::addTaskListToXml(QString listName, QListWidget &rQListWidget)
{
    m_Xml += "    <" + listName + ">\n";

    for(int i = 0; i<rQListWidget.count(); ++i)
    {
        QListWidgetItem *pItem = rQListWidget.item(i);
        m_Xml += "        <task>" + pItem->text() + "</task>\n";

        if(pItem->checkState() == Qt::Checked)
        {
            m_Xml += "        <status>Done</status>\n";
        }
        else
        {
            m_Xml += "        <status>Pending</status>\n";
        }
    }
    m_Xml += "    </" + listName+ ">\n";
    return *this;
}

QString TaskToXmlBuilder::buildXml() const
{
    return m_Xml;
}
