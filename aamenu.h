#ifndef AAMENU_H
#define AAMENU_H
#include<QTextEdit>
#include<QLabel>
#include<QDialog>
#include<QGroupBox>
#include<QGridLayout>
#include<QPushButton>
#include<QInputDialog>
#include<QMessageBox>
#include<QFileDialog>
#include <string.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>

class Aamenu : public QDialog
{
     Q_OBJECT
public:
    Aamenu(QString &S1,QString &S2,QString &S3,QString &S4,QWidget *parent);
public slots:
    void save();
private:
    QTextEdit *T1,*T2,*T3;
    QLabel *L1;
    QGroupBox *B1;
    QGridLayout *G1;
    QPushButton *BB1;
};

#endif // AAMENU_H
