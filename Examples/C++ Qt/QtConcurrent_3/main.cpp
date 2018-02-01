#include <iostream>
#include <QtCore>
#include <QtConcurrent>
#include <QFuture>
#include <QThreadPool>
#include <thread>
#include <functional>
#include "timer.h"


const int delayDurationMs = 1000;


using namespace std;


int main( int argc, char* argv[] )
{
    Q_UNUSED( argc )
    Q_UNUSED( argv )

    // Create list of values
    auto generateValueList = []( int size )
    {
        QList<int> ret;
        for( int i = 0; i < size; i++ )
            ret.append( i );
        return ret;
    };


    //------------------------------------------------------------------------------
    // QtConcurrent::run
    {
        cout << "QtConcurrent::run\n";

        auto workerFunction = []() -> int
        {
            this_thread::sleep_for( std::chrono::milliseconds( delayDurationMs ) );
            return 10;
        };

        QFuture<int> resultFuture1 = QtConcurrent::run( workerFunction );
        QFuture<int> resultFuture2 = QtConcurrent::run( workerFunction );

        resultFuture1.waitForFinished();
        resultFuture2.waitForFinished();

        int result1 = resultFuture1.result();
        int result2 = resultFuture2.result();

        cout << "Result1: " << result1 << "\n";
        cout << "Result2: " << result2 << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::run (with arguments)
    {
        cout << "\n\nQtConcurrent::run with args\n";

        auto workerFunction = []( int a, float b ) -> float
        {
            this_thread::sleep_for( std::chrono::milliseconds( delayDurationMs ) );
            return static_cast<float>( a ) / b;
        };

        QFuture<float> resultFuture = QtConcurrent::run( workerFunction, 10, 3.0f );
        resultFuture.waitForFinished();
        float result = resultFuture.result();

        cout << "Result: " << result << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::run (with thread pool)
    {
        cout << "\n\nQtConcurrent::run using the global threadpool\n";

        auto workerFunction = []( int a, float b ) -> float
        {
            this_thread::sleep_for( std::chrono::milliseconds( delayDurationMs ) );
            return static_cast<float>( a ) / b;
        };

        QFuture<float> resultFuture = QtConcurrent::run( QThreadPool::globalInstance(), workerFunction, 11, 4.5f );
        resultFuture.waitForFinished();
        float result = resultFuture.result();

        cout << "Result: " << result << "\n";
    }


    int value;
    cin >> value;
    return 0;
}
