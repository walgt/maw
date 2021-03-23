#include "aamenu.h"
using namespace std;
Aamenu::Aamenu(QString &S1,QString &S2,QString &S3,QString &S4,QWidget *parent): QDialog(parent)
{
    QWidget::setWindowTitle ( "MAW GENRATOR" );
 setFixedSize(800, 480);
 L1=new QLabel(S1,this);

 T1 = new QTextEdit();
 T1->setPlainText(S2);
 T1->setReadOnly(true);

 T2 = new QTextEdit();
 T2->setPlainText(S3);
 T2->setReadOnly(true);

 T3 = new QTextEdit();
 T3->setPlainText(S4);
 T3->setReadOnly(true);
 BB1=new QPushButton("Save",this);

 QGridLayout *G1 = new QGridLayout;
 G1->addWidget(L1, 0, 0);
 G1->addWidget(T1, 0, 1);
 G1->addWidget(T2, 0, 2);
 G1->addWidget(T3, 0, 3);
 G1->addWidget(BB1, 1, 3);
 this->setLayout(G1);


 QObject::connect(BB1, SIGNAL(clicked()), SLOT(save()));



}


void Aamenu::save(){
         bool ok = false;
        QString pseudo = QInputDialog::getText(this, "Save", "Please enter the filename that will contain the list of minimal absent words  of  human DNA but present in virus. ",QLineEdit::Normal, QString(), &ok);



            if (ok && !pseudo.isEmpty())

            {

                QString d=QFileDialog::getExistingDirectory(this);
                d=d+"/"+pseudo;

                string s="";

                QByteArray byteArray = d.toLatin1();
                const char *characters = byteArray.constData();
                char* foo = const_cast<char*>(characters);
               ifstream fichier("result2.txt");
               ofstream monFlux(foo);


               if(fichier)
               {
                  //L'ouverture s'est bien passée, on peut donc lire

                  string ligne=""; //Une variable pour stocker les lignes lues

                  while(getline(fichier, ligne)) //Tant qu'on n'est pas à la fin, on lit
                  {

                         monFlux<<ligne<<endl;
                  }


               }
               else
               {
                  cout << "ERROR: Could not open file for reading." << endl;
               }


            }

            else

            {

                QMessageBox::critical(this, "Save", "You did not enter the file name");

            }
}
