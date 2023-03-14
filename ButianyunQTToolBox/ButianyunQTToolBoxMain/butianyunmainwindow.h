/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#ifndef BUTIANYUNMAINWINDOW_H
#define BUTIANYUNMAINWINDOW_H

#include <QMainWindow>

class ButianyunQMLCacheGeneratorWidget;

class ButianyunMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ButianyunMainWindow(QWidget *parent = nullptr);
    ~ButianyunMainWindow();

private:
    void initialize_menu_bar();
    ButianyunQMLCacheGeneratorWidget* qml_cache_generator_widget;
};
#endif // BUTIANYUNMAINWINDOW_H
