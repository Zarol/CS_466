#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstring>

std::string loadData( std::string );
void parseConfig( std::string );
std::vector<std::string> splitOperations( std::string );
std::vector<std::string> parseOperation( std::string );
void runProcess( int, int );
void inputProcess( std::string, int, int );
void outputProcess( std::string, int, int );
void applicationLoop( std::vector<std::string>, int );
void simulatorLoop( std::vector<std::string> );
void printConfig();

std::clock_t startTime;
float duration;

struct Config
{
    std::string version;
    std::string filePath;
    int processorCycle;
    int monitorDisplayCycle;
    int hardDriveCycle;
    int printerCycle;
    int keyboardCycle;
    std::string log;
    std::string logFilePath;
} config;

int main( int argc, char** argv )
{    
    startTime = std::clock();

    std::cout.precision( 6 );
    std::cout.setf( std::ios::fixed );
    if( argc == 2 )
    {
        std::string configFile = loadData ( argv[1] );
        parseConfig( configFile );
        std::string data = loadData ( config.filePath );
        std::vector<std::string> operations = splitOperations( data );
        std::vector<std::string> firstOperation = 
            parseOperation( operations[2] );
        
        simulatorLoop( operations );

        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "WARNING: Invalid number of arguments." << std::endl;
        return EXIT_FAILURE;
    }
}

std::string loadData( std::string path )
{
    std::string temp;
    std::ifstream read;

    read.open( path.c_str() );
    if( read.is_open() )
    {
        while( !read.eof() )
            temp += read.get();
    }
    else
    {
        std::cout << "WARNING: File could not be opened." << std::endl;
        exit( EXIT_FAILURE );
    }
    read.close();
    return temp;
}

void parseConfig( std::string data )
{
    size_t frontPos = 0; 
    size_t backPos = 0;

    //Remove useless first line
    data.erase( 0, data.find( '\n' ) + 1 );
    //Version
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.version = data.substr( frontPos, backPos - frontPos );
    data.erase( 0, backPos + 1 );
    //File Path
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.filePath = data.substr( frontPos, backPos - frontPos );
    data.erase( 0, backPos + 1 );
    //Processor Cycle Time
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.processorCycle = std::stoi(
        data.substr( frontPos, backPos - frontPos ));
    data.erase( 0, backPos + 1 );
    //Monitor Display Time
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.monitorDisplayCycle = std::stoi(
        data.substr( frontPos, backPos - frontPos ));
    data.erase( 0, backPos + 1 );
    //Hard Drive Cycle Time
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.hardDriveCycle = std::stoi(
        data.substr( frontPos, backPos - frontPos ));
    data.erase( 0, backPos + 1 );
    //Printer Cycle Time
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.printerCycle = std::stoi(
        data.substr( frontPos, backPos - frontPos ));
    data.erase( 0, backPos + 1 );
    //Keyboard Cycle Time
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.keyboardCycle = std::stoi(
        data.substr( frontPos, backPos - frontPos ));
    data.erase( 0, backPos + 1 );
    //Log
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.log = data.substr( frontPos, backPos - frontPos );
    data.erase( 0, backPos + 1 );
    //Log File Path
    frontPos = data.find( ": " ) + 2;
    backPos = data.find( '\n' );
    config.logFilePath = data.substr( frontPos, backPos - frontPos );
    data.erase( 0, backPos + 1 );

}

std::vector<std::string> splitOperations( std::string data )
{
    std::vector<std::string> temp;
    std::string delimiter = "; ";
    
    size_t pos = 0;
    std::string token;

    // Remove the first line (contains "Start Program Meta-Data Code:")
    data.erase( 0, data.find( '\n' ) + 1 );
    // Remove new line characters
    data.erase( 
        std::remove( data.begin(), data.end(), '\n' ), data.end() );

    // Grab all the operations ending with ";"
    while( ( pos = data.find( delimiter ) ) != std::string::npos )
    {
        token = data.substr( 0, pos );
        temp.push_back( token );
        data.erase( 0, pos + delimiter.length() );
    }
    // Grab the last operation that ends with "."
    pos = data.find( "." );
    token = data.substr( 0, pos );
    temp.push_back( token );

    return temp;
}

std::vector<std::string> parseOperation( std::string data )
{
    std::vector<std::string> temp;
    std::string token;
    std::string delimiter = ")";
    
    size_t pos = 0;

    // <Component Letter>(<Operation>)<Cycle Time>
    // Get component letter
    pos = 1;
    token = data.substr( 0, pos );
    temp.push_back( token );
    data.erase( 0, pos + 1 );

    // Get operation
    pos = data.find( delimiter );
    token = data.substr( 0, pos );
    temp.push_back( token );
    data.erase( 0, pos + delimiter.length() );

    // Get cycle time
    temp.push_back( data );

    return temp;
}

void runProcess( int cycleTime, int appID )
{
    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - Process " << appID 
        << ": START processing action" << std::endl;
            
    int sleepTime = cycleTime * config.processorCycle;
    std::this_thread::sleep_for( std::chrono::milliseconds( sleepTime ) );

    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - Process " << appID 
        << ": END processing action" << std::endl;
}

void runInput( std::string name, int cycleTime, int appID )
{
    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - Process " << appID << ": START " << 
        name << " input" << std::endl;
            
    int sleepTime = 0;
    if( name == "hard drive" )
    {
        sleepTime = cycleTime * config.hardDriveCycle;
    }
    else if ( name == "keyboard" )
    {
        sleepTime = cycleTime * config.keyboardCycle;
    }
    else if ( name == "monitor" )
    {
        sleepTime = cycleTime * config.monitorDisplayCycle;
    }
    else if ( name == "printer" )
    {
        sleepTime = cycleTime * config.printerCycle;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( sleepTime ) );

    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - Process " << appID << ": END " << 
        name << " input" << std::endl;
}

void runOutput( std::string name, int cycleTime, int appID )
{
    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - Process " << appID << ": START " << 
        name << " output" << std::endl;
            
    int sleepTime = 0;
    if( name == "hard drive" )
    {
        sleepTime = cycleTime * config.hardDriveCycle;
    }
    else if ( name == "keyboard" )
    {
        sleepTime = cycleTime * config.keyboardCycle;
    }
    else if ( name == "monitor" )
    {
        sleepTime = cycleTime * config.monitorDisplayCycle;
    }
    else if ( name == "printer" )
    {
        sleepTime = cycleTime * config.printerCycle;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( sleepTime ) );

    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - Process " << appID << ": END " << 
        name << " output" << std::endl;
}

void applicationLoop( std::vector<std::string> operations, int appID )
{
    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - OS: START process " << appID << std::endl;

    for( unsigned int i = 0; i < operations.size(); i++ )
    {
        std::vector<std::string> parsedOp = parseOperation( operations[i] );
        int cycleTime = std::stoi( parsedOp[2] );
        if( parsedOp[0] == "P" )
        {
            std::thread processThread( runProcess, cycleTime, appID );
            processThread.join();
        }
        else if( parsedOp[0] == "I" )
        {
            std::thread inputThread( runInput, parsedOp[1], cycleTime, appID );
            inputThread.join();
        }
        else if( parsedOp[0] == "O" )
        {
            std::thread outputThread( runOutput, parsedOp[1], cycleTime, 
                appID );
            outputThread.join();
        }
    }

    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - END process " << appID << std::endl;
}

void simulatorLoop( std::vector<std::string> operations )
{
    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - <Simulator START>" << std::endl;

    //Reverse the vector so the operations may be popped in order
    std::reverse( operations.begin(), operations.end() );
    //Pop simulator start
    operations.pop_back();

    int appID = 0;

    while( operations.size() != 0 )
    {
        std::vector<std::string> parsedOp = parseOperation( operations.back() );

        if( parsedOp[0] == "A" && parsedOp[1] == "start" )
        {
            appID++;
            duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
            std::cout << duration << " - OS: PREPARING process " << appID 
                << std::endl;

            std::vector<std::string> applicationOperations;
            operations.pop_back();
            parsedOp = parseOperation( operations.back() );
            while( parsedOp[0] != "A" && parsedOp[1] != "end" )
            {
                applicationOperations.push_back( operations.back() );
                operations.pop_back();
                parsedOp = parseOperation( operations.back() );
            }

            applicationLoop( applicationOperations, appID );
        }
        operations.pop_back();
    }

    duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << " - <Simulator END>" << std::endl;
}

void printConfig()
{
    std::cout << config.version << std::endl;
    std::cout << config.filePath << std::endl;
    std::cout << config.processorCycle << std::endl;
    std::cout << config.monitorDisplayCycle << std::endl;
    std::cout << config.hardDriveCycle << std::endl;
    std::cout << config.printerCycle << std::endl;
    std::cout << config.keyboardCycle << std::endl;
    std::cout << config.log << std::endl;
    std::cout << config.logFilePath << std::endl;
}