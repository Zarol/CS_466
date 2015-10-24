#include "application.h"

Application::Application( Config config, int appID, 
    std::list<std::string> operations )
    :   m_config( config ), m_appID( appID ), m_operations( operations )
{
    calculateApplicationTime();
}

/**
 * @brief      Executes each process "P", input "I", and output "O" linearly.
 * @details    Each I/O operation executes within its own thread.
 */
void Application::start()
{
    logger << Timer::msDT() << " - OS: " << "START" << " process "
        << m_appID << "\n";

    std::list<std::string>::iterator iter = m_operations.begin();
    while( !( m_operations.empty() ) )
    {
        std::map<std::string, std::string> operation = Parser::splitOperation( 
            *iter );
        int runningTime = calculateOperationTime( operation );

        if( operation["Component"] == "P" )
        {
            runProcess( runningTime );
        }
        else if( operation["Component"] == "I" )
        {
            std::thread inputThread( &Application::runInput, this, 
                operation["Operation"], runningTime );
            inputThread.join();
        }
        else if( operation["Component"] == "O" )
        {
            std::thread outputThread( &Application::runOutput, this, 
                operation["Operation"], runningTime );
            outputThread.join();
        }
        
        m_operations.erase( iter++ );
    }

    logger << Timer::msDT() << " - OS: " << "END" << " process "
        << m_appID << "\n";
}

//
// I/O/P ACTIONS ///////////////////////////////////////////////////////////////
//

/**
 * @brief      Executes a process "P" with a defined wait time.
 *
 * @param[in]  cycleTime  The time in miliseconds to simulate an operation by
 * sleeping the thread.
 */
void Application::runProcess( int runningTime )
{
    logger << Timer::msDT() << " - Process " << m_appID << ": " << "START" 
        << " processing action\n";

    std::this_thread::sleep_for( std::chrono::milliseconds( runningTime ) );

    logger << Timer::msDT() << " - Process " << m_appID << ": " << "END" 
        << " processing action\n";
}

/**
 * @brief      Executes an input operation "I" with a defined wait time and a
 * defined operation name.
 *
 * @note       This function is currently very similar to runOutput, but is kept
 * separately in case future iterations of the simulation causes them to change
 * more distinctly.
 * 
 * @param[in]  name       The name of the operation. It may be "hard drive",
 * "keyboard", "monitor", or "printer".
 * @param[in]  cycleTime  The time in miliseconds to simulate an operation by
 * sleeping the thread.
 */
void Application::runInput( std::string name, int runningTime )
{
    logger << Timer::msDT() << " - Process " << m_appID << ": " << "START" 
        << " " << name << " input\n";

    std::this_thread::sleep_for( std::chrono::milliseconds( runningTime ) );

    logger << Timer::msDT() << " - Process " << m_appID << ": " << "END" 
        << " " << name << " input\n";
}

/**
 * @brief      Executes an output operation "O" with a defined wait time and a
 * defined operation name.
 * 
 * @note       This function is currently very similar to runInput, but is kept
 * separately in case future iterations of the simulation causes them to change
 * more distinctly.
 *
 * @param[in]  name       The name of the operation. It may be "hard drive",
 * "keyboard", "monitor", or "printer".
 * @param[in]  cycleTime  The time in miliseconds to simulate an operation by
 * sleeping the thread.
 */
void Application::runOutput( std::string name, int runningTime )
{
    logger << Timer::msDT() << " - Process " << m_appID << ": " << "START" 
        << " " << name << " output\n";

    std::this_thread::sleep_for( std::chrono::milliseconds( runningTime ) );

    logger << Timer::msDT() << " - Process " << m_appID << ": " << "END" 
        << " " << name << " output\n";
}

//
// HELPER FUNCTIONS ////////////////////////////////////////////////////////////
//


void Application::calculateApplicationTime()
{
    ApplicationTime = 0;
    for( std::list<std::string>::const_iterator iter = m_operations.begin();
         iter != m_operations.end(); ++iter )
    {
        std::map<std::string, std::string> operation = Parser::splitOperation( 
            *iter );
        ApplicationTime += calculateOperationTime( operation );
    }
}

int Application::calculateOperationTime( 
    std::map<std::string, std::string> operation )
{
    int cycleTime = std::stoi( operation["Cycle"] );

    if( operation["Component"] == "P" )
    {
        return cycleTime * m_config.processorCycle;
    }
    else if( operation["Component"] == "I" ||
             operation["Component"] == "O")
    {
        if( operation["Operation"] == "hard drive" )
        {
            return cycleTime * m_config.hardDriveCycle;
        }
        else if ( operation["Operation"] == "keyboard" )
        {
            return cycleTime * m_config.keyboardCycle;
        }
        else if ( operation["Operation"] == "monitor" )
        {
            return cycleTime * m_config.monitorDisplayCycle;
        }
        else if ( operation["Operation"] == "printer" )
        {
            return cycleTime * m_config.printerCycle;
        }
    }
        
    return 0;
}

//
// OPERATOR OVERLOADS //////////////////////////////////////////////////////////
//

bool operator<( const Application& app1, const Application& app2 )
{
    return app1.ApplicationTime < app2.ApplicationTime;
}

std::ostream& operator<<( std::ostream& os, const Application& app )
{
    os << "[Application " << app.m_appID << "]" << std::endl;
    for( std::list<std::string>::const_iterator iter = app.m_operations.begin();
         iter != app.m_operations.end(); ++iter )
    {
        os << *iter << std::endl;
    }
    os << "[Application " << app.m_appID << "]" << std::endl;
    return os;
}