#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QString>
#include <QVBoxLayout>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    counter = 0;

    ui->setupUi(this);

    //set name for the main window
    QWidget::setWindowTitle("Death Counter");

    // set fixed size for the window
    this->setFixedSize(750,500);


    // button to add deaths
    ui->pushButton->setText("Press to Add a Death");
    ui->pushButton->setFixedHeight(200);
    ui->pushButton->setFixedWidth(200);
    QRect button(5, 5, 185, 185);
    QRegion region(button, QRegion::Ellipse);
    ui->pushButton->setMask(region);

    // set up VBoxlayout for the scroll area
    QWidget *scrollArea = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scrollArea);
    scrollArea->setLayout(layout);
    ui->scrollArea->setWidget(scrollArea);




    /*      Connectors       */

    // button for counter
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::onButtonPress);

    // background
    setStyleSheet("MainWindow {background-image: url(:/new/prefix1/eldenbackground.jpeg);}");

    //boss text
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::bossText);

    //saved boss info
    connect(ui->pushButton_2,&QPushButton::released, this, &MainWindow::addDeaths);

    //reset button connected to add to list button
    connect(ui->pushButton_2,&QPushButton::released, this, &MainWindow::resetCounter);

    loadData();

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::loadData(){
    QSettings settings("Project", "DeathCounterApp");
    int size = settings.beginReadArray("bosses");
    for(int i = 0; i < size; i++){
        settings.setArrayIndex(i);
        QString bossName = settings.value("bossName").toString();
        int deathCount = settings.value("deathCount").toInt();
        qDebug() << "Loaded boss:" << bossName << "with death count:" << deathCount;
        addDeathsPerBossToList(bossName, deathCount);
    }
    settings.endArray();

    //load the counter at the top
    counter = settings.value("counter", 0).toInt();
    ui->label->setText(QString("\tDeath Count: %1").arg(counter));
}

void MainWindow::saveData(){
    QSettings settings("Project", "DeathCounterApp");
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollArea->widget()->layout());
    settings.beginWriteArray("bosses");
    for(int i = 0; i < layout->count(); i++){
        settings.setArrayIndex(i);
        QPushButton *button = qobject_cast<QPushButton*>(layout->itemAt(i)->widget());
        if(button){
            QString text = button->text();
            QStringList parts = text.split(": ");
            if(parts.size() == 2){
                QString bossName = parts[0].mid(19).trimmed();
                int deathCount = parts[1].toInt();
                qDebug() << "Saving boss:" << bossName << "with death count:" << deathCount;
                settings.setValue("bossName", bossName);
                settings.setValue("deathCount", deathCount);
            }
        }
    }
    settings.endArray();

    settings.setValue("Counter", counter);
}


void MainWindow::closeEvent(QCloseEvent *event){
    saveData();
    QMainWindow::closeEvent(event);
}

void MainWindow::resetCounter(){
    counter = 0;
    ui->label->setText(QString("\tDeath Count: %1").arg(counter));
}

void MainWindow::onButtonPress(){
    counter++;
    ui->label->setText(QString("\tDeath Count: %1").arg(counter));
}

void MainWindow::bossText(){
    // user inputs name of boss they are fighting and is displayed in the ui->label line
    bossName = ui->lineEdit->text();

    ui->label_2->setText(QString("\tFighting %1").arg(bossName));
}

void MainWindow::addDeathsPerBossToList(const QString &bossName, int deathCount){
    // Retrieve the QVBoxLayout from the widget inside the scroll area
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollArea->widget()->layout());

    // // debugginf print statement
    // if (!layout) {
    //     qWarning("Scroll area does not have a QVBoxLayout.");
    //     return; // Exit early if layout is not valid
    // }

    QString buttonTxt = tr("Number of Deaths to %1: %2").arg(bossName).arg(deathCount);
    QPushButton* newButton = new QPushButton(buttonTxt, ui->scrollArea);

    newButton->setStyleSheet("QPushButton {border: none; background: none;}");

    layout->insertWidget(0, newButton);

    QObject::connect(
        newButton, &QPushButton::released,
        this, &MainWindow::removeFromList
        );
}

void MainWindow::addDeaths(){
    // Retrieve the QVBoxLayout from the widget inside the scroll area
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollArea->widget()->layout());

    // // debugginf print statement
    // if (!layout) {
    //     qWarning("Scroll area does not have a QVBoxLayout.");
    //     return; // Exit early if layout is not valid
    // }

    QString buttonTxt = tr("Number of Deaths to %1: %2").arg(bossName).arg(counter);
    QPushButton* newButton = new QPushButton(buttonTxt, ui->scrollArea);

    newButton->setStyleSheet("QPushButton {border: none; background: none;}");

    layout->insertWidget(0, newButton);

    QObject::connect(
        newButton, &QPushButton::released,
        this, &MainWindow::removeFromList
        );
}


void MainWindow::removeFromList(){
    QPushButton *newButton = qobject_cast<QPushButton*>(sender());
    delete newButton;
}








