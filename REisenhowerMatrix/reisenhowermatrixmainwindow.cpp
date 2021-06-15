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
#include "tasktoxmlbuilder.h"

REisenhowerMatrixMainWindow::REisenhowerMatrixMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pUI(new Ui::REisenhowerMatrixMainWindow)
    , m_wasModificationMade(false)
{
    m_pUI->setupUi(this);
    configurationEmDoGroup();
    configurationEmScheduleGroup();
    configurationEmDelegateGroup();
    configurationEmEliminateGroup();
    configurationMenuGroup();

    m_pUI->statusbar->showMessage("Ready");
}

REisenhowerMatrixMainWindow::~REisenhowerMatrixMainWindow()
{
    delete m_pUI;
}

void REisenhowerMatrixMainWindow::changeTaskStatus(QListWidgetItem *pListWidgetItem)
{
    QFont itemFont = pListWidgetItem->font();
    bool isFontStrikeOut = false;
    QColor itemBackGroundColor = QColor("#ffffff");

    if(pListWidgetItem->checkState() == Qt::Checked)
    {
        isFontStrikeOut = true;
        itemBackGroundColor = QColor("#5bdbf5");
        //itemBackGroundColor = QColor("#ffffb2");
    }

    itemFont.setStrikeOut(isFontStrikeOut);
    pListWidgetItem->setFont(itemFont);
    pListWidgetItem->setBackgroundColor(itemBackGroundColor);
    m_wasModificationMade = true;
}

void REisenhowerMatrixMainWindow::addTaskItemToListWidget()
{
    m_pUI->statusbar->showMessage("Ready");
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

    if(pQLineEdit == m_pUI->emDoLineEdit)
    {
        m_pUI->emDoListWidget->addItem(createListWidgetItem(m_pUI->emDoListWidget, itemName));
        updateTotalTaskCounter(m_pUI->emDoTotalTasksLabel, m_pUI->emDoListWidget->model()->rowCount());
    }
    else if(pQLineEdit == m_pUI->emDelegateLineEdit)
    {
        m_pUI->emDelegateListWidget->addItem(createListWidgetItem(m_pUI->emDelegateListWidget, itemName));
        updateTotalTaskCounter(m_pUI->emDelegateTotalTasksLabel, m_pUI->emDelegateListWidget->model()->rowCount());
    }
    else if(pQLineEdit == m_pUI->emScheduleLineEdit)
    {
        m_pUI->emScheduleListWidget->addItem(createListWidgetItem(m_pUI->emScheduleListWidget, itemName));
        updateTotalTaskCounter(m_pUI->emScheduleTotalTasksLabel, m_pUI->emScheduleListWidget->model()->rowCount());
    }
    else if(pQLineEdit == m_pUI->emEliminateLineEdit)
    {
        m_pUI->emEliminateListWidget->addItem(createListWidgetItem(m_pUI->emEliminateListWidget, itemName));
        updateTotalTaskCounter(m_pUI->emEliminateTotalTasksLabel, m_pUI->emEliminateListWidget->model()->rowCount());
    }
    else
    {
        //Nothing
    }

    pQLineEdit->clear();
    pQLineEdit->setPlaceholderText(m_lineEditDefaultName);
    m_wasModificationMade = true;
}

void REisenhowerMatrixMainWindow::clearAllTasks()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    if(pButton == m_pUI->emDoClearAllButton)
    {
        m_pUI->emDoListWidget->clear();
        updateTotalTaskCounter(m_pUI->emDoTotalTasksLabel, 0);
    }
    else if(pButton == m_pUI->emDelegateClearAllButton)
    {
        m_pUI->emDelegateListWidget->clear();
        updateTotalTaskCounter(m_pUI->emDelegateTotalTasksLabel, 0);
    }
    else if(pButton == m_pUI->emScheduleClearAllButton)
    {
        m_pUI->emScheduleListWidget->clear();
        updateTotalTaskCounter(m_pUI->emScheduleTotalTasksLabel, 0);
    }
    else if(pButton == m_pUI->emEliminateClearAllButton)
    {
        m_pUI->emEliminateListWidget->clear();
        updateTotalTaskCounter(m_pUI->emEliminateTotalTasksLabel, 0);
    }
    else
    {
        //Nothing
    }

    m_wasModificationMade = true;
}

void REisenhowerMatrixMainWindow::clearSelectedTask()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());

    if(pButton == m_pUI->emDoClearSelectedButton)
    {
        m_pUI->emDoListWidget->model()->removeRow(m_pUI->emDoListWidget->currentIndex().row());
        updateTotalTaskCounter(m_pUI->emDoTotalTasksLabel, m_pUI->emDoListWidget->model()->rowCount());
    }
    else if(pButton == m_pUI->emDelegateClearSelectedButton)
    {
        m_pUI->emDelegateListWidget->model()->removeRow(m_pUI->emDelegateListWidget->currentIndex().row());
        updateTotalTaskCounter(m_pUI->emDelegateTotalTasksLabel, m_pUI->emDelegateListWidget->model()->rowCount());
    }
    else if(pButton == m_pUI->emScheduleClearSelectedButton)
    {
        m_pUI->emScheduleListWidget->model()->removeRow(m_pUI->emScheduleListWidget->currentIndex().row());
        updateTotalTaskCounter(m_pUI->emScheduleTotalTasksLabel, m_pUI->emScheduleListWidget->model()->rowCount());
    }

    else if(pButton == m_pUI->emEliminateClearSelectedButton)
    {
        m_pUI->emEliminateListWidget->model()->removeRow(m_pUI->emEliminateListWidget->currentIndex().row());
        updateTotalTaskCounter(m_pUI->emEliminateTotalTasksLabel, m_pUI->emEliminateListWidget->model()->rowCount());
    }
    else
    {
        //Nothing
    }

    m_wasModificationMade = true;
}

void REisenhowerMatrixMainWindow::loadFromFile()
{
    if(m_wasModificationMade)
    {
        QMessageBox::StandardButton reply;
          reply = QMessageBox::warning(this, "Warning", "\nDo you want to save the document before load?\n\n",
                                        QMessageBox::Save |QMessageBox::Discard);
          if (reply == QMessageBox::Save)
          {
              saveToFile();
          }
    }

    QString xmlFilePath = QFileDialog::getOpenFileName(this,
                                                       tr("Open xml File"),
                                                       "c:/",
                                                       tr("xml file (*.xml)"));
    if(xmlFilePath.isEmpty())
    {
        m_pUI->statusbar->showMessage("The file path is empty");
        return;
    }
    QFile xmlFile(xmlFilePath);
    if(!xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this,
                                 tr("Unable to open file"),
                                 xmlFile.errorString());
        m_pUI->statusbar->showMessage("Cannot read the file");
        return;
    }

    XmlEMatrixReader reader(xmlFile);
    reader.Read();
    bool isSuccess = reader.FillList(*m_pUI->emDoListWidget, "DoListMatrix");
    isSuccess &= reader.FillList(*m_pUI->emScheduleListWidget, "ScheduleListMatrix");
    isSuccess &= reader.FillList(*m_pUI->emDelegateListWidget, "DelegateListMatrix");
    isSuccess &= reader.FillList(*m_pUI->emEliminateListWidget, "EliminateListMatrix");

    m_pUI->statusbar->showMessage("File: " + xmlFilePath + " - Loaded");

    if(!isSuccess)
    {
        m_pUI->statusbar->showMessage("Error - Cannot parse the file");
    }

    updateTotalTaskCounter(m_pUI->emDoTotalTasksLabel,        m_pUI->emDoListWidget->model()->rowCount());
    updateTotalTaskCounter(m_pUI->emDelegateTotalTasksLabel,  m_pUI->emDelegateListWidget->model()->rowCount());
    updateTotalTaskCounter(m_pUI->emScheduleTotalTasksLabel,  m_pUI->emScheduleListWidget->model()->rowCount());
    updateTotalTaskCounter(m_pUI->emEliminateTotalTasksLabel, m_pUI->emEliminateListWidget->model()->rowCount());
    xmlFile.close();

    m_wasModificationMade = false;
}

void REisenhowerMatrixMainWindow::saveToFile()
{
    QString savefileNamePath = QFileDialog::getSaveFileName(this,
                                                            tr("Save Eisenhower Matrix to xml file"),
                                                            "c:/",
                                                            tr("xml file (*.xml)"));

    if(savefileNamePath.isEmpty())
    {
        m_pUI->statusbar->showMessage("Something wrong with path to save");
        return;
    }

    QFile file(savefileNamePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        m_pUI->statusbar->showMessage("Cannot read file");
        return;
    }

    QTextStream out(&file);
    TaskToXmlBuilder xmlBuilder;

    out << xmlBuilder.addXmlHeader()
                     .addTaskListToXml("DoListMatrix",        *m_pUI->emDoListWidget)
                     .addTaskListToXml("ScheduleListMatrix",  *m_pUI->emScheduleListWidget)
                     .addTaskListToXml("DelegateListMatrix",  *m_pUI->emDelegateListWidget)
                     .addTaskListToXml("EliminateListMatrix", *m_pUI->emEliminateListWidget)
                     .addXmlFootage()
                     .buildXml();

    m_pUI->statusbar->showMessage("File: " + savefileNamePath + " - Saved");

    file.close();
    m_wasModificationMade = false;
}

void REisenhowerMatrixMainWindow::configurationEmDoGroup()
{
    listWidgetConfiguration(m_pUI->emDoListWidget);
    connect(m_pUI->emDoLineEdit,                  SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(m_pUI->emDoListWidget,                SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(changeTaskStatus(QListWidgetItem*)));
    connect(m_pUI->emDoClearAllButton,            SIGNAL(clicked()),                     this, SLOT(clearAllTasks()));
    connect(m_pUI->emDoClearSelectedButton,       SIGNAL(clicked()),                     this, SLOT(clearSelectedTask()));
}

void REisenhowerMatrixMainWindow::configurationEmScheduleGroup()
{
    listWidgetConfiguration(m_pUI->emScheduleListWidget);
    connect(m_pUI->emScheduleLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(m_pUI->emScheduleListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(changeTaskStatus(QListWidgetItem*)));
    connect(m_pUI->emScheduleClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllTasks()));
    connect(m_pUI->emScheduleClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedTask()));
}

void REisenhowerMatrixMainWindow::configurationEmDelegateGroup()
{
    listWidgetConfiguration(m_pUI->emDelegateListWidget);
    connect(m_pUI->emDelegateLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(m_pUI->emDelegateListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(changeTaskStatus(QListWidgetItem*)));
    connect(m_pUI->emDelegateClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllTasks()));
    connect(m_pUI->emDelegateClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedTask()));
}

void REisenhowerMatrixMainWindow::configurationEmEliminateGroup()
{
    listWidgetConfiguration(m_pUI->emEliminateListWidget);
    connect(m_pUI->emEliminateLineEdit,            SIGNAL(returnPressed()),               this, SLOT(addTaskItemToListWidget()));
    connect(m_pUI->emEliminateListWidget,          SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(changeTaskStatus(QListWidgetItem*)));
    connect(m_pUI->emEliminateClearAllButton,      SIGNAL(clicked()),                     this, SLOT(clearAllTasks()));
    connect(m_pUI->emEliminateClearSelectedButton, SIGNAL(clicked()),                     this, SLOT(clearSelectedTask()));
}

void REisenhowerMatrixMainWindow::configurationMenuGroup()
{
    connect(m_pUI->loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(m_pUI->saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
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

