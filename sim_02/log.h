#ifndef LOG_H
#define LOG_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include "Escape_Sequences_Colors.h"

#include "config.h"
#include "timer.h"

#define logger Log::getLog()

struct Log
{
public:
    static Log& getLog();

    static void setConfig( Config& config );

    static void closeFile();

    Log& operator<<( std::string val );
    Log& operator<<( float val );
    Log& operator<<( int val );

private:
    static Config m_config;
    static std::ofstream m_file;
    
    Log()
    {
    };
};

#endif  //  LOG_H