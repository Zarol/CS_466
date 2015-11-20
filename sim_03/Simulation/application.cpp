#include "application.h"

//
// CONSTRUCTORS ////////////////////////////////////////////////////////////////
//

/**
 * @brief      Default constructor for an "A" operation.
 *
 * @param[in]  config      The config file associated with this application.
 * @param[in]  appID       The unique ID associated with this application.
 * @param[in]  operations  The list of operations associated with this 
 * application.
 */
Application::Application( Config config, int appID, 
    std::list<Operation> operations )
    :   m_config( config ), m_appID( appID ), m_operations( operations )
{
    calculateApplicationTime();
}

//
// MAIN APP LOOP ///////////////////////////////////////////////////////////////
//

/**
 * @brief      Executes each process "P", input "I", and output "O" linearly.
 * @details    Each I/O operation executes within its own thread.
 */
void Application::start()
{
    logger << Timer::msDT() << " - OS: " << "START" << " process "
        << m_appID << "\n";

    int remainingQuantumTime = m_config.quantumTime;
    // Executes each operation within this Application
    std::list<Operation>::iterator iter = m_operations.begin();
    while( remainingQuantumTime > 0 )
    {
        if( m_operations.empty() )
        {
            remainingQuantumTime = 0;
        }
        else
        {
            if( iter->Component == "I" || iter->Component == "O" )
            {
                Blocked = true;
                iter->execute( remainingQuantumTime );
                Blocked = false;
            } else {
                iter->execute( remainingQuantumTime );
            } 
            if( iter->RemainingCycles == 0 )
                m_operations.erase( iter++ );
        }

        calculateApplicationTime();
    }

    logger << Timer::msDT() << " - OS: " << "END" << " process "
        << m_appID << "\n";
}

//
// HELPER FUNCTIONS ////////////////////////////////////////////////////////////
//

/**
 * @brief      Sets this application's ApplicationTime to the total time of all
 * operations within this Application.
 */
void Application::calculateApplicationTime()
{
    ApplicationTime = 0;
    // Iterates through the list of operations, adds each operation's
    // running time to the total running time
    for( std::list<Operation>::const_iterator iter = m_operations.begin();
         iter != m_operations.end(); ++iter )
    {
        ApplicationTime += iter->getRemainingTime();
    }
}

//
// OPERATOR OVERLOADS //////////////////////////////////////////////////////////
//

/**
 * @brief      An Application is considered to be less than another Application
 * if its ApplicationTime is less than the other.
 *
 * @param[in]  app1  The first application.
 * @param[in]  app2  The second application.
 *
 * @return     True if the first application is less than the second
 * application, false otherwise.
 */
bool operator<( const Application& app1, const Application& app2 )
{
    return app1.ApplicationTime < app2.ApplicationTime;
}