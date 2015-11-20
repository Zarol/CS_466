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
#include "operation.h"

class Simulator
{
public:
    Simulator( Config config, std::list<std::string> operations );
    void start();

private:
    void buildFIFO( std::list<std::string> operations );
    void RoundRobin();
    void FirstInFirstOutPreEmption();
    void ShortestRemainingTimeFirstPreEmption();

    Config m_config;
    std::list<Application> m_applications;
};

#endif  //  SIMULATOR_H