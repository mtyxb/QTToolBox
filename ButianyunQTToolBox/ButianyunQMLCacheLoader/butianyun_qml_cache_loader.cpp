/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/



#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileInfo>


namespace  butianyun
{
namespace qml
{
    const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[] = { { 0, QMetaType::fromType<void>(), {}, nullptr } };


struct Registry
{
    Registry();
    ~Registry();

    int load();

    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};


};
};


Q_GLOBAL_STATIC(butianyun::qml::Registry, butianyun_qml_unit_registry)


namespace  butianyun
{
namespace qml
{
int Registry::load()
{
    QString json_filename = "configuration.json";
    QFileInfo fi(json_filename);
    QString data_dir = fi.absolutePath() + QDir::separator();
    qInfo().noquote().nospace() << "data_dir:{" << data_dir << "}";

    QByteArray json_text;
    {
        QFile f(json_filename);
        if (!f.open(QIODevice::ReadOnly))
        {
            qInfo().noquote().nospace() << "configuration: Failed to open json configuration file!";
            return 1;
        }
        json_text = f.readAll();
        f.close();
    }

    if (json_text.length() < 10)
    {
        qInfo().noquote().nospace() << "configuration text is too short!";
        return 2;
    }

    QJsonDocument doc = QJsonDocument::fromJson(json_text);
    QJsonObject root = doc.object();
    qInfo() << root.keys();


    if (!root.value("version").isDouble() || root.value("version").toDouble() < 1.0)
    {
        qInfo().noquote().nospace() << "configuration version is too old or invalid!";
        return 3;
    }

    if (!root.value("format").isString() || root.value("format").toString().compare("Butianyun QT ToolBox For QML") != 0)
    {
        qInfo().noquote().nospace() << "configuration format dismatched!";
        return 4;
    }

    QJsonValue group_value = root.value("group");
    if (!group_value.isArray())
    {
        qInfo().noquote().nospace() << "configuration group not found!";
        return 5;
    }
    QJsonArray group = group_value.toArray();
    int count = group.count();
    for (int i = 0; i < count; i++)
    {
        QJsonValue config_value = group.at(i);
        if (!config_value.isObject())
        {
            qInfo().noquote().nospace() << "configuration config value " << i << " is invalid";
            continue;
        }

        QJsonObject config = config_value.toObject();
        QJsonValue url_value = config.value("url");
        QJsonValue filename_value = config.value("filename");

        if (!url_value.isString() || !filename_value.isString())
        {
            qInfo().noquote().nospace() << "configuration config url or filename is invalid";
            continue;
        }

        QString url_str = url_value.toString();
        QString filename_str = filename_value.toString();
        if (url_str.isEmpty() || filename_str.isEmpty())
        {
            qInfo().noquote().nospace() << "configuration config url or filename is empty";
            continue;
        }


        QString qmrc_filename;

        fi.setFile(filename_str);
        if (fi.isRelative())
        {
            qmrc_filename = data_dir + filename_str;
        }
        else
        {
            qmrc_filename = filename_str;
        }
        QFile f(qmrc_filename);
        if (!f.open(QIODevice::ReadOnly))
        {
            qInfo().noquote().nospace() << "configuration config: Failed to open the file: {"
                                        << filename_str << "}"
                                        << " => {"
                                        << qmrc_filename << "}";
            continue;
        }
        int size = f.size();
        char* data = new char[size + 1];
        int len = f.read(data, size);
        f.close();

        if (len != size)
        {
            qInfo().noquote().nospace() << "configuration config: Failed to read data from the file: {"
                                        << filename_str << "}"
                                        << " => {"
                                        << qmrc_filename << "}";            continue;
        }

        QQmlPrivate::CachedQmlUnit* unit = new QQmlPrivate::CachedQmlUnit();
        unit->qmlData =  reinterpret_cast<const QV4::CompiledData::Unit*>(data);
        unit->aotCompiledFunctions = aotBuiltFunctions;
        resourcePathToCachedUnit.insert(url_str, unit);
    }


    return 0;
}


Registry::Registry()
{
    load();
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry()
{
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));

    for (auto & p : resourcePathToCachedUnit)
    {
        delete[] const_cast<char*>(reinterpret_cast<const char*>(p->qmlData));
        delete p;
    }
    resourcePathToCachedUnit.clear();
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url)
{
    if (url.scheme() != QLatin1String("butianyun-qml")
        && url.scheme() != QLatin1String("qml")
        && url.scheme() != QLatin1String("qrc"))
    {
        return nullptr;
    }
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
    {
        return nullptr;
    }
    if (!resourcePath.startsWith(QLatin1Char('/')))
    {
        resourcePath.prepend(QLatin1Char('/'));
    }
    return butianyun_qml_unit_registry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}

int butianyun_qml_qInitResources_qmake_resources()
{
    ::butianyun_qml_unit_registry();
    return 1;
}

Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(butianyun_qml_qInitResources_qmake_resources))


};
};
