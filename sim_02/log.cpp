#include "log.h"

Config Log::m_config;
std::ofstream Log::m_file;

Log& Log::getLog()
{
    static Log log;
    return log;
}

void Log::setConfig( Config& config )
{
    Log::m_config = config;

    if( Log::m_config.log == "Log to Both" || 
        Log::m_config.log == "Log to File")
    {
        Log::m_file.open( Log::m_config.logFilePath );            
    }
}

void Log::closeFile()
{
    if( m_file.is_open() )
            m_file.close();   
}

Log& Log::operator<<( std::string val )
{
    if( m_config.log == "Log to Both" )
    {
         if( val == "START" )
            std::cout << COLOR_GREEN_BLACK << val << COLOR_NORMAL;
         else if( val == "SELECTING" )
             std::cout << COLOR_YELLOW_BLACK << val << COLOR_NORMAL;
        else if( val == "END" )
            std::cout << COLOR_RED_BLACK << val << COLOR_NORMAL;
        else
            std::cout << val;
        if( m_file.is_open() )
            m_file << val;
    }
    else if( m_config.log == "Log to Monitor" )
    {
           if( val == "START" )
            std::cout << COLOR_GREEN_BLACK << val << COLOR_NORMAL;
        else if( val == "SELECTING" )
            std::cout << COLOR_YELLOW_BLACK << val << COLOR_NORMAL;
        else if( val == "END" )
            std::cout << COLOR_RED_BLACK << val << COLOR_NORMAL;
        else
            std::cout << val;
    }
    else if( m_config.log == "Log to File" )
    {
        if( m_file.is_open() )
            m_file << val;
    }
    return *this;
}

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