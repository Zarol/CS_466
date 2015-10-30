#include <iostream>
#include <list>
#include <map>

#include "Tools/log.h"
#include "Tools/timer.h"
#include "Tools/parser.h"
#include "Tools/config.h"
#include "Simulation/simulator.h"

/**
 * @brief      Main program loop.
 *
 * @param[in]  argc  The number of command line arguments
 * @param      argv  The command line arguments, the second of which should be
 * the .cnf file path.
 *
 * @return     EXIT_SUCESS if the program executed without error, EXIT_FAILURE
 * otherwise.
 */
int main( int argc, char** argv )
{
    // The arguments provided contain the program execution and config file
    if( argc == 2 )
    {
        //Load config into a string, parse, and put the data into a class
        Config config;
        std::string configFile = Parser::loadData( argv[1] );
        Parser::parseConfig( configFile, config );

        //Set the logger class' configuration
        logger.setConfig( config ); 

        //Parse every meta-data operation into a list
        std::string metadata = Parser::loadData( config.filePath );
        std::list<std::string> operations = Parser::splitMetaData( metadata );
        
        //Begin the simulator by giving it the requested configuration &
        //All the operations found in the file specified
        logger << Timer::msDT() << " - Simulator " << "START" << "\n";
        Simulator sim( config, operations );
        sim.start();
        logger << Timer::msDT() << " - Simulator " << "END" << "\n";
        
        //Close the file if opened by the logger
        logger.closeFile();
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "WARNING: Invalid number of arguments." << std::endl;
        std::cout << "Ex: ./sim02 path/to/file.cnf" << std::endl;
        return EXIT_FAILURE;
    }
}