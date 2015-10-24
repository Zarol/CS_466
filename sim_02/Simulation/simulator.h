#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <list>
#include <map>
#include <algorithm>

#include "log.h"
#include "config.h"
#include "parser.h"
#include "timer.h"
#include "application.h"

class Simulator
{
public:
    Simulator( Config config, std::list<std::string> operations );
    void start();

private:
    void buildFIFO( std::list<std::string> operations );
    void buildSJF( std::list<std::string> operations );
    void printApplications();

    Config m_config;
    std::list<Application> m_applications;
};

#endif  //  SIMULATOR_H