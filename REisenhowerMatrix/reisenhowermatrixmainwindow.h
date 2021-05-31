#ifndef REISENHOWERMATRIXMAINWINDOW_H
#define REISENHOWERMATRIXMAINWINDOW_H

#include <QMainWindow>
#include <QString>

class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class REisenhowerMatrixMainWindow; }
QT_END_NAMESPACE

class REisenhowerMatrixMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    REisenhowerMatrixMainWindow(QWidget *parent = nullptr);
    ~REisenhowerMatrixMainWindow();

public slots:
    void changeTaskStatus(QListWidgetItem *pListWidgetItem);
    void addTaskItemToListWidget();
    void clearAllTasks();
    void clearSelectedTask();
    void loadFromFile();
    void saveToFile();

private:
    void configurationEmDoGroup();
    void configurationEmScheduleGroup();
    void configurationEmDelegateGroup();
    void configurationEmEliminateGroup();
    void configurationMenuGroup();
    void updateTotalTaskCounter(QLabel *pQLablel, int totalTasks);
    void listWidgetConfiguration(QListWidget *pQListWidget);
    QListWidgetItem * createListWidgetItem(QListWidget *pQListParent, const QString &rText);

    Ui::REisenhowerMatrixMainWindow *m_pUI;
    const QString m_lineEditDefaultName = "Add Task";

};
#endif // REISENHOWERMATRIXMAINWINDOW_H
