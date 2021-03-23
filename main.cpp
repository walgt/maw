#include"amenu.h"
#include<QApplication>
#include<menu.h>
int main(int argc, char *argv[])
{


    QApplication app(argc, argv);

     Menu fenetre;
     fenetre.show();

    return app.exec();
    return 0;
}
