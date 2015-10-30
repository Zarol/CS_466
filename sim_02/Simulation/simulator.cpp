#include "simulator.h"

/**
 * @brief      Default constructor to build a simulator.
 *
 * @param[in]  config      The configuration associated with this Simulator.
 * @param[in]  operations  The full list of oeprations to simulate.
 */
Simulator::Simulator( Config config, std::list<std::string> operations )
    :   m_config( config )
{
    logger << Timer::msDT() << " - OS: " << "START" << " process preparation\n";
    
    // Pop the first operation which is "Simulator Start"
    operations.pop_front();

    // Initially build FIFO regardless of scheduling code
    buildFIFO( operations );        

    // Builds SJF / SRTF-N by sorting the FIFO build
    if( m_config.schedulingCode == "SJF" || 
        m_config.schedulingCode == "SRTF-N" )
    {
        buildSJF();
    }
    
    logger << Timer::msDT() << " - OS: " << "END" << " process preparation\n";
}

//
// MAIN SIMULATOR LOOP /////////////////////////////////////////////////////////
//

/**
 * @brief      Starts the simulation by executing each application with the
 * config's scheduling code.
 */
void Simulator::start()
{
    // Iterate through all of the applications within the simulation
    std::list<Application>::iterator appIterator = m_applications.begin();
    while( !( m_applications.empty() ) )
    {
        logger << Timer::msDT() << " - OS: " << "SELECTING" 
            << " next process\n";

        // Execute the applications in its entirety
        (*appIterator).start();
        // Remove the application once it has been executed
        m_applications.erase( appIterator++ );
        
        // Re-Sort the applications to pick the next shortest one
        if( m_config.schedulingCode == "SRTF-N" )
            buildSJF();
    }
}

//
// SCHEDULING CODES ////////////////////////////////////////////////////////////
//

/**
 * @brief      Builds the First In First Out scheduling code.
 * @details    This function will create applications and give every application
 * their respective operations between A Start and A End. Any operation not
 * between "A" are discarded.
 *
 * @param[in]  operations  The full list of operations to build from.
 */
void Simulator::buildFIFO( std::list<std::string> operations )
{
    // Give each application a unique ID
    int appID = 0;

    // Builds the simulator's operations by creating applications
    // that contains their own operations
    std::list<std::string>::iterator operationsIter = operations.begin();
    while( !( operations.empty() ) )
    {
        std::map<std::string, std::string> operation = Parser::splitOperation(
            *operationsIter );
        // If the operation is an Application, extract the operations and give it to
        // the operation until that application's section is over
        if( operation["Component"] == "A" && operation["Operation"] == "start" )
        {
            appID++;
            // Removes "Application Start"
            operations.erase( operationsIter++ );

            // Create a temp list for all of the app's operations
            std::list<std::string> applicationOperations;

            operation = Parser::splitOperation( *operationsIter );
            // Add each operation until the app's section ends
            while( operation["Component"] != "A" && 
                   operation["Operation"] != "stop" )
            {
                applicationOperations.push_back( *operationsIter );
                operations.erase( operationsIter++ );
                operation = Parser::splitOperation( *operationsIter );
            }

            // Create the new application with the extracted operations
            Application app( m_config, appID, applicationOperations );
            // Give it to the simulator's application list
            m_applications.push_back( app );
        }
        // Removes "Simulation End" or any operations that do not belong
        // to any application 
        operations.erase( operationsIter++ );
    }
}

/**
 * @brief      Builds the Shortest Job First scheduling code.
 * @note       Currently also used to perform SRTF-N since all it does is
 * select the next shortest application.
 */
void Simulator::buildSJF( )
{
    m_applications.sort();
}

//
// HELPER FUNCTIONS ////////////////////////////////////////////////////////////
//

/**
 * @brief      Prints out all applications within this simulator.
 */
void Simulator::printApplications()
{
    // Iterates through the simulator's applications, prints each one
    for( std::list<Application>::iterator appIter = m_applications.begin();
         appIter != m_applications.end(); ++appIter )
    {
        std::cout << *appIter << std::endl;
    }
}