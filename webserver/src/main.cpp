#include <string>
#include <boost/algorithm/string.hpp>
#include "./include/Console.h"

/**
 * main
 *
 * Initialises the server
 * 
 * Coded by Kuihong Chen
 */
enum {JSON_STORE, DB_STORE};

int main(void) {
    Console::printInitMessage();
    
    // need to add inital sever

    string op = Console::readLine();
    boost::to_lower(op);

    if (op == 'a') {
        // need data class...
    }
        
        
}