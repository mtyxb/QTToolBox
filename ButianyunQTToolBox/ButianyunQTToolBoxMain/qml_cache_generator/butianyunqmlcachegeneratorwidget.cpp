/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunqmlcachegeneratorwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QThread>
#include <QDir>
#include <QFileDialog>
#include <QTextBrowser>
#include <QLineEdit>
#include <QSettings>
#include <QIcon>

QSettings settings()
{
    return QSettings("Butianyun", "ButianyunQMLCacheGenerator");
}

ButianyunQMLCacheGeneratorWidget::ButianyunQMLCacheGeneratorWidget(QWidget *parent)
    : QWidget{parent}
{
    setWindowTitle(tr("QT QML Cache Generator"));
    QSettings options = settings();

    QVBoxLayout* main_layout = new QVBoxLayout();
    setLayout(main_layout);

    QHBoxLayout* hbox_layout = new QHBoxLayout();
    program_label = new QLabel(tr("qmlcachegen Program"));
    hbox_layout->addWidget(program_label);
    program_edit = new QLineEdit();
    program_edit->setText(options.value("qmlcachegen_program").toString());
    program_edit->setReadOnly(true);
    hbox_layout->addWidget(program_edit);

    program_button = new QPushButton();
    program_button->setText(tr("Select"));
    hbox_layout->addWidget(program_button);
    main_layout->addLayout(hbox_layout);

    hbox_layout = new QHBoxLayout();
    output_dir_label = new QLabel(tr("Output Directory"));
    hbox_layout->addWidget(output_dir_label);
    output_dir_edit = new QLineEdit();
    output_dir_edit->setReadOnly(true);
    output_dir_edit->setText(options.value("qmlcachegen_output_dir").toString());
    hbox_layout->addWidget(output_dir_edit);

    select_output_dir_button = new QPushButton();
    select_output_dir_button->setText(tr("Select"));
    hbox_layout->addWidget(select_output_dir_button);
    main_layout->addLayout(hbox_layout);

    QString value;

    hbox_layout = new QHBoxLayout();
    resource_qml_prefix_label = new QLabel();
    resource_qml_prefix_label->setText(tr("QML QRC Prefix"));
    hbox_layout->addWidget(resource_qml_prefix_label);
    resource_qml_prefix_edit = new QLineEdit();
    value = options.value("qml_resource_prefix").toString();
    if (value.trimmed().isEmpty())
    {
        value = "/qml/";
    }
    resource_qml_prefix_edit->setText(value);
    hbox_layout->addWidget(resource_qml_prefix_edit);

    resource_qmlc_prefix_label = new QLabel();
    resource_qmlc_prefix_label->setText(tr("QML QRC Prefix"));
    hbox_layout->addWidget(resource_qmlc_prefix_label);
    resource_qmlc_prefix_edit = new QLineEdit();
    value = options.value("qmlc_resource_prefix").toString();
    if (value.trimmed().isEmpty())
    {
        value = "/qml/";
    }
    resource_qmlc_prefix_edit->setText(value);
    hbox_layout->addWidget(resource_qmlc_prefix_edit);

    qmldir_module_label = new QLabel();
    qmldir_module_label->setText(tr("qmldir module"));
    hbox_layout->addWidget(qmldir_module_label);
    qmldir_module_edit = new QLineEdit();
    value = options.value("qml_qmldir_module").toString();
    if (value.trimmed().isEmpty())
    {
        value = "qml";
    }
    qmldir_module_edit->setText(value);
    hbox_layout->addWidget(qmldir_module_edit);


    qmldir_prefer_label = new QLabel();
    qmldir_prefer_label->setText(tr("qmldir prefer"));
    hbox_layout->addWidget(qmldir_prefer_label);
    qmldir_prefer_edit = new QLineEdit();
    value = options.value("qml_qmldir_prefer").toString();
    if (value.trimmed().isEmpty())
    {
        value = "/qml/";
    }
    qmldir_prefer_edit->setText(value);
    hbox_layout->addWidget(qmldir_prefer_edit);

    main_layout->addLayout(hbox_layout);


    input_file_list_label = new QLabel(tr("Input file list"));
    main_layout->addWidget(input_file_list_label);

    file_list_widget = new QListWidget();
    file_list_widget->setSortingEnabled(false);
    file_list_widget->setDragEnabled(false);
    file_list_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    file_list_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    file_list_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    main_layout->addWidget(file_list_widget);


    hbox_layout = new QHBoxLayout();
    browser_button = new QPushButton();
    browser_button->setText(tr("Browser QML & JavaScript Files"));
    hbox_layout->addWidget(browser_button);

    generator_button = new QPushButton();
    generator_button->setText(tr("Generate QML cache"));
    hbox_layout->addWidget(generator_button);
    main_layout->addLayout(hbox_layout);


    status_label = new QLabel(tr("Ready"));
    main_layout->addWidget(status_label);

    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    main_layout->addWidget(progress_bar);

    text_log = new QTextBrowser();
    main_layout->addWidget(text_log);

    generator_thread = new QThread(this);
    generator = new ButianyunQMLCacheGenerator;
    generator->moveToThread(generator_thread);


    connect(generator, &ButianyunQMLCacheGenerator::sig_qml_cache_generate_status_report,
            this, &ButianyunQMLCacheGeneratorWidget::slot_qml_cache_generate_status_report);
    connect(generator, &ButianyunQMLCacheGenerator::sig_message,
            this, &ButianyunQMLCacheGeneratorWidget::slot_message);

    connect(program_button, &QPushButton::clicked, this, &ButianyunQMLCacheGeneratorWidget::slot_select_qmlcachegen_program);
    connect(select_output_dir_button, &QPushButton::clicked, this, &ButianyunQMLCacheGeneratorWidget::slot_select_output_dir);
    connect(browser_button, &QPushButton::clicked, this, &ButianyunQMLCacheGeneratorWidget::slot_browser);
    connect(generator_button, &QPushButton::clicked, this, &ButianyunQMLCacheGeneratorWidget::slot_generate);

    generator_thread->start();

}

void ButianyunQMLCacheGeneratorWidget::slot_message(QString message)
{
    text_log->append(message);
}

void ButianyunQMLCacheGeneratorWidget::slot_qml_cache_generate_status_report(ButianyunQMLCacheGeneratorStatus status)
{
    QString message;
    int progress;

    if (status.phase == ButianyunQMLCacheGeneratorPhase::TotalBegin)
    {
            message = QString(tr("Starting to process Total %1 Files"))
            .arg(status.total_file_count);
            progress = 0;
    }
    else if (status.phase == ButianyunQMLCacheGeneratorPhase::TotalEnd)
    {
       message  = QString(tr("Total: %1 Success: %2 Failed: %3"))
                .arg(status.total_file_count)
                .arg(status.success_file_count)
                .arg(status.failed_file_count);
       if (status.total_file_count > 0)
       {
          progress = (status.current_file_index + 1) * 100 / status.total_file_count;
       }
    }
    else if (status.phase == ButianyunQMLCacheGeneratorPhase::Begin)
    {
       message  = QString(tr("%1 of %2 phase:%3  filename:{%4} ..."))
                .arg(status.current_file_index)
                .arg(status.total_file_count)
                .arg(status.phase == ButianyunQMLCacheGeneratorPhase::Begin)
               .arg(status.input_filename);
       progress = status.current_file_index * 100 / status.total_file_count;
    }
    else if (status.phase == ButianyunQMLCacheGeneratorPhase::End)
    {
        message  = QString(tr("%1 of %2 phase:%3 result:%4  filename:{%5}"))
                 .arg(status.current_file_index)
                 .arg(status.total_file_count)
                 .arg(status.phase == ButianyunQMLCacheGeneratorPhase::Begin)
                 .arg(status.result ? tr("Failed") : tr("Success"))
                .arg(status.input_filename);
        progress = (status.current_file_index + 1) * 100 / status.total_file_count;
    }

    status_label->setText(message);
    progress_bar->setValue(progress);

}


void ButianyunQMLCacheGeneratorWidget::slot_select_qmlcachegen_program()
{
    QSettings options = settings();
    QString  lastdir = options.value("qmlcachegen_program_lastdir").toString();
    QString filename= QFileDialog::getOpenFileName(this,
                    tr("Select qmlcachegen executable program"),
                    lastdir,
                    QString(tr("Executable files (qmlcachegen.exe);;All files (qmlcachegen))"))
                    );
    if (filename.isEmpty())
    {
        return;
    }
    program_edit->setText(filename);

    QFileInfo fi(filename);
    if (!fi.isFile() || !fi.exists() || !fi.isExecutable())
    {
        slot_message(tr("Operation aborted: qmlcachegen program not exist or not a executable file"));
    }
    options.setValue("qmlcachegen_program_lastdir", fi.absolutePath());
    options.setValue("qmlcachegen_program", filename);
}

void ButianyunQMLCacheGeneratorWidget::slot_select_output_dir()
{
    QString output_dir = QFileDialog::getExistingDirectory(this,
                    tr("Select QML & JavaScript Files"),
                    output_dir_edit->text());

    if (output_dir.isEmpty())
    {
        return;
    }
    output_dir_edit->setText(output_dir);
    QSettings options = settings();
    options.setValue("qmlcachegen_output_dir", output_dir);
}

void ButianyunQMLCacheGeneratorWidget::slot_browser()
{
    QSettings options = settings();
    QString  lastdir = options.value("qmlcachegen_browser_lastdir").toString();

    QStringList filenames= QFileDialog::getOpenFileNames(this,
                    tr("Select QML & JavaScript Files"),
                    lastdir,
                    QString(tr("QML & JavaScript Files (*.qml *.js);;QML Files (*.qml);;JavaScript Files (*.js)"))
                    );

    if (filenames.length() == 0)
    {
        return;
    }


    file_list.append(filenames);
    int new_count = file_list.length();
    int add_count = filenames.length();
    QString message = tr("%1 Files add, Total Files: %2").arg(add_count).arg(new_count);

    for (int i = 0; i < filenames.length(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(filenames[i]);
        file_list_widget->addItem(item);
    }

    status_label->setText(message);

    QFileInfo fi(filenames[0]);
    lastdir = fi.absolutePath();
    options.setValue("qmlcachegen_browser_lastdir", lastdir);
}

void ButianyunQMLCacheGeneratorWidget::slot_generate()
{
    slot_message(tr("Starting ..."));
    progress_bar->setValue(0);

    ButianyunQMLCacheGeneratorContext context;
    context.output_dir = output_dir_edit->text();
    context.input_filenames = file_list;
    context.qmlcachegen_program = program_edit->text();
    context.qml_resource_prefix =  resource_qml_prefix_edit->text().trimmed();
    context.qmlc_resource_prefix = resource_qmlc_prefix_edit->text().trimmed();
    context.qml_qmldir_module =  qmldir_module_edit->text().trimmed();
    context.qml_qmldir_prefer =  qmldir_prefer_edit->text().trimmed();


    if (context.qml_resource_prefix.isEmpty())
    {
        slot_message(tr("Operation aborted: qml_resource_prefix is empty"));
        return;
    }

    if (context.qmlc_resource_prefix.isEmpty())
    {
        slot_message(tr("Operation aborted: qmlc_resource_prefix is empty"));
        return;
    }

    if (context.qml_qmldir_module.isEmpty())
    {
        slot_message(tr("Operation aborted: qml_qmldir_module is empty"));
        return;
    }

    if (context.qml_qmldir_prefer.isEmpty())
    {
        slot_message(tr("Operation aborted: qml_qmldir_prefer is empty"));
        return;
    }

    QSettings options = settings();
    options.setValue("qml_resource_prefix", context.qml_resource_prefix);
    options.setValue("qmlc_resource_prefix", context.qmlc_resource_prefix);
    options.setValue("qml_qmldir_module", context.qml_qmldir_module);
    options.setValue("qml_qmldir_prefer", context.qml_qmldir_prefer);



    if (context.input_filenames.length() == 0)
    {
        slot_message(tr("Operation aborted: No QML & JavaScript files"));
        return;
    }

    if (context.output_dir.isEmpty())
    {
        slot_message(tr("Operation aborted: Output directory is empty"));
        return;
    }

    if (context.qmlcachegen_program.isEmpty())
    {
        slot_message(tr("Operation aborted: qmlcachegen program is empty"));
        return;
    }

    QFileInfo fi(context.qmlcachegen_program);
    if (!fi.isFile() || !fi.exists() || !fi.isExecutable())
    {
        slot_message(tr("Operation aborted: qmlcachegen program not exist or not a executable file"));
        return;
    }

    generator->generate(context);
}
