#include "menu.h"
#include"amenu.h"

Menu::Menu():QWidget()
{
    QWidget::setWindowTitle ( "MAW GENRATOR" );

    setFixedSize(672, 480);
    I1=new QLabel(this);
    I1->setPixmap(QPixmap("menu.png"));
    P1=new QPushButton("Start !",this);
    P2=new QPushButton("Help",this);
    style="QPushButton {background-color: #C3C3C3;border-style: outset;border-width: 2px;border-radius: 10px;border-color: beige;font: bold 14px;min-width: 10em;padding: 6px;}QPushButton:pressed {background-color: #B9ACAB;border-style: inset;}";

    P2->setStyleSheet(style);
    P1->setStyleSheet(style);
    P1->move(240,340);
    P2->move(240,300);


    QObject::connect(P2, SIGNAL(clicked()), this, SLOT(ouvrirhelp()));
    QObject::connect(P1, SIGNAL(clicked()), this, SLOT(commencetraitement()));

    }
void Menu::ouvrirhelp(){


   QMessageBox M1;
   QFile *ffile=new QFile("help.txt");
   QString  *f2=new QString();
   QTextEdit *T1;

   if(ffile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
         (*f2) = ffile->readAll();
         ffile->close();
    }
    else (*f2)="Impossible to open the file !";
    M1.setText((*f2));
    M1.exec();
}
void Menu::commencetraitement(){

    Amenu *nouvelle_fenetre=new Amenu(this);
    nouvelle_fenetre->exec();
}
