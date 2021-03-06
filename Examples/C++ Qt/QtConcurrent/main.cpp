#include <iostream>
#include <QtCore>
#include <QtConcurrent>
#include <thread>
#include <functional>
#include "timer.h"


const int delayDurationMs = 500;
// For the reduce functions the possible reduce options are:
//      QtConcurrent::UnorderedReduce   - reduction done in any order
//      QtConcurrent::OrderedReduce     - reduction done in order of the original sequence
//      QtConcurrent::SequentialReduce  - reduction done sequentially (one thread at a time)


using namespace std;


void blockingFilteredReduced_reduceFunction( int& result, const int& value )
{
    result += value;
}

void blockingMappedReduced_reduceFunction( int& result, const int& value )
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
    // QtConcurrent::blockingMapped
    {
        cout << "QtConcurrent::blockingMapped...\n";

        // Create list of values
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto mapFunction = []( const int value ) -> int
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            return value * 10;
        };

        // Wrap our lambda in an std::function
        function<int(const int)> mapFunctionWrapper = mapFunction;

        // Called the blockingMapped function
        Timer t;
        QList<int> mappedValueList = QtConcurrent::blockingMapped( valueList, mapFunctionWrapper );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        for( int value : mappedValueList )
            cout << "Value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::blockingMap
    {
        cout << "\n\nQtConcurrent::blockingMap...\n";

        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto mapFunction = []( int& value ) -> void
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            value *= 11;
        };

        // Wrap our lambda in an std::function
        function<void(int&)> mapFunctionWrapper = mapFunction;

        // Call the blockingMap function (note this doesnt return a copy, instead
        // it directly modifies the original)
        Timer t;
        QtConcurrent::blockingMap( valueList, mapFunctionWrapper );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        for( int value : valueList )
            cout << "Value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // QtConcurrent::blockingMappedReduced
    {
        cout << "\n\nQtConcurrent::blockingMappedReduced...\n";

        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish
        auto mappedFunction = []( const int value ) -> int
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );
            return value * 5;
        };

        // Wrap our lambda in an std::function
        function<int(const int)> mappedFunctionWrapper = mappedFunction;

        Timer t;
        QtConcurrent::ReduceOptions reduceOptions = QtConcurrent::UnorderedReduce | QtConcurrent::SequentialReduce;
        int result = QtConcurrent::blockingMappedReduced( valueList, mappedFunctionWrapper, blockingMappedReduced_reduceFunction, reduceOptions );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        cout << "Reduction result: " << result << "\n";
    }


    //------------------------------------------------------------------------------
    // Qt::Concurrent::blockingFilter
    {
        cout << "\n\nQtConcurrent::blockingFilter...\n";
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish and returns true if the value
        // param should stay in the list or false if it is to be removed
        auto filterFunction = []( const int value ) -> bool
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );

            // Filter out odd numbers
            bool isOdd = ( value % 2 ) > 0;
            if( isOdd )
                return false;

            return true;
        };

        // Wrap our lambda in an std::function
        function<bool(const int)> filterFunctionWrapper = filterFunction;

        Timer t;
        QtConcurrent::blockingFilter( valueList, filterFunctionWrapper );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        for( int value : valueList )
            cout << "Value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // Qt::Concurrent::blockingFiltered
    {
        cout << "\n\nQtConcurrent::blockingFiltered...\n";
        QList<int> valueList = generateValueList( 10 );

        // Function that takes some time to finish and returns true if the value
        // param should stay in the list or false if it is to be removed
        auto filterFunction = []( const int value ) -> bool
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );

            // Filter out even numbers
            bool isEven = ( value % 2 ) == 0;
            if( isEven )
                return false;

            return true;
        };

        // Wrap our lambda in an std::function
        function<bool(const int)> filterFunctionWrapper = filterFunction;

        Timer t;
        QList<int> filteredValueList = QtConcurrent::blockingFiltered( valueList, filterFunctionWrapper );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        for( int value : filteredValueList )
            cout << "Value: " << value << "\n";
    }


    //------------------------------------------------------------------------------
    // Qt::Concurrent::blockingFilteredReduced
    // Note: this function for some reason does not support std::function for the reduce function only
    {
        cout << "\n\nQtConcurrent::blockingFilteredReduced...\n";

        QList<int> valueList = generateValueList( 30 );

        auto filterFunction = []( const int value ) -> bool
        {
            this_thread::sleep_for( chrono::milliseconds( delayDurationMs ) );

            // Filter out numbers not divisible by three
            bool isDivisibleByThree = ( value % 3 ) == 0;
            if( !isDivisibleByThree )
                return false;

            return true;
        };

        function<bool(const int)> filterFunctionWrapper = filterFunction;

        Timer t;
        QtConcurrent::ReduceOptions reduceOptions = QtConcurrent::UnorderedReduce | QtConcurrent::SequentialReduce;
        int result = QtConcurrent::blockingFilteredReduced( valueList, filterFunctionWrapper, blockingFilteredReduced_reduceFunction, reduceOptions );
        auto elapsedMs = t.elapsedMs();

        cout << "Elapsed ms: " << elapsedMs << "\n";
        cout << "Reduction result: " << result << "\n";
    }



    int value;
    cin >> value;
    return 0;
}
