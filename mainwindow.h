#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QSettings>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setAction(QAction *action);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onButtonPress();
    void bossText();
    //need two one for the call in the function the other that connects to the button
    void addDeathsPerBossToList(const QString &bossName, int deathCount);
    void addDeaths();
    void resetCounter();
    void removeFromList();

private:
    Ui::MainWindow *ui;
    QPushButton *m_button;
    int counter; // adds counter variable
    QString bossName;
    QSettings settings;
    void loadData();
    void saveData();
};
#endif // MAINWINDOW_H
