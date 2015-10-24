#include "parser.h"

/**
 * @brief     Writes the contents of a text file to a string parameter.
 * @details   This function will exit the program if the file cannot be opened.
 *
 * @param[in]  path  The path of the file to open.
 *
 * @return     A string with the exact contents of the text file.
 */
std::string Parser::loadData( std::string path )
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
 * @brief      Parses string data into a Config.
 *
 * @param[in]  data    String data that was loaded from a .cnf file.
 * @param      config  The Config to load the data into.
 */
void Parser::parseConfig( std::string data, Config& config )
{
    //Remove useless first line
    data.erase( 0, data.find( '\n' ) + 1 );

    for( int lineNumber = 0; lineNumber < 10; lineNumber++ )
    {
        size_t frontPos = data.find( ": " ) + 2;
        size_t backPos = data.find( '\n' );

        switch( lineNumber )
        {
            case 0:
                config.version = data.substr( frontPos, backPos - frontPos );
                break;
            case 1:
                config.filePath = data.substr( frontPos, backPos - frontPos );
                break;
            case 2:
                config.schedulingCode = data.substr( frontPos, backPos - frontPos );
                break;
            case 3:
                config.processorCycle = std::stoi( 
                    data.substr( frontPos, backPos - frontPos ));
                break;
            case 4:
                config.monitorDisplayCycle = std::stoi( 
                    data.substr( frontPos, backPos - frontPos ));
                break;
            case 5:
                config.hardDriveCycle = std::stoi( 
                    data.substr( frontPos, backPos - frontPos ));
                break;
            case 6:
                config.printerCycle = std::stoi( 
                    data.substr( frontPos, backPos - frontPos ));
                break;
            case 7:
                config.keyboardCycle = std::stoi( 
                    data.substr( frontPos, backPos - frontPos ));
                break;
            case 8:
                config.log = data.substr( frontPos, backPos - frontPos );
                break;
            case 9:
                config.logFilePath = data.substr( frontPos, backPos - frontPos );
                break;
            default:
                break;
        }

        data.erase( 0, backPos + 1 );
    }
}

/**
 * @brief    Splits a list of meta-data operations into a list.
 * @details  Meta-data operations take the form: 
 * "COMPONENT_LETTER(OPERATION)CYCLE_TIME".
 *
 * @param[in]  metadata  String data that was loaded from a .mdf file.
 *
 * @return     A list where each element contains a meta-data operation.
 */
std::list<std::string> Parser::splitMetaData( std::string metadata )
{
    // Remove the first line (contains "Start Program Meta-Data Code:")
    metadata.erase( 0, metadata.find( '\n' ) + 1 );
    // Remove all new line characters
    metadata.erase( 
        std::remove( metadata.begin(), metadata.end(), '\n' ), metadata.end() );

    size_t pos = 0;
    std::string token;
    std::list<std::string> tokens;
    std::string delimiter = "; ";
    // Grab all the operations ending with ";"
    while( ( pos = metadata.find( delimiter ) ) != std::string::npos )
    {
        // Extract the operation
        token = metadata.substr( 0, pos );
        // Push it into the vector
        tokens.push_back( token );
        //Remove the operation
        metadata.erase( 0, pos + delimiter.length() );
    }
    // Grab the last operation that ends with "." instead of a ";"
    pos = metadata.find( "." );
    token = metadata.substr( 0, pos );
    tokens.push_back( token );

    return tokens;
}

/**
 * @brief       Splits a single operation into a map.
 * details      The operation must take the form: 
 * "COMPONENT_LETTER(OPERATION)CYCLE_TIME". An example is "I(hard drive)10".
 *
 * @param[in]  data   An operation in the proper format.
 *
 * @return     A map containing the keys "Component", "Operation", "Cycle".
 */
std::map<std::string, std::string> Parser::splitOperation( 
    std::string operation )
{
    std::map<std::string, std::string> operationMap;
    std::string token;
    std::string delimiter = ")";
    
    size_t pos = 0;

    // <Component Letter>(<Operation>)<Cycle Time>
    // Get component letter
    pos = 1;
    token = operation.substr( 0, pos );
    operationMap["Component"] = token;
    operation.erase( 0, pos + 1 );

    // <Operation>)<Cycle Time>
    // Get operation
    pos = operation.find( delimiter );
    token = operation.substr( 0, pos );
    operationMap["Operation"] = token;
    operation.erase( 0, pos + delimiter.length() );

    // <Cycle Time>
    // Get cycle time
    operationMap["Cycle"] = operation;

    return operationMap;
}