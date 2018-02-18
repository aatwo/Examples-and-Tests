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

    { // lambda calling another lambda

        auto lambdaOne = []() -> string
        {
            return "hello from lambda Two";
        };

        auto lambdaTwo = [lambdaOne]()
        {
            cout << "Hello from lambda One + " << lambdaOne() << endl;
        };

        lambdaTwo();
    }

    { // std::function lambda

        function<int(int, int)> lambdaOne = []( int a, int b ) -> int 
        {
            return a * b;
        };

        cout << "Hello from lambda stored as std::function object. 8 * 9 = " << lambdaOne( 8, 9 ) << endl;
    }

    { // Recursive lambda

        int maxCount = 5;
        int count = 0;
        function<void( int& )> lambda;
        lambda = [&lambda, &maxCount]( int& count )
        {
            count++;
            if( count >= maxCount )
                return;

            cout << "Hello from recursive lambda (" << count << ")..." << endl;
            lambda( count );
        };

        lambda( count );
    }

    { // Template lambda

        auto templateLambda = []<typename T>( T a, T b )
        {
            return a * b;
        };

        cout << "Hello from template lambda: " << templateLambda( 4, 5 ) << endl;
        
        // Note the type automatic deduction for T in the function call, and the return type of the lambda. 
    }

    { // Mutable

        struct MyStruct
        {
            MyStruct( int a, int b ) : a( a ), b( b ){}

            int getA() { return a; }
            int getB() const { return b; }

            int a;
            int b;
        };

        MyStruct a( 1, 2 );

        auto lambda = [a]() mutable
        {
            cout << "a = " << a.getA() << ", b = " << a.getB() << endl;
        };

        lambda();

        // THe mutable keyword is required as captured copy variables are const by default. Mutable allows you to call their const member functionss.
    }

    cout << "\n\n";
    return 0;
}