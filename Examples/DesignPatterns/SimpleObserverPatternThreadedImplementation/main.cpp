#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <functional>
#include <vector>
#include <chrono>


//------------------------------------------------------------------------------
// SimpleObserverPatternThreadedImplementation
//
//      This is a very simple implementation of the observer pattern using only
//      the standard library. It employs two threads. One for the producer and
//      another for all the observers.
//------------------------------------------------------------------------------


using namespace std;


mutex observerTerminateLock;
bool observerTerminate = false;


class Producer;


class Observer
{
    public:

        Observer()
        {
            static int nextId = 0;
            nextId++;

            mId = nextId;
        }

        void dataReady( string data )
        {
            mDataLock.lock();
            for( char c : data )
                mData.push_back( c );
            mDataLock.unlock();

            cout << "Observer " << mId << " recieved data: " << data << " (" << data.size() << " chars)...\n";
        }

        string getData()
        {
            string ret;
            mDataLock.lock();
            ret = mData;
            mDataLock.unlock();
            return ret;
        }


    private:

        int                 mId;
        mutex               mDataLock;
        string              mData;
};


class Producer
{
    public:

        Producer( int testDurationS )
        {
            mTestDurationS = testDurationS;
        }

        // Worker function
        void exec()
        {
            auto randomNumber = []( int min, int max )
            {
                return rand() % ( max - min + 1 ) + min;
            };

            auto randomChar = [=]()
            {
                return static_cast<char>( randomNumber( 33, 126 ) );
            };

            auto start = chrono::high_resolution_clock::now();

            while( true )
            {
                bool quit = false;
                mTerminateLock.lock();
                quit = mTerminate;
                mTerminateLock.unlock();

                if( quit )
                    break;

                string newData;

                mDataLock.lock();
                // Generate a random number of random chars
                for( int i = 0; i < randomNumber( 1, 10 ); i++ )
                {
                    char c = randomChar();
                    newData.push_back( c );
                    mData.push_back( c );
                }
                mDataLock.unlock();

                // Notify observers about new data
                mObserverListLock.lock();
                for( auto consumer : mObservers )
                    consumer->dataReady( newData );
                mObserverListLock.unlock();

                // Quit after 10 seconds...
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double, std::milli> elapsed = end - start;
                if( elapsed.count() >= ( mTestDurationS * 1000 ) )
                    break;

                // Sleep for a random number of milliseconds
                this_thread::sleep_for( chrono::milliseconds( randomNumber( 2, 50 ) ) );
            }
        }


        void registerObserver( Observer* c ) // Reentrant
        {
            mObserverListLock.lock();
            mObservers.push_back( c );
            mObserverListLock.unlock();
        }

        void terminate() // Reentrant
        {
            mTerminateLock.lock();
            mTerminate = true;
            mTerminateLock.unlock();
        }


    private:

        mutex               mDataLock;
        string              mData;

        mutex               mObserverListLock;
        vector<Observer*>   mObservers;

        mutex               mTerminateLock;
        bool                mTerminate = false;
        int                 mTestDurationS;


};


void producer_thread_function( Producer* p )
{
    srand( time( nullptr ) );
    cout << "Producer thread started...\n";
    p->exec();
    cout << "Producer thread finished...\n";
}

void observer_thread_function( Producer* p, vector<Observer*>& observers, int observerCount )
{
    srand( time( nullptr ) );
    cout << "Observer thread started...\n";

    // Create 2 observers and register them with the producer
    for( int i = 0; i < observerCount; i++ )
    {
        auto c = new Observer();
        observers.push_back( c );
        p->registerObserver( c );
    }

    while( true )
    {
        bool quit = false;
        observerTerminateLock.lock();
        if( observerTerminate )
            quit = true;
        observerTerminateLock.unlock();

        if( quit )
            break;
    }

    cout << "Observer thread finished...\n";
}


int main( int argc, char *argv[] )
{
    srand( time( nullptr ) );

    const int observerCount = 5;
    const int testDurationS = 10;

    auto producer = new Producer( testDurationS );
    thread producerThread( producer_thread_function, producer );

    vector<Observer*> observerList;
    thread observerThread( observer_thread_function, producer, ref( observerList ), observerCount );

    // Wait for the producer thread to finish...
    producerThread.join();

    // Stop the observer thread
    observerTerminateLock.lock();
    observerTerminate = true;
    observerTerminateLock.unlock();
    observerThread.join();

    cout << "Finished...\n\n";

    // Print the observer data
    for( auto observer : observerList )
        cout << "Observer data: " << observer->getData() << "...\n";

    // Check all data was equal
    bool allDataEqual = true;
    string lastData = observerList[ 0 ]->getData();
    for( int i = 1; i < observerList.size(); i++ )
    {
        auto observer = observerList[ i ];

        string currentData = observer->getData();
        if( currentData != lastData )
            allDataEqual = false;

        lastData = currentData;
    }

    if( allDataEqual )
        cout << "All observer data is equal...\n";
    else
        cout << "Some observer data was not equal!\n";

    cout << "Finished for reals...\n\n";

    // Cleanup
    delete producer;
    for( auto observer : observerList )
        delete observer;

    string input;
    cin >> input;

    return 0;
}
