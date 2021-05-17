#include "xmlematrixreader.h"
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>

XmlEMatrixReader::XmlEMatrixReader(QIODevice &rIoDevice):
    m_XmlReader(&rIoDevice)
{
    if(!checkRootCorrectness())
    {
        m_XmlReader.raiseError(QObject::tr("Not a EisenhowerMatrix xml file"));
    }
}

void XmlEMatrixReader::Read()
{
    while (m_XmlReader.readNextStartElement() && !m_XmlReader.atEnd())
    {
        if(m_XmlReader.name() == "DoListMatrix")
        {
            //qDebug() << "Parsing DoListMatrix";
            readTaskLists(m_DoList);
        }
        else if(m_XmlReader.name() == "ScheduleListMatrix")
        {
            //qDebug() << "Parsing ScheduleListMatrix";
            readTaskLists(m_ScheduleList);
        }

        else if(m_XmlReader.name() == "DelegateListMatrix")
        {
            //qDebug() << "Parsing DelegateListMatrix";
            readTaskLists(m_DelegateList);
        }

        else if(m_XmlReader.name() == "EliminateListMatrix")
        {
            //qDebug() << "Parsing EliminateListMatrix";
            readTaskLists(m_EliminateList);
        }

    }

    return;
}

bool XmlEMatrixReader::FillList(QListWidget &rList, QString listName)
{
    QVector<QPair<QString, QString>> *pParsedXmlTaskList = GetParsedXmlTaskList(listName);
    if(pParsedXmlTaskList == nullptr)
    {
        return false;
    }
    rList.clear();

    for(QPair<QString, QString> &element : *pParsedXmlTaskList)
    {
        QListWidgetItem* pQListWidgetItem = new QListWidgetItem(&rList);
        if(pQListWidgetItem == nullptr)
        {
            return false;
        }

        pQListWidgetItem->setFlags(pQListWidgetItem->flags() | Qt::ItemIsUserCheckable);
        if(element.second == "Done")
        {
            pQListWidgetItem->setCheckState(Qt::Checked);
        }
        else
        {
            pQListWidgetItem->setCheckState(Qt::Unchecked);
        }

        pQListWidgetItem->setText(element.first);
        rList.addItem(pQListWidgetItem);
    }

    return true;
}

QVector<QPair<QString, QString>> *XmlEMatrixReader::GetParsedXmlTaskList(QString listName)
{
    if(listName == "DoListMatrix")
        return &m_DoList;
    else if(listName == "ScheduleListMatrix")
        return &m_ScheduleList;
    else if(listName == "DelegateListMatrix")
        return &m_DelegateList;
    else if(listName == "EliminateListMatrix")
        return &m_EliminateList;
    else
        return nullptr;
}

void XmlEMatrixReader::readTaskLists(QVector<QPair<QString, QString>> &rList)
{
    while (m_XmlReader.readNextStartElement())
    {
        if(m_XmlReader.name() == "task")
        {
            QString taskName = m_XmlReader.readElementText();
            if(m_XmlReader.readNextStartElement())
            {
                if(m_XmlReader.name() == "status")
                {
                    QString taskStatus = m_XmlReader.readElementText();
                    QPair<QString, QString> pairObj(taskName, taskStatus);
                    rList.push_back(pairObj);

                    //qDebug() << taskName;
                    //qDebug() << taskStatus;
                }
                else
                {
                    m_XmlReader.skipCurrentElement();
                }

            }
            else
            {
                break;
            }
        }
        else
        {
            m_XmlReader.skipCurrentElement();
        }
    }
}

bool XmlEMatrixReader::checkRootCorrectness()
{
    if(m_XmlReader.readNextStartElement())
    {
        if(m_XmlReader.name() == "EisenhowerMatrix")
        {
            return true;
        }
    }
    return false;
}
