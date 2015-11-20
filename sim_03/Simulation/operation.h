#ifndef OPERATION_H
#define OPERATION_H

#include <iostream>
#include <thread>

#include "Tools/config.h"
#include "Tools/log.h"
#include "Tools/timer.h"

class Operation
{
public:
    Operation( Config config, int appID, std::string component, 
        std::string name, int cycles);
    int execute( int cycles );
    int getRemainingTime() const;

    std::string Component;
    std::string Name;
    int RemainingCycles;
private:
    void runIO( int& cycles );
    int calculateOperationTime( int& cycles );

    Config m_config;
    int m_appID;
};

#endif  //  OPERATION_H