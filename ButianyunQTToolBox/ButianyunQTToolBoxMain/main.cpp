/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunmainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/butianyun_logo.png"));
    a.setApplicationDisplayName(QStringLiteral("Butianyun QT Tool Box V0.1"));
    ButianyunMainWindow w;
    w.setMinimumSize(800, 600);
    w.showMaximized();
    return a.exec();
}
