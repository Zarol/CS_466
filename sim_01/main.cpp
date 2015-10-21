/** @file main.cpp
 * @mainpage   Operating Systems Simulation - Phase 1
 * @author     REDACTED
 * @version    1.00
 * @brief      This program simulates a very basic version of an Operating
 * System's scheduler.
 * @details    This program accepts the meta-data for a program with a
 * potentially unlimited number of meta-data operations, run it, and end the
 * simulation. Each unique I/O operation is conducted inside of it's own thread.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>

//
// FORWARD DECLARATIONS ////////////////////////////////////////////////////////
// 

std::string loadData( std::string );
void parseConfig( std::string );
std::vector<std::string> splitMetaData( std::string );
std::vector<std::string> splitOperation( std::string );
void simulatorLoop( std::vector<std::string> );
void applicationLoop( std::vector<std::string>, int );
void runProcess( int, int );
void runInput( std::string, int, int );
void runOutput( std::string, int, int );
void writeLog( std::string );
void setElapsedTime();
void printConfig();

//
// GLOBAL VARIABLES ////////////////////////////////////////////////////////////
// 

// Global program clock as well as total running time
std::chrono::time_point<std::chrono::system_clock> startTime;
float duration;

// Global output variables for logging
std::stringstream logOutput;
std::ofstream outputFile;

/**
 * @brief      Contains the parsed information from a proper .cnf file.
 */
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

/**
 * @brief      Index definitions of where each element comes within the meta-
 * data.
 */
enum META_DATA
{
    COMPONENT_LETTER = 0,
    OPERATION = 1,
    CYCLE_TIME = 2
};

/**
 * @brief      Main program loop.
 * @details    This function will start the program clock, ensure proper user 
 * execution, as well as parse loaded files before starting the main simulation.
 *
 * @param[in]  argc  The number of command line arguments.
 * @param      argv  The command line arguments, the second of which should be
 * the .cnf file path.
 *
 * @return     EXIT_SUCESS if the program executed without error, EXIT_FAILURE
 * otherwise.
 */
int main( int argc, char** argv )
{    
    // Start the global clock
    startTime = std::chrono::system_clock::now();

    // Set the global string buffer to print out all 6 decimal places
    logOutput.precision( 6 );
    logOutput.setf( std::ios::fixed );

    if( argc == 2 )
    {
        // Load & parse config file
        std::string configFile = loadData ( argv[1] );
        parseConfig( configFile );

        // Load & split meta data to make it easily parsable
        std::string data = loadData ( config.filePath );
        std::vector<std::string> operations = splitMetaData( data );
        
        simulatorLoop( operations );

        // Close the output log file
        if( outputFile.is_open() )
            outputFile.close();

        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "WARNING: Invalid number of arguments." << std::endl;
        return EXIT_FAILURE;
    }
}

//
// PARSING FUNCTIONS ///////////////////////////////////////////////////////////
// 

/**
 * @brief      Writes the contents of a text file to a string parameter.
 *
 * @param[in]  path  The path of the file to open.
 *
 * @return     A string with the exact contents of the text file.
 */
std::string loadData( std::string path )
{
    std::string temp;
    std::ifstream read;

    read.open( path.c_str() );
    if( read.is_open() )
    {
        // Push characters into the string
        while( !read.eof() )
            temp += read.get();
    }
    else
    {
        // Exit the program if the file is non-existent
        std::cout << "WARNING: File could not be opened." << std::endl;
        exit( EXIT_FAILURE );
    }
    read.close();
    return temp;
}

/**
 * @brief      Parses string data into the global Config struct.
 * @details    The general algorithm for parsing is as follows: Find the semi-
 * colon delimiter and make it the starting position, find the new-line
 * delimiter and make it the ending position, set the Config data to the
 * substring of the front and back positions, delete the line fron the string.
 * 
 * @param[in]  data  String data that was loaded from a .cnf file.
 */
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

/**
 * @brief      Splits a list of meta-data operations so that each operation may
 * be accessed in a vector index.
 *
 * @param[in]  data   String data that was loaded from a .mdf file.
 *
 * @return     A vector of single string operations in the format
 * "COMPONENT_LETTER(OPERATION)CYCLE_TIME".
 */
std::vector<std::string> splitMetaData( std::string data )
{
    std::vector<std::string> temp;
    std::string delimiter = "; ";
    
    size_t pos = 0;
    std::string token;

    // Remove the first line (contains "Start Program Meta-Data Code:")
    data.erase( 0, data.find( '\n' ) + 1 );
    // Remove all new line characters
    data.erase( std::remove( data.begin(), data.end(), '\n' ), data.end() );

    // Grab all the operations ending with ";"
    while( ( pos = data.find( delimiter ) ) != std::string::npos )
    {
        // Extract the operation
        token = data.substr( 0, pos );
        // Push it into the vector
        temp.push_back( token );
        //Remove the operation
        data.erase( 0, pos + delimiter.length() );
    }
    // Grab the last operation that ends with "." instead of a ";"
    pos = data.find( "." );
    token = data.substr( 0, pos );
    temp.push_back( token );

    return temp;
}

/**
 * @brief      Splits a single operation so that each piece may be accessed
 * from a vector index.
 *
 * @param[in]  data   An operation in the form: 
 * "COMPONENT_LETTER(OPERATION)CYCLE_TIME". An example is "I(hard drive)10".
 *
 * @return     A vector with component letter, operation, and cycle time in
 * their own respective indices.
 */
std::vector<std::string> splitOperation( std::string data )
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

    // <Operation>)<Cycle Time>
    // Get operation
    pos = data.find( delimiter );
    token = data.substr( 0, pos );
    temp.push_back( token );
    data.erase( 0, pos + delimiter.length() );

    // <Cycle Time>
    // Get cycle time
    temp.push_back( data );

    return temp;
}

//
// MAIN SIMULATION FUNCTIONS ///////////////////////////////////////////////////
// 

/**
 * @brief      Main loop for simulating every meta-data operation within a .mdf
 * file.
 * @details    Each application "A" has it's respective P/I/O operations
 * extracted, it is given an appID, and will then run in it's own 
 * applicationLoop.
 *
 * @param[in]  operations  The entire parsed meta-data contents from a .mdf
 * file.
 */
void simulatorLoop( std::vector<std::string> operations )
{
    // Set the start time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - <Simulator START>" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );

    //Reverse the vector so the operations may be popped in order
    std::reverse( operations.begin(), operations.end() );
    //Pop simulator start
    operations.pop_back();

    int appID = 0;

    // Execute every meta-data operation
    while( operations.size() != 0 )
    {
        std::vector<std::string> parsedOp = splitOperation( operations.back() );

        // An application label is found, so extract the contents so that it may
        // be executed within it's own loop
        if( parsedOp[ META_DATA::COMPONENT_LETTER ] == "A" && 
            parsedOp[ META_DATA::OPERATION ] == "start" )
        {
            // Set the start time and output it to the logs
            setElapsedTime();
            appID++;
            logOutput << duration << " - OS: PREPARING process " << appID 
                << std::endl;
            writeLog( logOutput.str() );
            logOutput.str( std::string() );

            // Extract the operations and place them into another vector
            std::vector<std::string> appOperations;
            operations.pop_back();
            parsedOp = splitOperation( operations.back() );
            while( parsedOp[ META_DATA::COMPONENT_LETTER ] != "A" && 
                   parsedOp[ META_DATA::OPERATION ] != "end" )
            {
                appOperations.push_back( operations.back() );
                operations.pop_back();
                parsedOp = splitOperation( operations.back() );
            }

            // Begin the application loop
            applicationLoop( appOperations, appID );
        }
        operations.pop_back();
    }

    // Set the end time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - <Simulator END>" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
}

/**
 * @brief      Executes each process "P", input "I", and output "O" linearly.
 * @details    Each I/O operation executes within its own thread.
 *
 * @param[in]  operations   A list of meta-data operatiosn that belong to this
 * application.
 * @param[in]  appID        The application "A" ID every operation belongs to.
 */
void applicationLoop( std::vector<std::string> operations, int appID )
{
    // Set the start time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - OS: START process " << appID << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );

    // Loop through the operations and "execute" each one by calling it's
    // respective function
    for( unsigned int numOperations = 0; numOperations < operations.size(); 
        numOperations++ )
    {
        std::vector<std::string> parsedOp = 
            splitOperation( operations[ numOperations ] );

        int cycleTime = std::stoi( parsedOp[ META_DATA::CYCLE_TIME ] );
        if( parsedOp[ META_DATA::COMPONENT_LETTER ] == "P" )
        {
            runProcess( cycleTime, appID );
        }
        else if( parsedOp[ META_DATA::COMPONENT_LETTER ] == "I" )
        {
            // Create a thread to run the Input operation
            std::thread inputThread( 
                runInput, parsedOp[ META_DATA::OPERATION ], cycleTime, appID );
            inputThread.join();
        }
        else if( parsedOp[ META_DATA::COMPONENT_LETTER ] == "O" )
        {
            // Create a thread to run the Output operation
            std::thread outputThread( 
                runOutput, parsedOp[ META_DATA::OPERATION ], cycleTime, appID );
            outputThread.join();
        }
    }

    // Set the end time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - OS: END process " << appID << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
}

/**
 * @brief      Executes a process "P" with a defined wait time.
 *
 * @param[in]  cycleTime  The time in miliseconds to simulate an operation by
 * sleeping the thread.
 * @param[in]  appID      The application "A" ID that is executing this process.
 */
void runProcess( int cycleTime, int appID )
{
    // Set the start time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - Process " << appID 
        << ": START processing action" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
            
    // Simulate this process with a thread sleep
    int sleepTime = cycleTime * config.processorCycle;
    std::this_thread::sleep_for( std::chrono::milliseconds( sleepTime ) );

    // Set the end time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - Process " << appID 
        << ": END processing action" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
}

/**
 * @brief      Executes an input operation "I" with a defined wait time and a
 * defined operation name.
 *
 * @note       This function is currently very similar to runInput, but is kept
 * separately in case future iterations of the simulation causes them to change
 * more distinctly.
 * 
 * @param[in]  name       The name of the operation. It may be "hard drive",
 * "keyboard", "monitor", or "printer".
 * @param[in]  cycleTime  The time in miliseconds to simulate an operation by
 * sleeping the thread.
 * @param[in]  appID      The application "A" ID that is executing this process.
 */
void runInput( std::string name, int cycleTime, int appID )
{
    // Set the start time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - Process " << appID << ": START " << 
        name << " input" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
            
    // Set the simulated execution time with a value from the config file
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

    // Set the end time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - Process " << appID << ": END " << 
        name << " input" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
}

/**
 * @brief      Executes an output operation "O" with a defined wait time and a
 * defined operation name.
 * 
 * @note       This function is currently very similar to runInput, but is kept
 * separately in case future iterations of the simulation causes them to change
 * more distinctly.
 *
 * @param[in]  name       The name of the operation. It may be "hard drive",
 * "keyboard", "monitor", or "printer".
 * @param[in]  cycleTime  The time in miliseconds to simulate an operation by
 * sleeping the thread.
 * @param[in]  appID      The application "A" ID that is executing this process.
 */
void runOutput( std::string name, int cycleTime, int appID )
{
    // Set the start time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - Process " << appID << ": START " << 
        name << " output" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
            
    // Set the simulated execution time with a value from the config file
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

    // Set the end time and output it to the logs
    setElapsedTime();
    logOutput << duration << " - Process " << appID << ": END " << 
        name << " output" << std::endl;
    writeLog( logOutput.str() );
    logOutput.str( std::string() );
}

//
// HELPER FUNCTIONS ////////////////////////////////////////////////////////////
// 

/**
 * @brief      Helper function to determine where to output the simulation.
 *
 * @param[in]  output  The string to output to the logs.
 */
void writeLog( std::string output )
{
    if( config.log == "Log to Both" )
    {
        std::cout << output;
        if( !outputFile.is_open() )
            outputFile.open( config.logFilePath );
        outputFile << output;
    }
    else if( config.log == "Log to Monitor" )
    {
        std::cout << output;
    }
    else if( config.log == "Log to File" )
    {
        if( !outputFile.is_open() )
            outputFile.open( config.logFilePath );
        outputFile << output;
    }
}

/**
 * @brief      Helper function to set the global counter duration to the elapsed
 * time.
 */
void setElapsedTime()
{
    duration = ( 
        (std::chrono::system_clock::now() - startTime).count() ) / 1000000000.0f;
}

/**
 * @brief      Helper function to check config file parsing.
 */
void printConfig()
{
    std::cout << "##   CONFIG FILE   ##" << std::endl;
    std::cout << config.version << std::endl;
    std::cout << config.filePath << std::endl;
    std::cout << config.processorCycle << std::endl;
    std::cout << config.monitorDisplayCycle << std::endl;
    std::cout << config.hardDriveCycle << std::endl;
    std::cout << config.printerCycle << std::endl;
    std::cout << config.keyboardCycle << std::endl;
    std::cout << config.log << std::endl;
    std::cout << config.logFilePath << std::endl;
    std::cout << "## END CONFIG FILE ##" << std::endl;
}