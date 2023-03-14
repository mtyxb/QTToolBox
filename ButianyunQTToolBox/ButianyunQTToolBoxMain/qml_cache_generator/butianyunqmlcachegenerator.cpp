/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunqmlcachegenerator.h"
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

ButianyunQMLCacheGenerator::ButianyunQMLCacheGenerator(QObject *parent)
    : QObject{parent}
{
    connect(this, &ButianyunQMLCacheGenerator::sig_generate,
            this, &ButianyunQMLCacheGenerator::slot_generate);

}

struct ButianyunStatusReportWrapper
{
    ButianyunQMLCacheGeneratorStatus status;
    ButianyunQMLCacheGenerator* generator;
    const ButianyunQMLCacheGeneratorContext& context;

    ButianyunStatusReportWrapper(ButianyunQMLCacheGenerator* gen, const ButianyunQMLCacheGeneratorContext& ctx)
        : generator(gen)
        , context(ctx)
    {
        emit generator->sig_message(QObject::tr("Start to generate ..."));
        status.phase = ButianyunQMLCacheGeneratorPhase::TotalBegin;
        status.result = 0;
        status.current_file_index = 0;
        status.success_file_count = 0;
        status.failed_file_count = 0;
        status.total_file_count = context.input_filenames.length();
        status.input_filename = "";
        emit generator->sig_qml_cache_generate_status_report(status);
    }

    ~ButianyunStatusReportWrapper()
    {
        emit generator->sig_message(QObject::tr("Finished"));
        status.phase = ButianyunQMLCacheGeneratorPhase::TotalEnd;
        status.input_filename = "";
        emit generator->sig_qml_cache_generate_status_report(status);
    }

    void report(int index, ButianyunQMLCacheGeneratorPhase phase, int result)
    {
        status.current_file_index = index;
        status.phase = phase;
        status.result = result;
        if (ButianyunQMLCacheGeneratorPhase::End == phase)
        {
            status.failed_file_count += (result ? 1 : 0);
            status.success_file_count += (result ? 0 : 1);
            emit generator->sig_message(QObject::tr("Generated %1 : %2 : {%3} ...")
                                        .arg(index)
                                        .arg(result? QObject::tr("Failed") : QObject::tr("Success"))
                                        .arg(context.input_filenames[index])
                                        );
        }
        status.input_filename = context.input_filenames[index];
        emit generator->sig_qml_cache_generate_status_report(status);
    }
};

struct ButianyunStatusReportCurrentWrapper
{
    ButianyunStatusReportWrapper& wrapper;
    int current_file_index;
    int status;
    ButianyunStatusReportCurrentWrapper(ButianyunStatusReportWrapper& SRW, int index)
        : wrapper(SRW)
        , current_file_index(index)
        , status(0)
    {
        wrapper.report(index, ButianyunQMLCacheGeneratorPhase::Begin, true);
    }

    ~ButianyunStatusReportCurrentWrapper()
    {
        wrapper.report(current_file_index, ButianyunQMLCacheGeneratorPhase::End, status);
    }

    void failed(int result)
    {
        status = result;
    }
};


void ButianyunQMLCacheGenerator::generate(ButianyunQMLCacheGeneratorContext context)
{
    emit sig_generate(context);
}

void ButianyunQMLCacheGenerator::slot_generate(ButianyunQMLCacheGeneratorContext context)
{
    ButianyunStatusReportWrapper status_report(this, context);

    int r;
    r = generate_qrc_qml_js(context);
    emit sig_message(tr("qml.qrc: %1").arg(r));

    for (int i = 0; i < context.input_filenames.length(); i++)
    {
        ButianyunStatusReportCurrentWrapper current_status_report(status_report, i);
        int result = generate(context, i);
        qInfo().noquote().nospace() << i << " " << result << " " << context.input_filenames[i];
        if (0 != result)
        {
            current_status_report.failed(result);
        }
    }

    r = generate_qrc_qmlc(context);
    emit sig_message(tr("qmlc.qrc: %1").arg(r));

    r = generate_configuration(context);
    emit sig_message(tr("configuration.json: %1").arg(r));

    r = generate_qmldir(context);
    emit sig_message(tr("qmldir: %1").arg(r));
}

int ButianyunQMLCacheGenerator::generate_qrc_qml_js(const ButianyunQMLCacheGeneratorContext& context)
{
    QString qrc_qml_js = QString(R"(<!DOCTYPE RCC>
<RCC version="1.0">
<qresource prefix="%1">
<file>qmldir</file>
)")
    .arg(context.qml_resource_prefix);

    for (int i = 0; i < context.input_filenames.length(); i++)
    {
        QFileInfo fi(context.input_filenames[i]);
        qrc_qml_js += QString("<file>%1</file>%2")
                .arg(fi.fileName())
                .arg(i + 1 == context.input_filenames.length() ? "" : "\r\n");
    }

    qrc_qml_js += R"(
</qresource>
</RCC>
)";

    QFile f(context.output_dir + QDir::separator() + "qml.qrc");
    if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return 1;
    }

    QByteArray bytes = qrc_qml_js.toUtf8();
    if (f.write(bytes) != bytes.length())
    {
        f.close();
        return 2;
    }
    f.close();

    return 0;
}

int ButianyunQMLCacheGenerator::generate_qrc_qmlc(const ButianyunQMLCacheGeneratorContext& context)
{
    QString qrc_qmlc = QString(R"(<!DOCTYPE RCC>
<RCC version="1.0">
<qresource prefix="%1">
<file>qmldir</file>
)")
    .arg(context.qmlc_resource_prefix);

    for (int i = 0; i < context.input_filenames.length(); i++)
    {
        QFileInfo fi(context.input_filenames[i]);
        QString qmlc_filename = fi.fileName() + "c";
        qrc_qmlc += QString("<file>%1</file>%2")
                .arg(qmlc_filename)
                .arg(i + 1 == context.input_filenames.length() ? "" : "\r\n");
    }

    qrc_qmlc += R"(
</qresource>
</RCC>
)";

    QFile f(context.output_dir + QDir::separator() + "qmlc.qrc");
    if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return 200;
    }

    QByteArray bytes = qrc_qmlc.toUtf8();
    if (f.write(bytes) != bytes.length())
    {
        f.close();
        return 201;
    }
    f.close();

    return 0;
    return 0;
}



int ButianyunQMLCacheGenerator::generate(const ButianyunQMLCacheGeneratorContext& context, int index)
{
    QString  input_filename = context.input_filenames[index];

    QFileInfo fi(input_filename);
    QString output_filename = fi.fileName() + "c";

    QProcess process;
    process.setProgram(context.qmlcachegen_program);

    QStringList args;
    args << "--resource"
         <<  context.output_dir + QDir::separator() +  "qml.qrc"
         <<  "--only-bytecode"
         << "-o"
         << context.output_dir +  QDir::separator() + output_filename
         << input_filename;

    process.setArguments(args);

    qInfo().noquote().nospace() << context.qmlcachegen_program << "  " << args.join(" ");

    process.start();
    process.waitForStarted();
    if (process.state() != QProcess::Running)
    {
        return 100;
    }

    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit)
    {
        return 101;
    }
    return 0;
}

int ButianyunQMLCacheGenerator::generate_configuration(const ButianyunQMLCacheGeneratorContext& context)
{
    QJsonObject root;
    root.insert("version", QJsonValue(1.0));
    root.insert("format", "Butianyun QT ToolBox For QML");

    QJsonArray group;
    for (int i = 0; i < context.input_filenames.length(); i++)
    {
        QVariantMap unit;
        QString input_filename = context.input_filenames[i];
        QFileInfo fi(input_filename);
        QString url = context.qml_resource_prefix + fi.fileName();
        unit.insert("url", url);
        QString filename = ":" + context.qmlc_resource_prefix +  fi.fileName() + "c";
        unit.insert("filename", filename);
        group.append(QJsonValue::fromVariant(unit));
    }
    root.insert("group", QJsonValue::fromVariant(group.toVariantList()));
    QJsonDocument doc;
    doc.setObject(root);
    QByteArray bytes = doc.toJson();
    QFile f(context.output_dir + QDir::separator() + "configuration.json");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return 300;
    }

    if (f.write(bytes) != bytes.length())
    {
        f.close();
        return 301;
    }

    f.close();

    return 0;
}


int ButianyunQMLCacheGenerator::generate_qmldir(const ButianyunQMLCacheGeneratorContext& context)
{
    QString qmldir = QString(R"(module %1
prefer :%2
)")
    .arg(context.qml_qmldir_module)
    .arg(context.qml_qmldir_prefer);

    for (int i = 0; i < context.input_filenames.length(); i++)
    {
        QFileInfo fi(context.input_filenames[i]);
        QString basename = fi.baseName();
        QString filename = fi.fileName();
        if (filename.endsWith(".qml") && basename.first(1).isUpper())
        {
           qmldir += QString("%1 1.0 %2\r\n")
                .arg(basename)
                .arg(filename);
        }
    }


    QFile f(context.output_dir + QDir::separator() + "qmldir");
    if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return 400;
    }

    QByteArray bytes = qmldir.toUtf8();
    if (f.write(bytes) != bytes.length())
    {
        f.close();
        return 401;
    }
    f.close();

    return 0;
}
