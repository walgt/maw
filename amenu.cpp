#include "amenu.h"
#include <iostream>
#include <vector>
#include<string>
#include<string.h>
#include<QByteArray>
#include<QString>
Amenu::Amenu(QWidget *parent) : QDialog(parent)
{
QWidget::setWindowTitle ( "MAW GENRATOR" );
   setFixedSize(672, 480);
   A3=new QLabel(this);
   A3->setPixmap(QPixmap("execut.png"));

   file= new QLineEdit("",this);
   file1= new QLineEdit("",this);
   style="QPushButton {background-color: #C3C3C3;border-style: outset;border-width: 2px;border-radius: 10px;border-color: beige;font: bold 14px;min-width: 8em;padding: 6px;}QPushButton:pressed {background-color: #B9ACAB;border-style: inset;}";

   button = new QPushButton("Select",this);
   button->setStyleSheet(style);
   label=new QLabel("      Human DNA ",this);
   label->setMinimumHeight(30);
   button->move(500,100);
   label->move(20,100);
   file->move(125,100);
   file->setMinimumWidth(340);
   file->setMinimumHeight(30);
   QObject::connect(button, SIGNAL(clicked()), SLOT(browse()));

   button1 = new QPushButton("Select",this);
   button1->setStyleSheet(style);
   button1->move(500,150);
   label1=new QLabel("Virus DNA",this);
   label1->setMinimumHeight(30);
   label1->move(50,150);

   file1->move(130,150);
   file1->setMinimumWidth(340);
   file1->setMinimumHeight(30);

   button2 = new QPushButton("Launch",this);
   button2->setStyleSheet(style);
   button2->move(250,340);

   label3=new QLabel("Length",this);
   nbr=new QLineEdit("",this);
   label3->move(50,205);
   nbr->move(150,200);
   QObject::connect(button1, SIGNAL(clicked()), SLOT(browse1()));
   QObject::connect(button2, SIGNAL(clicked()), SLOT(start()));


}
void  Amenu::demarer(){

}

void Amenu::browse(){
    QString S=QFileDialog::getOpenFileName(this, "Open a file", QString());
    file->setText(S);
}

void Amenu::browse1(){
    QString S=QFileDialog::getOpenFileName(this, "Open a file", QString());
    file1->setText(S);
}

void Amenu::start(){

    vector<string> a,d;

    QByteArray byteArray = file->text().toLatin1();
    const char *characters = byteArray.constData();
    char* foo = const_cast<char*>(characters);
    
    QByteArray byteArray1 = file1->text().toLatin1();
    const char *characters1 = byteArray1.constData();
    char* foo1 = const_cast<char*>(characters1);

    if (fopen(foo,"r")!=NULL){


       if(fopen(foo1,"r")!=NULL){

        string c,b;




         a=pinho(nbr->text().toInt(),foo);
         c ="Pinho:\nNumber of minimal absent words found : "+a[2]+"\n-----------------\n";

                 d=maw(foo,nbr->text().toInt());
                 c =c+"MAW:\nNumber of minimal absent words found : "+std::to_string((int)d.size()-1)+"\n-----------------\n";
                 float ss=std::stof(a[3]) -std::stof(d[d.size()-1]);

                 c=c+"Human DNA size : "+std::to_string(std::stof(a[1]))+" ko\nNumber of characters : "+std::to_string(int(std::stof(a[1])/0.001))+"\n";


        QFile *ffile=new QFile("result.txt");
        QString  *f2=new QString();
        QString  *f3=new QString();
        QString  *f4=new QString();

        if(ffile->open(QIODevice::ReadOnly | QIODevice::Text))
         {
              (*f2) = ffile->readAll();
              ffile->close();
         }
         else (*f2)="Impossible to open the file !";
        ffile->close();

        a=ac(a,foo1);
        c=c+"DNA virus size: "+a[1]+" ko\nNumber of characters : "+std::to_string(int(std::stof(a[1])*1000))+"\n-----------------\n";
        c=c+"number of minimal absent words that are absent\n from humans DNA but present in virus DNA : "+std::to_string((a.size()-2))+" \n";
         QString h=QString::fromStdString(c);
        ffile=new QFile("result2.txt");

        if(ffile->open(QIODevice::ReadOnly | QIODevice::Text))
         {
              (*f4) = ffile->readAll();
              ffile->close();
         }
         else (*f4)="Impossible to open the file !";

        ffile->close();



        ffile=new QFile("result3.txt");

        if(ffile->open(QIODevice::ReadOnly | QIODevice::Text))
         {
              (*f3) = ffile->readAll();
              ffile->close();
         }
         else (*f3)="Impossible d'ouvrir le fichier !";

        ffile->close();


        Aamenu *nouvelle_fenetre=new Aamenu(h,(*f2),(*f3),(*f4),this);
        nouvelle_fenetre->exec();
           }else{
           file1->setText("file does'nt exist");
       }
    }else{
        file->setText("file does'nt exist");
    }

}

