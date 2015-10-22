#include <iostream>
#include <list>
#include <map>

#include "timer.h"
#include "parser.h"
#include "config.h"
#include "application.h"

int main( int argc, char** argv )
{
    if( argc == 2 )
    {
        Config config;
        std::string configFile = Parser::loadData( argv[1] );
        Parser::parseConfig( configFile, config );

        std::string metadata = Parser::loadData( config.filePath );
        std::list<std::string> operations = Parser::splitMetaData( metadata );
        Application myApp = Application( config, 1, operations );
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "WARNING: Invalid number of arguments." << std::endl;
        return EXIT_FAILURE;
    }
}