#include "operation.h"

/**
 * @brief      Default constructor for an operation
 *
 * @param[in]  config     The config for this simulation.
 * @param[in]  appID      The appID this operation belongs to.
 * @param[in]  component  The component letter "I" / "O" / "P".
 * @param[in]  name       The name of the operation "hard drive", "keyboard",
 *     "monitor", and "printer".
 * @param[in]  cycles     The cycles this operation will run for.
 */
Operation::Operation( Config config, int appID, std::string component, 
    std::string name, int cycles)
    :   Component( component ), Name( name ), RemainingCycles( cycles ),
        m_config( config ), m_appID( appID )
{
}

/**
 * @brief      Execute this operation.
 *
 * @param[in]  cycles  The amount of cycles to execute for.
 *
 * @return     The remaining number of cycles if any.
 */
int Operation::execute( int cycles )
{
    if( Component == "I" || Component == "O" )
    {
        std::thread ioThread( &Operation::runIO, this, std::ref(cycles) );
        ioThread.join();
    }
    else if( Component == "P" )
    {
        logger << Timer::msDT() << " - Process " << m_appID << ": " << "START" 
            << " processing action\n";

        std::this_thread::sleep_for( std::chrono::milliseconds( 
            calculateOperationTime( cycles ) ) );

        logger << Timer::msDT() << " - Process " << m_appID << ": " << "END" 
            << " processing action\n";
    }
    else
    {
        logger << "END" << " - ERROR COMPONENT NAME\n";
    }

    return cycles;
}

int Operation::getRemainingTime() const
{
    int cycleTime = 0;
    // Select the process' cycle time
    if( Component == "P" )
    {
        cycleTime = m_config.processorCycle;
    }
    // Select the input's / output's cycle time
    else if( Component == "I" || Component == "O")
    {
        if( Name == "hard drive" )
            cycleTime = m_config.hardDriveCycle;
        else if ( Name == "keyboard" )
            cycleTime = m_config.keyboardCycle;
        else if ( Name == "monitor" )
            cycleTime = m_config.monitorDisplayCycle;
        else if ( Name == "printer" )
            cycleTime = m_config.printerCycle;
    }

    return RemainingCycles * cycleTime;
}

/**
 * @brief      Executes an input / output operation "I" / "O" with a defined
 * cycle amount.
 *
 * @param[in]  cycles  The number of cycles to wait for.
 */
void Operation::runIO( int& cycles )
{
    if( Component == "I" )
    {
        logger << Timer::msDT() << " - Process " << m_appID << ": " << "START" 
            << " " << Name << " input\n";

        std::this_thread::sleep_for( std::chrono::milliseconds( 
            calculateOperationTime( cycles ) ) );

        logger << Timer::msDT() << " - Process " << m_appID << ": " << "END" 
            << " " << Name << " input\n";
    }
    else if( Component == "O" )
    {
        logger << Timer::msDT() << " - Process " << m_appID << ": " << "START" 
            << " " << Name << " output\n";

        std::this_thread::sleep_for( std::chrono::milliseconds( 
            calculateOperationTime( cycles ) ) );

        logger << Timer::msDT() << " - Process " << m_appID << ": " << "END" 
            << " " << Name << " output\n";
    }
}

/**
 * @brief      Helper function to determine an operation's running time.
 *
 * @param[in]  cycles  The cycles to run for.
 * @param[out] cycles  The remaining cycles if not consumed.
 *
 * @return     The time this operation will take in milliseconds.
 */
int Operation::calculateOperationTime( int& cycles )
{
    int cycleTime = 0;
    // Select the process' cycle time
    if( Component == "P" )
    {
        cycleTime = m_config.processorCycle;
    }
    // Select the input's / output's cycle time
    else if( Component == "I" || Component == "O")
    {
        if( Name == "hard drive" )
            cycleTime = m_config.hardDriveCycle;
        else if ( Name == "keyboard" )
            cycleTime = m_config.keyboardCycle;
        else if ( Name == "monitor" )
            cycleTime = m_config.monitorDisplayCycle;
        else if ( Name == "printer" )
            cycleTime = m_config.printerCycle;
    }

    // Update Remaining Time
    if( RemainingCycles - cycles > 0 )
    {
        int runningTime = cycles * cycleTime;
        RemainingCycles -= cycles;
        cycles = 0;
        return runningTime;
    } 
    // Some cycles have not been consumed
    else 
    {
        int runningTime = RemainingCycles * cycleTime;
        cycles -= RemainingCycles;
        RemainingCycles = 0;
        return runningTime;
    }
}