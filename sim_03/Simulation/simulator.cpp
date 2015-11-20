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
    // Executes the proper scheduling algorithm
    if( m_config.schedulingCode == "RR" )
        RoundRobin();
    else if( m_config.schedulingCode == "FIFO-P" )
        FirstInFirstOutPreEmption();
    else if( m_config.schedulingCode == "SRTF-P" )
        ShortestRemainingTimeFirstPreEmption();
    else
        logger << "END" << " - ERROR SCHEDULING CODE\n";
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
        std::map<std::string, std::string> opMap = Parser::splitOperation(
            *operationsIter );

        Operation operation( m_config, appID, opMap["Component"], 
            opMap["Operation"], std::stoi( opMap["Cycle"] ) );

        // If the operation is an Application, extract the operations and give it to
        // the operation until that application's section is over
        if( operation.Component == "A" && operation.Name == "start" )
        {
            appID++;
            // Removes "Application Start"
            operations.erase( operationsIter++ );

            // Create a temp list for all of the app's operations
            std::list<Operation> applicationOperations;

            opMap = Parser::splitOperation( *operationsIter );
            operation = Operation( m_config, appID, opMap["Component"], 
                opMap["Operation"], std::stoi( opMap["Cycle"] ) );
            // Add each operation until the app's section ends
            while( operation.Component != "A" && operation.Name != "stop" )
            {
                applicationOperations.push_back( operation );
                operations.erase( operationsIter++ );

                opMap = Parser::splitOperation( *operationsIter );
                operation = Operation( m_config, appID, opMap["Component"], 
                    opMap["Operation"], std::stoi( opMap["Cycle"] ) );
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

void Simulator::RoundRobin()
{
    // Iterate through all of the applications within the simulation
    std::list<Application>::iterator appIterator = m_applications.begin();
    while( !( m_applications.empty() ) )
    {
        logger << Timer::msDT() << " - OS: " << "SELECTING" 
            << " next process\n";
        while( appIterator == m_applications.end() || 
               appIterator->Blocked == true )
        {
            if( appIterator == m_applications.end() )
                appIterator = m_applications.begin();
            else
                appIterator++;
        }

        appIterator->start();
        if( appIterator->ApplicationTime == 0 )
            m_applications.erase( appIterator++ );
        else
            appIterator++;
    }
}

void Simulator::FirstInFirstOutPreEmption()
{

}

void Simulator::ShortestRemainingTimeFirstPreEmption()
{

}