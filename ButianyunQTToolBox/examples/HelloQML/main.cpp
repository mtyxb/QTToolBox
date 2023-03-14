/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLibrary>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QLibrary lib(R"(libButianyunQMLCacheLoader.dll)");
    qInfo() << lib.load();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
