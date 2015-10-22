#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
    std::string version;
    std::string filePath;
    int processorCycle;
    std::string schedulingCode;
    int monitorDisplayCycle;
    int hardDriveCycle;
    int printerCycle;
    int keyboardCycle;
    std::string log;
    std::string logFilePath;
};

#endif  //  CONFIG_H