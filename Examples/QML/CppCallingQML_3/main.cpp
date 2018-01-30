#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <memory>
#include <QDebug>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    { // Call the javascript function "testFunction()" found in main.qml

        auto rootObject = engine.rootObjects().first();
        QVariant ret;
        QVariant param = "C++ to javascript!";
        QMetaObject::invokeMethod( rootObject, "testFunction", Q_RETURN_ARG( QVariant, ret ), Q_ARG( QVariant, param ) );
        qDebug() << "Javascript return value: " << ret;
    }

    return app.exec();
}
