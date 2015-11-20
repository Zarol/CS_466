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
    std::list<std::string> operations )
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

    // Executes each operation within this Application
    std::list<std::string>::iterator iter = m_operations.begin();
    while( !( m_operations.empty() ) )
    {
        // Splits the operation into it's component pieces
        std::map<std::string, std::string> operation = Parser::splitOperation( 
            *iter );
        int runningTime = calculateOperationTime( operation );

        // Execute process
        if( operation["Component"] == "P" )
        {
            runProcess( runningTime );
        }
        // Execute input within it's own thread
        else if( operation["Component"] == "I" )
        {
            std::thread inputThread( &Application::runInput, this, 
                operation["Operation"], runningTime );
            inputThread.join();
        }
        // Execute output within it's own thread
        else if( operation["Component"] == "O" )
        {
            std::thread outputThread( &Application::runOutput, this, 
                operation["Operation"], runningTime );
            outputThread.join();
        }
        
        // The operation has been executed, so remove it from the list
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
 * @param[in]  runningTime  The time in miliseconds to simulate an operation by
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
 * @param[in]  runningTime  The time in miliseconds to simulate an operation by
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
 * @param[in]  runningTime  The time in miliseconds to simulate an operation by
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

/**
 * @brief      Sets this application's ApplicationTime to the total time of all
 * operations within this Application.
 */
void Application::calculateApplicationTime()
{
    ApplicationTime = 0;
    // Iterates through the list of operations, adds each operation's
    // running time to the total running time
    for( std::list<std::string>::const_iterator iter = m_operations.begin();
         iter != m_operations.end(); ++iter )
    {
        std::map<std::string, std::string> operation = Parser::splitOperation( 
            *iter );
        ApplicationTime += calculateOperationTime( operation );
    }
}

/**
 * @brief      Helper function to determine an operation's running time.
 *
 * @param[in]  operation  The operation to calculate the running time for.
 *
 * @return     The time this operation will take in milliseconds.
 */
int Application::calculateOperationTime( 
    std::map<std::string, std::string> operation )
{
    // Parse the cycle time to an int
    int cycleTime = std::stoi( operation["Cycle"] );

    // Return a process' cycle time
    if( operation["Component"] == "P" )
    {
        return cycleTime * m_config.processorCycle;
    }
    // Return an input's / output's cycle time
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

/**
 * @brief      Prints out this Application's ID and operations.
 *
 * @param      os    The ostream to print to.
 * @param[in]  app   The application to print out.
 *
 * @return     The ostream with the outputted values.
 */
std::ostream& operator<<( std::ostream& os, const Application& app )
{
    os << "[Application " << app.m_appID << "]" << std::endl;
    // Prints out all the operations on a newline within the application
    for( std::list<std::string>::const_iterator iter = app.m_operations.begin();
         iter != app.m_operations.end(); ++iter )
    {
        os << *iter << std::endl;
    }
    os << "[Application " << app.m_appID << "]" << std::endl;
    return os;
}