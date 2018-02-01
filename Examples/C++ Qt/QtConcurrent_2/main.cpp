#include <iostream>
#include <QtCore>
#include <QtConcurrent>
#include <QFuture>
#include <thread>
#include <functional>
#include "timer.h"


const int delayDurationMs = 500;
// For the reduce functions the possible reduce options are:
//      QtConcurrent::UnorderedReduce   - reduction done in any order
//      QtConcurrent::OrderedReduce     - reduction done in order of the original sequence
//      QtConcurrent::SequentialReduce  - reduction done sequentially (one thread at a time)


using namespace std;


void filteredReduced_reduceFunction( int& result, const int value )
{
    result += value;
}

void mappedReduced_reduceFunction( int& result, const int value )
{
    result -= value;
}

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
    // QtConcurrent::filter
    {
        cout << "QtConcurrent::filter...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto filterFunction = []( const int value ) -> bool
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            bool isOdd = ( value % 2 > 0 );
            if( isOdd )
                return false;

            return true;
        };

        // Wrap our lambda in an std::function
        function<bool(const int)> filterFunctionWrapper = filterFunction;

        Timer t;
        QFuture<void> filterListFuture = QtConcurrent::filter( valueList, filterFunctionWrapper );
        filterListFuture.waitForFinished();
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";

        for( int value : valueList )
        {
            cout << "Value: " << value << "\n";
        }
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::filtered
    {
        cout << "\n\nQtConcurrent::filtered...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto filterFunction = []( const int value ) -> bool
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            bool isEven = ( value % 2 == 0 );
            if( isEven )
                return false;

            return true;
        };

        // Wrap our lambda in an std::function
        function<bool(const int)> filterFunctionWrapper = filterFunction;

        Timer t;
        QFuture<int> filteredListFuture = QtConcurrent::filtered( valueList, filterFunctionWrapper );
        filteredListFuture.waitForFinished();
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";

        for( int value : filteredListFuture )
        {
            cout << "Value: " << value << "\n";
        }
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::filteredReduced
    {
        cout << "\n\nQtConcurrent::filteredReduced...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto filterFunction = []( const int value ) -> bool
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            bool isDivisibleByThree = ( value % 3 == 0 );
            if( !isDivisibleByThree )
                return false;

            return true;
        };

        // Wrap our lambda in an std::function
        function<bool(const int)> filterFunctionWrapper = filterFunction;

        Timer t;
        QtConcurrent::ReduceOptions reduceOptions = QtConcurrent::UnorderedReduce | QtConcurrent::SequentialReduce;
        QFuture<int> filteredListFuture = QtConcurrent::filteredReduced( valueList, filterFunctionWrapper, filteredReduced_reduceFunction, reduceOptions );
        filteredListFuture.waitForFinished();
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";

        // Note there should only be one result since the reduce function produces a single value output
        int result = filteredListFuture.result();
        cout << "Reduction result: " << result << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::map
    {
        cout << "\n\nQtConcurrent::map...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto mapFunction = []( int& value ) -> void
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            value *= 6;
        };

        // Wrap our lambda in an std::function
        function<void(int&)> mapFunctionWrapper = mapFunction;

        Timer t;
        QFuture<void> mapListFuture = QtConcurrent::map( valueList, mapFunctionWrapper );
        mapListFuture.waitForFinished();
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";

        for( int value : valueList )
            cout << "value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::mapped
    {
        cout << "\n\nQtConcurrent::mapped...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto mapFunction = []( const int value ) -> int
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            return value * 7;
        };

        // Wrap our lambda in an std::function
        function<int(const int)> mapFunctionWrapper = mapFunction;

        Timer t;
        QFuture<int> mappedListFuture = QtConcurrent::mapped( valueList, mapFunctionWrapper );
        mappedListFuture.waitForFinished();
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";

        for( int value : mappedListFuture )
            cout << "value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::mappedReduced
    {
        cout << "\n\nQtConcurrent::mappedReduced...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto mapFunction = []( const int value ) -> int
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            return value * 7;
        };

        // Wrap our lambda in an std::function
        function<int(const int)> mapFunctionWrapper = mapFunction;

        Timer t;
        QtConcurrent::ReduceOptions reduceOptions = QtConcurrent::UnorderedReduce | QtConcurrent::SequentialReduce;
        QFuture<int> processedListFuture = QtConcurrent::mappedReduced( valueList, mapFunctionWrapper, mappedReduced_reduceFunction, reduceOptions );
        processedListFuture.waitForFinished();
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";

        // Note there should only be one result since the mappedReduced function produces a single value output
        int result = processedListFuture.result();
        cout << "Reduction result: " << result << "\n";
    }

    int value;
    cin >> value;
    return 0;
}
