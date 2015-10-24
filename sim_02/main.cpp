#include <iostream>
#include <list>
#include <map>

#include "Tools/log.h"
#include "Tools/timer.h"
#include "Tools/parser.h"
#include "Tools/config.h"
#include "Simulation/simulator.h"

int main( int argc, char** argv )
{
    if( argc == 2 )
    {
        Config config;
        std::string configFile = Parser::loadData( argv[1] );
        Parser::parseConfig( configFile, config );
        logger.setConfig( config ); 

        std::string metadata = Parser::loadData( config.filePath );
        std::list<std::string> operations = Parser::splitMetaData( metadata );
        
        logger << Timer::msDT() << " - Simulator " << "START" << "\n";
        Simulator sim( config, operations );
        sim.start();
        logger << Timer::msDT() << " - Simulator " << "END" << "\n";
        
        logger.closeFile();
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "WARNING: Invalid number of arguments." << std::endl;
        return EXIT_FAILURE;
    }
}