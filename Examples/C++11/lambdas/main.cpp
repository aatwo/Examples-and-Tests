#include <iostream>
#include <string>
#include <functional>

using namespace std;

int main( int argc, char** argv )
{
    { // Simple lambda

        auto myLambda = []()
        {
            cout << "Hello from simple lambda!" << endl;
        };

        myLambda();
    }

    { // lambda with params

        auto myLambda = []( int intParam, string stringParam )
        {
            cout << "Hello from lambda with params: " << intParam << " + " << stringParam << endl;
        };

        myLambda( 12, "string param" );
    }

    { // lambda with capture variable by value

        string testString = "test";
        auto myLambda = [testString]()
        {
            cout << "Hello from lambda with capture variable by value: " << testString << endl;
        };

        myLambda();
    }

    { // lambda with capture variable by reference
    
        string testString = "test";
        auto myLambda = [&testString]()
        {
            cout << "Hello from lambda with capture variable by reference: " << testString << endl;
            testString += " modified by lambda";
        };

        myLambda();
        cout << "Test string after lambda call: " << testString << endl;
    }

    { // lambda with capture variable by capture value assignment
    
        string testString = "test";
        auto myLambda = [testString2 = testString + " modified by capture initialisation"]()
        {
            cout << "Hello from lambda with capture variable by capture value assignment: " << testString2 << endl;
        };

        myLambda();
    }

    cout << "\n\n";
    return 0;
}