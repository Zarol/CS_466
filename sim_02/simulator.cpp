#include "simulator.h"

Simulator::Simulator( Config config, std::list<std::string> operations )
    :   m_config( config )
{
    std::cout << Timer::msDT() << " - OS: START Process Preparation" 
        << std::endl;
    
    operations.pop_front();

    buildFIFO( operations );        

    if( m_config.schedulingCode == "SJF" )
        buildSJF( operations );

    std::cout << Timer::msDT() << " - OS: END Process Preparation" 
        << std::endl;
}

//
// PRIVATE HELPER FUNCTIONS ////////////////////////////////////////////////////
//

void Simulator::buildFIFO( std::list<std::string> operations )
{
    int appID = 0;

    std::list<std::string>::iterator operationsIter = operations.begin();
    while( !( operations.empty() ) )
    {
        std::map<std::string, std::string> operation = Parser::splitOperation(
            *operationsIter );
        if( operation["Component"] == "A" && operation["Operation"] == "start" )
        {
            appID++;
            operations.erase( operationsIter++ );

            std::list<std::string> applicationOperations;

            operation = Parser::splitOperation( *operationsIter );
            while( operation["Component"] != "A" && 
                   operation["Operation"] != "stop" )
            {
                applicationOperations.push_back( *operationsIter );
                operations.erase( operationsIter++ );
                operation = Parser::splitOperation( *operationsIter );
            }

            Application app( m_config, appID, applicationOperations );
            m_applications.push_back( app );
        }
        operations.erase( operationsIter++ );
    }
}

void Simulator::buildSJF( std::list<std::string> operations )
{
    m_applications.sort();
}

void Simulator::printApplications()
{
    for( std::list<Application>::iterator appIter = m_applications.begin();
         appIter != m_applications.end(); ++appIter )
    {
        std::cout << *appIter << std::endl;
    }
}