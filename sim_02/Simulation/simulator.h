#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <list>
#include <map>
#include <algorithm>

#include "Tools/log.h"
#include "Tools/config.h"
#include "Tools/parser.h"
#include "Tools/timer.h"

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