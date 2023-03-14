/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#ifndef BUTIANYUNQMLCACHEGENERATORWIDGET_H
#define BUTIANYUNQMLCACHEGENERATORWIDGET_H

#include "butianyunqmlcachegenerator.h"
#include <QWidget>
#include "butianyunqmlcachegenerator.h"


class QLabel;
class QLineEdit;
class QListWidget;
class QProgressBar;
class QPushButton;
class QTextBrowser;
class QThread;
class ButianyunQMLCacheGenerator;
struct ButianyunQMLCacheGeneratorStatus;


class ButianyunQMLCacheGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ButianyunQMLCacheGeneratorWidget(QWidget *parent = nullptr);

signals:

private slots:
    void slot_qml_cache_generate_status_report(ButianyunQMLCacheGeneratorStatus status);
    void slot_select_qmlcachegen_program();
    void slot_select_output_dir();
    void slot_browser();
    void slot_generate();
    void slot_message(QString message);


private:
    QLabel* status_label;
    QLabel* input_file_list_label;
    QListWidget* file_list_widget;
    QLabel*  program_label;
    QLineEdit*  program_edit;
    QLabel*  output_dir_label;
    QLineEdit*  output_dir_edit;
    QLabel*  resource_qml_prefix_label;
    QLineEdit*  resource_qml_prefix_edit;
    QLabel*  resource_qmlc_prefix_label;
    QLineEdit*  resource_qmlc_prefix_edit;
    QLabel*  qmldir_module_label;
    QLineEdit*  qmldir_module_edit;
    QLabel*  qmldir_prefer_label;
    QLineEdit*  qmldir_prefer_edit;
    QPushButton* program_button;
    QPushButton* select_output_dir_button;
    QPushButton* browser_button;
    QPushButton* generator_button;
    QProgressBar* progress_bar;
    QTextBrowser* text_log;
    QStringList file_list;
    ButianyunQMLCacheGenerator* generator;
    QThread* generator_thread;
    QString qmlcachegen_program;
};

#endif // BUTIANYUNQMLCACHEGENERATORWIDGET_H
