#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "TodoListModel.h"
#include "TodoListData.h"
#include <memory>
#include <QQmlContext>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<TodoListModel>( "com.MyCompany.qmlComponents", 1, 0, "TodoListModel" );
    qmlRegisterType<TodoListData>( "com.MyCompany.qmlComponents", 1, 0, "TodoListData" );
    std::unique_ptr<TodoListData> todoListData( new TodoListData );
    engine.rootContext()->setContextProperty( "todoListData", todoListData.get() );

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
