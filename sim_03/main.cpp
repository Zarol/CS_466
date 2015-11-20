/** @file main.cpp
 * @mainpage Operating Systems Simulation - Phase 2
 * @author   REDACTED
 * @version  2.00
 * @brief    This program simulates an Operating System's Scheduler and features
 * the First In First Out (FIFO), Shortest Job First (SJF), and Shortest
 * Remaining Time First Non-Preemptive (SRTF-N) scheduling codes.
 * 
 * @details  The overlying logic for this program is as follows: The simulator
 * will create applications that store their own operations. Each application
 * will be responsible for running their own operations. The simulator will then
 * select / arrange the next application to start. The simulator will execute
 * applications until there are no more remaining applications, each application
 * will execute every operation until there are no more remaining operations.
 */

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