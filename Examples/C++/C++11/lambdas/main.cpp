#include <iostream>
#include <string>
#include <functional>
#include <type_traits>

int main( int argc, char** argv )
{
    { // Simple lambda

        auto myLambda = []()
        {
            std::cout << "Hello from simple lambda!" << std::endl;
        };

        myLambda();
    }

    { // lambda with params

        auto myLambda = []( int intParam, std::string stringParam )
        {
            std::cout << "Hello from lambda with params: " << intParam << " + " << stringParam << std::endl;
        };

        myLambda( 12, "string param" );
    }

    { // lambda with capture variable by value

        std::string testString = "test";
        auto myLambda = [testString]()
        {
            std::cout << "Hello from lambda with capture variable by value: " << testString << std::endl;
        };

        myLambda();
    }

    { // lambda with trailing return type

        { // Without trailing return type this lambda which returns a string literal will return a value of type const char*
            auto lambdaOne = []()
            {
                return "test string";
            };

            bool is_const_char_ptr = std::is_same<decltype(lambdaOne()), const char*>::value;
            std::cout << "Trailing return type ommitted results in a const char* being returned when returning a string literal: " << (is_const_char_ptr ? "true" : "false") << std::endl;
        }

        { // With the trailing return type specified a string will be returned
            auto lambdaTwo = []() -> std::string
            {
                return "test string";
            };

            bool is_string = std::is_same<decltype(lambdaTwo()), std::string>::value;
            std::cout << "Trailing return type specified results in an std::string being returned when returning a string literal: " << (is_string ? "true" : "false") << std::endl;
        }        
    }

    { // lambda with capture variable by reference
    
        std::string testString = "test";
        auto myLambda = [&testString]()
        {
            std::cout << "Hello from lambda with capture variable by reference: " << testString << std::endl;
            testString += " modified by lambda";
        };

        myLambda();
        std::cout << "Test std::string after lambda call: " << testString << std::endl;
    }

    { // lambda with capture variable by capture value assignment
    
        std::string testString = "test";
        auto myLambda = [testString2 = testString + " modified by capture initialisation"]()
        {
            std::cout << "Hello from lambda with capture variable by capture value assignment: " << testString2 << std::endl;
        };

        myLambda();
    }

    { // lambda calling another lambda

        auto lambdaOne = []() -> std::string
        {
            return "hello from lambda Two";
        };

        auto lambdaTwo = [lambdaOne]()
        {
            std::cout << "Hello from lambda One + " << lambdaOne() << std::endl;
        };

        lambdaTwo();
    }

    { // function lambda

        std::function<int(int, int)> lambdaOne = []( int a, int b ) -> int 
        {
            return a * b;
        };

        std::cout << "Hello from lambda stored as function object. 8 * 9 = " << lambdaOne( 8, 9 ) << std::endl;
    }

    { // Recursive lambda

        int maxCount = 5;
        int count = 0;
        std::function<void( int& )> lambda;
        lambda = [&lambda, &maxCount]( int& count )
        {
            count++;
            if( count >= maxCount )
                return;

            std::cout << "Hello from recursive lambda (" << count << ")..." << std::endl;
            lambda( count );
        };

        lambda( count );
    }

    { // Template lambda (C++14)

        auto templateLambda = []( auto a, auto b )
        {
            return a * b;
        };

        std::cout << "Hello from template lambda: " << templateLambda( 4, 5 ) << std::endl;
        
        // Note the type automatic deduction for a, b and the return type
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
            std::cout << "a = " << a.getA() << ", b = " << a.getB() << std::endl;
        };

        lambda();

        // The mutable keyword is required as captured copy variables are const by default. Mutable allows you to call their const member functions.
    }

    std::cout << "\n\n";
    return 0;
}