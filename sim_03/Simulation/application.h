#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <list>
#include <map>
#include <chrono>

#include "Tools/log.h"
#include "Tools/config.h"
#include "Tools/parser.h"
#include "Tools/timer.h"

#include "operation.h"

class Application
{
public:
    Application( Config config, int appID, std::list<Operation> operations );

    void start();

    int ApplicationTime;
    bool Blocked = false;
private:
    void calculateApplicationTime();

    Config m_config;
    int m_appID;
    std::list<Operation> m_operations;

    friend bool operator<( const Application& app1, const Application& app2 );
};

#endif  //  APPLICATION_H