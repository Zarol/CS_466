#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <list>
#include <map>
#include <thread>
#include <chrono>

#include "log.h"
#include "config.h"
#include "parser.h"
#include "timer.h"

class Application
{
public:
    Application( Config config, int appID, std::list<std::string> operations );

    void start();

    int ApplicationTime;
private:
    void runProcess( int cycleTime );
    void runInput( std::string name, int cycleTime );
    void runOutput( std::string name, int cycleTime );

    void calculateApplicationTime();
    int calculateOperationTime( std::map<std::string, std::string> operation );

    Config m_config;
    int m_appID;
    std::list<std::string> m_operations;

    friend bool operator<( const Application& app1, const Application& app2 );
    friend std::ostream& operator<<( std::ostream& os, const Application& app );
};

#endif  //  APPLICATION_H