#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

#include <memory>
#include "common.h"
#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register the namespace
    auto qml_import_name = "Whatever.common";
    auto namespace_name_to_use_in_qml = "CommonNamespace";
    qmlRegisterUncreatableMetaObject(common::staticMetaObject, qml_import_name, 1, 0, namespace_name_to_use_in_qml,  "namespace not creatable");

    // make common::MyEnum available
    qRegisterMetaType<common::MyEnum>("common::MyEnum");
    qRegisterMetaType<common::MyEnum>("MyEnum");

    // make common::MyStruct available
    qRegisterMetaType<common::MyStruct>("common::MyStruct");
    qRegisterMetaType<common::MyStruct>("MyStruct");

    // See NB at page bottom for info about why the types are double registered

    // Create test object which has some C++ functions we want to call from QML
    // which take some C++ types as parameters (see common.h)
    auto testObject = std::make_unique<Test>();
    engine.rootContext()->setContextProperty("testObject", testObject.get());

    // C++ objects cannot be created by QML so we use a simple factory class to
    // create them
    auto commonFactory = std::make_unique<common::Factory>();
    engine.rootContext()->setContextProperty("commonFactory", commonFactory.get());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

// NB
//      We double register types this way to account for situations where the
//      type may not be fully qualified.

// For example
//      If we didn't register "MyStruct" the call to common::Factory::makeMyStruct()
//      would fail in QML because if you look at how the function prototype is written in
//      common.h the return type is specified as "MyStruct" instead of "common::MyStruct"
