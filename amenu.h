#ifndef AMENU_H
#define AMENU_H

#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QGroupBox>
#include<QLabel>
#include<QPainter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QDialog>
#include<QComboBox>
#include<QFileDialog>
#include<QComboBox>
#include<QMessageBox>
#include"pinho.h"
#include"aamenu.h"
#include<QRadioButton>
class Amenu : public QDialog
{
    Q_OBJECT
public:
   Amenu(QWidget *parent);
public slots:
    void  demarer();
    void browse();
    void browse1();
    void start();
private:
    QLineEdit *file,*file1,*nbr;
    QFormLayout *T1;
    QGroupBox  *B1,*B2;
    QLabel  *A1,*A2,*A3,*label,*label1,*label2,*label3;
    QTextEdit *TT1;
   QGridLayout *G1;
   QPushButton *P1,*button,*button1,*button2;
   QHBoxLayout *layout;
   QFile *ffile;
   QTextStream *f1;
   QString  *f2,style;
   QComboBox *liste;
   QRadioButton *R1,*R2;

};

#endif // AMENU_H
