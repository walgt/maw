#ifndef MENU_H
#define MENU_H

#include<QObject>
#include<QWidget>
#include<QLabel>
#include<QPushButton>
#include<QString>
#include<QTextEdit>
#include <QFile>
#include <QTextStream>
#include<QMessageBox>

class Menu: public QWidget
{
     Q_OBJECT
public:
    Menu();
public slots:
    void ouvrirhelp();
    void commencetraitement();
private:
    QLabel *I1;
    QPushButton *P1,*P2;
    QString style;


};

#endif // MENU_H
