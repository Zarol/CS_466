#include "log.h"

//Static configs for output file
Config Log::m_config;
std::ofstream Log::m_file;

//
// STATIC FUNCTIONS ////////////////////////////////////////////////////////////
//

/**
 * @brief      Singleton Log.
 *
 * @return     Returns the Log instance.
 */
Log& Log::getLog()
{
    static Log log;
    return log;
}

/**
 * @brief      Initializes the logger with a config file.
 * @details    This function will open a fill defined by the config if the
 * config has been set to "Log to Both" or "Log to File".
 *
 * @param      config  The configuration to setup the logger with.
 */
void Log::setConfig( Config& config )
{
    Log::m_config = config;

    if( Log::m_config.log == "Log to Both" || 
        Log::m_config.log == "Log to File")
    {
        Log::m_file.open( Log::m_config.logFilePath );            
    }
}

/**
 * @brief      Close the file currently opened by the logger.
 */
void Log::closeFile()
{
    if( m_file.is_open() )
            m_file.close();   
}

//
// OPERATOR OVERLOADS //////////////////////////////////////////////////////////
//

/**
 * @brief     Outputs strings to the selected logger configuration.
 * @details   Passing "START", "SELECTING", or "END" will color the strings to
 * green, yellow, and red respectively to the console.
 * 
 * @param[in] val  The value of the string to log.
 * @return    A reference to this Log so that outputs may be concatenated.
 */
Log& Log::operator<<( std::string val )
{
    //Log to both console and file
    if( m_config.log == "Log to Both" )
    {
        //Set output colors for specific keywords
        if( val == "START" )
            std::cout << COLOR_GREEN_BLACK << val << COLOR_NORMAL;
        else if( val == "SELECTING" )
             std::cout << COLOR_YELLOW_BLACK << val << COLOR_NORMAL;
        else if( val == "END" )
            std::cout << COLOR_RED_BLACK << val << COLOR_NORMAL;
        //Write all other strings to cout
        else
            std::cout << val;
        //Write all strings to the file (no color)
        if( m_file.is_open() )
            m_file << val;
    }
    //Log to only console
    else if( m_config.log == "Log to Monitor" )
    {
        //Set output colors for specific keywords
        if( val == "START" )
            std::cout << COLOR_GREEN_BLACK << val << COLOR_NORMAL;
        else if( val == "SELECTING" )
            std::cout << COLOR_YELLOW_BLACK << val << COLOR_NORMAL;
        else if( val == "END" )
            std::cout << COLOR_RED_BLACK << val << COLOR_NORMAL;
        //Non-specific keyword, just write
        else
            std::cout << val;
    }
    //Log to only file
    else if( m_config.log == "Log to File" )
    {
        if( m_file.is_open() )
            m_file << val;
    }
    return *this;
}

/**
 * @brief     Outputs floats to the selected logger configuration.
 * @details   This output will format the float to a precision of 6.
 * 
 * @param[in] val  The value of the float to log.
 * @return    A reference to this Log so that outputs may be concatenated.
 */
Log& Log::operator<<( float val )
{
    if( m_config.log == "Log to Both" )
    {
        std::cout << std::fixed << std::setprecision( 6 ) << val;
        if( m_file.is_open() )
            m_file << std::fixed << std::setprecision( 6 ) << val;
    }
    else if( m_config.log == "Log to Monitor" )
    {
        std::cout << std::fixed << std::setprecision( 6 ) << val;
    }
    else if( m_config.log == "Log to File" )
    {
        if( m_file.is_open() )
            m_file << std::fixed << std::setprecision( 6 ) << val;
    }
    return *this;
}

/**
 * @brief     Outputs ints to the selected logger configuration.
 * 
 * @param[in] val  The value of the int to log.
 * @return    A reference to this Log so that outputs may be concatenated.
 */
Log& Log::operator<<( int val )
{
    if( m_config.log == "Log to Both" )
    {
        std::cout << val;
        if( m_file.is_open() )
            m_file << val;
    }
    else if( m_config.log == "Log to Monitor" )
    {
        std::cout << val;
    }
    else if( m_config.log == "Log to File" )
    {
        if( m_file.is_open() )
            m_file << val;
    }
    return *this;
}