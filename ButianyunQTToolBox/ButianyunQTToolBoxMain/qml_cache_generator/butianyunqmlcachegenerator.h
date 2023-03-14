/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#ifndef BUTIANYUNQMLCACHEGENERATOR_H
#define BUTIANYUNQMLCACHEGENERATOR_H

#include <QObject>

struct ButianyunQMLCacheGeneratorContext
{
    QString qmlcachegen_program;
    QStringList input_filenames;
    QString output_dir;
    QString qml_resource_prefix;
    QString qmlc_resource_prefix;
    QString qml_qmldir_module;
    QString qml_qmldir_prefer;
};

enum class ButianyunQMLCacheGeneratorPhase
{
     TotalBegin,
     Begin, End,
     TotalEnd
};


struct ButianyunQMLCacheGeneratorStatus
{
    ButianyunQMLCacheGeneratorPhase phase;
    int result;
    int total_file_count;
    int success_file_count;
    int failed_file_count;
    int current_file_index;
    QString input_filename;
};

class ButianyunQMLCacheGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ButianyunQMLCacheGenerator(QObject *parent = nullptr);

    void generate(ButianyunQMLCacheGeneratorContext context);

    int generate_qrc_qml_js(const ButianyunQMLCacheGeneratorContext& context);
    int generate_qrc_qmlc(const ButianyunQMLCacheGeneratorContext& context);
    int generate_qmldir(const ButianyunQMLCacheGeneratorContext& context);
    int generate_configuration(const ButianyunQMLCacheGeneratorContext& context);
    int  generate(const ButianyunQMLCacheGeneratorContext& context, int index);

signals:
    void sig_message(QString message);
    void sig_generate(ButianyunQMLCacheGeneratorContext context);
    void sig_qml_cache_generate_status_report(ButianyunQMLCacheGeneratorStatus status);


private slots:
    void slot_generate(ButianyunQMLCacheGeneratorContext context);
};

#endif // BUTIANYUNQMLCACHEGENERATOR_H
