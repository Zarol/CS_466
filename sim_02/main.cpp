#include <iostream>
#include <list>
#include <map>
#include "Escape_Sequences_Colors.h"

#include "timer.h"
#include "parser.h"
#include "config.h"
#include "simulator.h"

int main( int argc, char** argv )
{
    if( argc == 2 )
    {
        std::cout.precision( 6 );
        std::cout.setf( std::ios::fixed );

        Config config;
        std::string configFile = Parser::loadData( argv[1] );
        Parser::parseConfig( configFile, config );

        std::string metadata = Parser::loadData( config.filePath );
        std::list<std::string> operations = Parser::splitMetaData( metadata );
        
        std::cout << Timer::msDT() << " - Simulator " << COLOR_GREEN_BLACK 
            << "START" << COLOR_NORMAL << std::endl;
        Simulator sim( config, operations );
        sim.start();
        std::cout << Timer::msDT() << " - Simulator " << COLOR_RED_BLACK
            << "END" << COLOR_NORMAL << std::endl;
        (void) sim;
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "WARNING: Invalid number of arguments." << std::endl;
        return EXIT_FAILURE;
    }
}