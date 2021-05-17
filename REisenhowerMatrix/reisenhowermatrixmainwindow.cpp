#include "reisenhowermatrixmainwindow.h"
#include "ui_reisenhowermatrixmainwindow.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QColor>
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QTextStream>
#include "xmlematrixreader.h"

REisenhowerMatrixMainWindow::REisenhowerMatrixMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::REisenhowerMatrixMainWindow)
{
    ui->setupUi(this);
    configurationEmDoGroup();
    configurationEmScheduleGroup();
    configurationEmDelegateGroup();
    configurationEmEliminateGroup();
    configurationMenuGroup();

    ui->statusbar->showMessage("Ready");
}

REisenhowerMatrixMainWindow::~REisenhowerMatrixMainWindow()
{
    delete ui;
}

void REisenhowerMatrixMainWindow::highlightAndStrokeItemChecked(QListWidgetItem *pListWidgetItem)
{
    QFont itemFont = pListWidgetItem->font();
    bool isFontStrikeOut = false;
    QColor itemBackGroundColor = QColor("#ffffff");

    if(pListWidgetItem->checkState() == Qt::Checked)
    {
        isFontStrikeOut = true;
        itemBackGroundColor = QColor("#ffffb2");
    }

    itemFont.setStrikeOut(isFontStrikeOut);
    pListWidgetItem->setFont(itemFont);
    pListWidgetItem->setBackgroundColor(itemBackGroundColor);
}

void REisenhowerMatrixMainWindow::addTaskItemToListWidget()
{
    QLineEdit* pQLineEdit = qobject_cast<QLineEdit*>(sender());
    if(pQLineEdit == nullptr)
    {
        return;
    }

    QString itemName = pQLineEdit->displayText();
    if(itemName.isEmpty())
    {
        return;
    }

    if(pQLineEdit == ui->emDoLineEdit)
    {
        ui->emDoListWidget->addItem(createListWidgetItem(ui->emDoListWidget, itemName));
        updateTotalTaskCounter(ui->emDoTotalTasksLabel, ui->emDoListWidget->model()->rowCount());
    }
    else if(pQLineEdit == ui->emDelegateLineEdit)
    {
        ui->emDelegateListWidget->addItem(createListWidgetItem(ui->emDelegateListWidget, itemName));
        updateTotalTaskCounter(ui->emDelegateTotalTasksLabel, ui->emDelegateListWidget->model()->rowCount());

    }
    else if(pQLineEdit == ui->emScheduleLineEdit)
    {
        ui->emScheduleListWidget->addItem(createListWidgetItem(ui->emScheduleListWidget, itemName));
        updateTotalTaskCounter(ui->emScheduleTotalTasksLabel, ui->emScheduleListWidget->model()->rowCount());
    }
    else if(pQLineEdit == ui->emEliminateLineEdit)
    {
        ui->emEliminateListWidget->addItem(createListWidgetItem(ui->emEliminateListWidget, itemName));
        updateTotalTaskCounter(ui->emEliminateTotalTasksLabel, ui->emEliminateListWidget->model()->rowCount());
    }
    pQLineEdit->clear();
    pQLineEdit->setPlaceholderText(m_lineEditDefaultName);
}

void REisenhowerMatrixMainWindow::clearAllItems()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    if(pButton == ui->emDoClearAllButton)
    {
        ui->emDoListWidget->clear();
        updateTotalTaskCounter(ui->emDoTotalTasksLabel, 0);
    }
    else if(pButton == ui->emDelegateClearAllButton)
    {
        ui->emDelegateListWidget->clear();
        updateTotalTaskCounter(ui->emDelegateTotalTasksLabel, 0);
    }
    else if(pButton == ui->emScheduleClearAllButton)
    {
        ui->emScheduleListWidget->clear();
        updateTotalTaskCounter(ui->emScheduleTotalTasksLabel, 0);
    }
    else if(pButton == ui->emEliminateClearAllButton)
    {
        ui->emEliminateListWidget->clear();
        updateTotalTaskCounter(ui->emEliminateTotalTasksLabel, 0);
    }
    else
    {
        //Nothing
    }
}

void REisenhowerMatrixMainWindow::clearSelectedItem()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());

    if(pButton == ui->emDoClearSelectedButton)
    {
        ui->emDoListWidget->model()->removeRow(ui->emDoListWidget->currentIndex().row());
        updateTotalTaskCounter(ui->emDoTotalTasksLabel, ui->emDoListWidget->model()->rowCount());
    }
    else if(pButton == ui->emDelegateClearSelectedButton)
    {
        ui->emDelegateListWidget->model()->removeRow(ui->emDelegateListWidget->currentIndex().row());
        updateTotalTaskCounter(ui->emDelegateTotalTasksLabel, ui->emDelegateListWidget->model()->rowCount());
    }
    else if(pButton == ui->emScheduleClearSelectedButton)
    {
        ui->emScheduleListWidget->model()->removeRow(ui->emScheduleListWidget->currentIndex().row());
        updateTotalTaskCounter(ui->emScheduleTotalTasksLabel, ui->emScheduleListWidget->model()->rowCount());
    }

    else if(pButton == ui->emEliminateClearSelectedButton)
    {
        ui->emEliminateListWidget->model()->removeRow(ui->emEliminateListWidget->currentIndex().row());
        updateTotalTaskCounter(ui->emEliminateTotalTasksLabel, ui->emEliminateListWidget->model()->rowCount());
    }
    else
    {
        //Nothing
    }

}

void REisenhowerMatrixMainWindow::loadFromFile()
{
    m_xmlFilePath = QFileDialog::getOpenFileName(this, tr("Open xml File"), "c:/", tr("xml file (*.xml)"));
    if(m_xmlFilePath.isEmpty())
    {
        ui->statusbar->showMessage("The file path is empty");
        return;
    }
    QFile xmlFile(m_xmlFilePath);
    if(!xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, tr("Unable to open file"),
            xmlFile.errorString());
        ui->statusbar->showMessage("Cannot read file");
        return;
    }

    ui->statusbar->showMessage("Open File: " + m_xmlFilePath);

    XmlEMatrixReader reader(xmlFile);
    reader.Read();
    bool isSuccess = reader.FillList(*ui->emDoListWidget, "DoListMatrix");
    isSuccess &= reader.FillList(*ui->emScheduleListWidget, "ScheduleListMatrix");
    isSuccess &= reader.FillList(*ui->emDelegateListWidget, "DelegateListMatrix");
    isSuccess &= reader.FillList(*ui->emEliminateListWidget, "EliminateListMatrix");

    if(isSuccess)
    {
        ui->statusbar->showMessage("Done");
    }
    else
    {
        ui->statusbar->showMessage("Error");
    }
    updateTotalTaskCounter(ui->emDoTotalTasksLabel, ui->emDoListWidget->model()->rowCount());
    updateTotalTaskCounter(ui->emDelegateTotalTasksLabel, ui->emDelegateListWidget->model()->rowCount());
    updateTotalTaskCounter(ui->emScheduleTotalTasksLabel, ui->emScheduleListWidget->model()->rowCount());
    updateTotalTaskCounter(ui->emEliminateTotalTasksLabel, ui->emEliminateListWidget->model()->rowCount());
    xmlFile.close();
}

void REisenhowerMatrixMainWindow::saveToFile()
{
    QString savefileNamePath = QFileDialog::getSaveFileName(this, tr("Save Eisenhower Matrix to xml file"),"c:/", tr("xml file (*.xml)"));

    if(savefileNamePath.isEmpty())
    {
        ui->statusbar->showMessage("Something wrong with path to save");
        return;
    }

    QFile file(savefileNamePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        ui->statusbar->showMessage("Cannot read file");
        return;
    }

    QTextStream out(&file);
    out << createXmlStr();

    ui->statusbar->showMessage("Saved!");

    file.close();
}

void REisenhowerMatrixMainWindow::configurationEmDoGroup()
{
    listWidgetConfiguration(ui->emDoListWidget);
    connect(ui->emDoLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(ui->emDoListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emDoClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
    connect(ui->emDoClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedItem()));
}

void REisenhowerMatrixMainWindow::configurationEmScheduleGroup()
{
    listWidgetConfiguration(ui->emScheduleListWidget);
    connect(ui->emScheduleLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(ui->emScheduleListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emScheduleClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
    connect(ui->emScheduleClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedItem()));
}

void REisenhowerMatrixMainWindow::configurationEmDelegateGroup()
{
    listWidgetConfiguration(ui->emDelegateListWidget);
    connect(ui->emDelegateLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(ui->emDelegateListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emDelegateClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
    connect(ui->emDelegateClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedItem()));
}

void REisenhowerMatrixMainWindow::configurationEmEliminateGroup()
{
    listWidgetConfiguration(ui->emEliminateListWidget);
    connect(ui->emEliminateLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(ui->emEliminateListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emEliminateClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
    connect(ui->emEliminateClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedItem()));
}

void REisenhowerMatrixMainWindow::configurationMenuGroup()
{
    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
}

QString REisenhowerMatrixMainWindow::createXmlStr()
{
    QString xmlStr;
    addXmlHeader(xmlStr);

    addTaskListToXml("DoListMatrix",        *ui->emDoListWidget, xmlStr);
    addTaskListToXml("ScheduleListMatrix",  *ui->emScheduleListWidget, xmlStr);
    addTaskListToXml("DelegateListMatrix",  *ui->emDelegateListWidget, xmlStr);
    addTaskListToXml("EliminateListMatrix", *ui->emEliminateListWidget, xmlStr);
    addXmlFootage(xmlStr);

    qDebug() << qPrintable(xmlStr);
    return xmlStr;
}

void REisenhowerMatrixMainWindow::addXmlHeader(QString& rXml)
{
    rXml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    rXml += "<!--R_EisenhowerMatrix Xml File-->\n";
    rXml += "<EisenhowerMatrix>\n";
}

void REisenhowerMatrixMainWindow::addXmlFootage(QString &rXml)
{
    rXml += "</EisenhowerMatrix>";
}

void REisenhowerMatrixMainWindow::addTaskListToXml(QString listName, QListWidget &rQListWidget, QString &rXml)
{
    rXml += "    <" + listName + ">\n";

    for(int i = 0; i<rQListWidget.count(); ++i)
    {
        QListWidgetItem *pItem = rQListWidget.item(i);
        rXml += "        <task>" + pItem->text() + "</task>\n";

        if(pItem->checkState() == Qt::Checked)
        {
            rXml += "        <status>Done</status>\n";
        }
        else
        {
            rXml += "        <status>Pending</status>\n";
        }
    }
    rXml += "    </" + listName+ ">\n";
}

void REisenhowerMatrixMainWindow::updateTotalTaskCounter(QLabel *pQLablel, int totalTasks)
{
    QString labelName = "Total Tasks " + QString::number(totalTasks);
    pQLablel->setText(labelName);
}

void REisenhowerMatrixMainWindow::listWidgetConfiguration(QListWidget *pQListWidget)
{
    pQListWidget->setDragEnabled(true);
    pQListWidget->setAcceptDrops(true);
    pQListWidget->setDropIndicatorShown(true);
    pQListWidget->setDefaultDropAction(Qt::MoveAction);
}

QListWidgetItem * REisenhowerMatrixMainWindow::createListWidgetItem(QListWidget *pQListParent, const QString &rText)
{
    if(pQListParent == nullptr)
    {
        return nullptr;
    }
    QListWidgetItem* pQListWidgetItem = new QListWidgetItem(pQListParent);
    pQListWidgetItem->setFlags(pQListWidgetItem->flags() | Qt::ItemIsUserCheckable);
    pQListWidgetItem->setCheckState(Qt::Unchecked);
    pQListWidgetItem->setText(rText);
    return pQListWidgetItem;
}

