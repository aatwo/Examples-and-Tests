#include <iostream>
#include <QtCore>
#include <QtConcurrent>
#include <thread>
#include <functional>
#include "timer.h"


using namespace std;


int main( int argc, char* argv[] )
{
    Q_UNUSED( argc )
    Q_UNUSED( argv )

    //------------------------------------------------------------------------------
    // QtConcurrent::blockingMapped
    {
        // Create list of values
        QList<int> valueList;
        for( int i = 0; i < 10; i++ )
            valueList.append( i );

        // Function that takes some time to finish
        auto mapFunction = []( const int& value )
        {
            this_thread::sleep_for( chrono::milliseconds( 1000 ) );
            return value * 10;
        };

        // Wrap our lambda in an std::function
        function<int(const int&)> mapFunctionWrapper = mapFunction;

        Timer t;
        QList<int> mappedValueList = QtConcurrent::blockingMapped( valueList, mapFunctionWrapper );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        for( int value : mappedValueList )
            cout << "Value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // TODO continue


    int value;
    cin >> value;
    return 0;
}
