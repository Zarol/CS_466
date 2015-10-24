#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <algorithm>

#include "config.h"

namespace Parser
{
    std::string loadData( std::string path );
    void parseConfig( std::string data, Config& config );
    std::list<std::string> splitMetaData( std::string metadata );
    std::map<std::string, std::string> splitOperation( std::string operation );
}

#endif  //  PARSER_H