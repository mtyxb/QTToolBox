/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunmainwindow.h"
#include "butianyunqmlcachegeneratorwidget.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

ButianyunMainWindow::ButianyunMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , qml_cache_generator_widget(nullptr)
{
    initialize_menu_bar();

}

ButianyunMainWindow::~ButianyunMainWindow()
{
}

void ButianyunMainWindow::initialize_menu_bar()
{
    QMenuBar* mb = menuBar();

    QMenu* sys_menu = new QMenu(this);
    sys_menu->setTitle(tr("&System"));
    QAction* action_exit = new QAction(this);
    action_exit->setText(tr("&Exit"));
    sys_menu->addAction(action_exit);
    mb->addMenu(sys_menu);

    connect(action_exit, &QAction::triggered, this, [this]{
        qApp->quit();
    });


    QMenu* menu_qml_tool = new QMenu(this);
    menu_qml_tool->setTitle(tr("&QML"));
    QAction* action_qml_cache_generator = new QAction(this);
    action_qml_cache_generator->setText(tr("QML &Cache Generator"));
    menu_qml_tool->addAction(action_qml_cache_generator);
    mb->addMenu(menu_qml_tool);

    connect(action_qml_cache_generator, &QAction::triggered, this, [this]{
        if (nullptr == qml_cache_generator_widget)
        {
            qml_cache_generator_widget = new ButianyunQMLCacheGeneratorWidget();
        }
        qml_cache_generator_widget->showMaximized();
    });



    QMenu* help_menu = new QMenu(this);
    help_menu->setTitle(tr("&Help"));

    QAction* action_about = new QAction(this);
    action_about->setText(tr("&About ..."));
    help_menu->addAction(action_about);

    connect(action_about, &QAction::triggered, this, [this]{
        QMessageBox::information(this, tr("About Butianyun QT Tool Box"),
           tr("Butianyun QT Tool Box\r\nVersion: V0.1\r\n"));
    });

    QAction* action_latest_version = new QAction(this);
    action_latest_version->setText(tr("Get &Latest Version of Butianyun QT Tool Box"));
    help_menu->addAction(action_latest_version);

    connect(action_latest_version, &QAction::triggered, this, [this]{
        QMessageBox::information(this, tr("Latest version"),
           tr("Join QQ group 875965101 to get latest version of Butianyun QT Tool Box"));
    });


    QAction* action_source_code = new QAction(this);
    action_source_code->setText(tr("Get &Source Code of  Butianyun QT Tool Box"));
    help_menu->addAction(action_source_code);

    connect(action_source_code, &QAction::triggered, this, [this]{
        QMessageBox::information(this, tr("Source Code"),
           tr("Join QQ group 875965101 to get source code of Butianyun QT Tool Box"));
    });

    QAction* action_qt_video_lesson = new QAction(this);
    action_qt_video_lesson->setText(tr("&Visit QT Video Lesson V2"));
    help_menu->addAction(action_qt_video_lesson);

    connect(action_qt_video_lesson, &QAction::triggered, this, [this]{
        QDesktopServices::openUrl(QUrl("https://ke.qq.com/course/package/32552?saleToken=2325786&from=pclink"));
    });

    mb->addMenu(help_menu);
}

