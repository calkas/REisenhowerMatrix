#include "reisenhowermatrixmainwindow.h"
#include "ui_reisenhowermatrixmainwindow.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QColor>

REisenhowerMatrixMainWindow::REisenhowerMatrixMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::REisenhowerMatrixMainWindow)
{
    ui->setupUi(this);
    configurationEmDoGroup();
    configurationEmScheduleGroup();
    configurationEmDelegateGroup();
    configurationEmEliminateGroup();
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

void REisenhowerMatrixMainWindow::addTaskIteamToListWidget()
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
        updateTotalTaskCounter(ui->emDoTotalTasksLabel,0);
        updatePendingTaskCounter(ui->emDoPendingTasksLabel,0);
    }
    else if(pButton == ui->emDelegateClearAllButton)
    {
        ui->emDelegateListWidget->clear();
        updateTotalTaskCounter(ui->emDelegateTotalTasksLabel,0);
        updatePendingTaskCounter(ui->emDelegatePendingTasksLabel,0);
    }
    else if(pButton == ui->emScheduleClearAllButton)
    {
        ui->emScheduleListWidget->clear();
        updateTotalTaskCounter(ui->emScheduleTotalTasksLabel,0);
        updatePendingTaskCounter(ui->emSchedulePendingTasksLabel,0);
    }

    else if(pButton == ui->emEliminateClearAllButton)
    {
        ui->emEliminateListWidget->clear();
        updateTotalTaskCounter(ui->emEliminateTotalTasksLabel,0);
        updatePendingTaskCounter(ui->emEliminatePendingTasksLabel,0);
    }

}

void REisenhowerMatrixMainWindow::configurationEmDoGroup()
{
    listWidgetConfiguration(ui->emDoListWidget);
    connect(ui->emDoLineEdit,       SIGNAL(returnPressed()),               this, SLOT(addTaskIteamToListWidget()));
    connect(ui->emDoListWidget,     SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emDoClearAllButton, SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
}

void REisenhowerMatrixMainWindow::configurationEmScheduleGroup()
{
    listWidgetConfiguration(ui->emScheduleListWidget);
    connect(ui->emScheduleLineEdit,       SIGNAL(returnPressed()),               this, SLOT(addTaskIteamToListWidget()));
    connect(ui->emScheduleListWidget,     SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emScheduleClearAllButton, SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
}

void REisenhowerMatrixMainWindow::configurationEmDelegateGroup()
{
    listWidgetConfiguration(ui->emDelegateListWidget);
    connect(ui->emDelegateLineEdit,       SIGNAL(returnPressed()),               this, SLOT(addTaskIteamToListWidget()));
    connect(ui->emDelegateListWidget,     SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emDelegateClearAllButton, SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
}

void REisenhowerMatrixMainWindow::configurationEmEliminateGroup()
{
    listWidgetConfiguration(ui->emEliminateListWidget);
    connect(ui->emEliminateLineEdit,       SIGNAL(returnPressed()),               this, SLOT(addTaskIteamToListWidget()));
    connect(ui->emEliminateListWidget,     SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightAndStrokeItemChecked(QListWidgetItem*)));
    connect(ui->emEliminateClearAllButton, SIGNAL(clicked()),                     this, SLOT(clearAllItems()));
}

void REisenhowerMatrixMainWindow::updateTotalTaskCounter(QLabel *pQLablel, int totalTasks)
{
    QString labelName = "Total Tasks " + QString::number(totalTasks);
    pQLablel->setText(labelName);

}

void REisenhowerMatrixMainWindow::updatePendingTaskCounter(QLabel *pQLablel, int pendingTasks)
{
    QString labelName = "Pending Tasks " + QString::number(pendingTasks);
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

