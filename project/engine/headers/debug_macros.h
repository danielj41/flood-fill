#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

/**
 This library defines macros to debug your program.

 In order to use those macros, make sure that you are including:

 #include <iostream>
 #include <cstdlib>
*/


/* In the release version define NDEBUG macro */
// #define NDEBUG


/* INFO macro prints a message in the screen using the std iostream */

#ifndef NDEBUG
#   define INFO(args...) \
        std::cout << args << std::endl;
#else
#   define INFO(args...)
#endif

/**
 DEBUG macro prints a message in the screen using the std error output
 If you already have a DEBUG macro defined this macro does not have any
 effect.
*/

#ifndef DEBUG
    #ifndef NDEBUG
    #   define DEBUG(args...) \
            std::cerr << args << std::endl;
    #else
    #   define DEBUG(args...)
    #endif
#endif

/*Source:
    http://stackoverflow.com/questions/3767869/adding-message-to-assert*/

#ifndef NDEBUG
#   define ASSERT(condition, message...) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message)
#endif

#endif
