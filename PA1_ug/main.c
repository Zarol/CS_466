/** @file main.c
* @mainpage Operating Systems Project 1 - Family Tree
* @author REDACTED
* @version 1.00
* @brief This program simulates a family tree by using process creations and 
* system calls.
* @details An example file is shown below:
* \n A B 3 C D X
* \n D Y 2 M E
* \n M F 0
* \n C P 1 K
* \n The sample output would be:
* \n A-B
* \n     C-P
* \n         K
* \n     D-Y
* \n         M-F
* \n         E
* \n     X
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// Forward Declarations
int initDataFromFile( const char*, char[] );
int createFamilyTree( char[], int, int );
int findSpouse( char[], int );
void printDataArray( char[] );

/**
* Test driver for printing out the family tree of one file.
* @param argc The number of command line arguments
* @param argv[] The command line arguments, 
* the second of which should be the file name
* @return EXIT_SUCCESS if the program has succeeded, EXIT_FAILURE otherwise.
*/
int main( int argc, char* argv[] )
{
    char fileData[1000]; // Character array for file less than 1000 characters
    
    // Initialize the character array with a command-line file
    if( initDataFromFile( argv[1], fileData ) == EXIT_FAILURE )
        return EXIT_FAILURE;

{
    int tabCount = 0; // Stores the tab amounts for following generations
    int currentIndex = 0; // Stores the current index of the person

    // Print the first parent
    fprintf( stdout, "%c", fileData[ currentIndex ] );
    fprintf( stdout, "-" );
    // Print the second parent
    currentIndex++;
    fprintf( stdout, "%c", fileData[ currentIndex ] );
    fprintf( stdout, "\n" );

    // Set the index and tab count to prepare for the parent's children
    currentIndex = 2;
    tabCount++;

    // Create the family tree
    if( createFamilyTree( fileData, tabCount, currentIndex ) == EXIT_FAILURE )
    {
        fprintf( stderr, "Unable to create family tree.\n" );
        return EXIT_FAILURE;
    }
}

    // Successfully created the family tree!
    return EXIT_SUCCESS;
}

/**
* Stores the data from a file in a relative file location to a character array.
* @param fileName The name of the file within the directory of the executable.
* @param data[] The character array to be written to - data will be overwritten.
* @return EXIT_FAILURE if the file could not be opened, EXIT_SUCCESS otherwise.
*/
int initDataFromFile( const char* fileName, char data[] )
{
    // Declare a new file and open it
    FILE* readFile;
    readFile = fopen( fileName, "r" );

    // If the file could not be open / file not found
    if( readFile == NULL )
    {
        fprintf( stderr, "Unable to open file: %s\n", fileName );
        return EXIT_FAILURE;
    }
    // Write the data within the file to the character array, append null char
    else
    {
        // Store the current index pointer of the file
        int currentIndex = 0;
        // While not end of file
        while( !feof( readFile ) )
        {
            // Store each character into an element of the array
            fscanf( readFile, "%s", &data[ currentIndex ] );
            currentIndex++;
        }
        // Append the null character
        data[ currentIndex ] = '\0';
    }

    // Close the file and return success
    fclose( readFile );
    return EXIT_SUCCESS;
}

/**
* Creates the family tree.
* @param fileData[] The character data to parse the family tree from.
* @param tabCount The current tab count for formatting children.
* @param currentIndex The current person to print out.
* @return EXIT_FAILURE if there is an issue in the fork(), 
* EXIT_SUCCESS otherwise.
*/
int createFamilyTree( char fileData[], int tabCount, int currentIndex )
{
    // Set the number of children currently
    int numChild = ( fileData[ currentIndex ] - '0' );
    currentIndex++;

    // Loop through all of the children
    int iCount = 0;
    for( iCount = 0; iCount < numChild; iCount++ )
    {
        // Check if the current person's spouse exists
        int spouseLocation = findSpouse( fileData, currentIndex );
        // Spouse exists
        if( spouseLocation > -1 )
        {
            // Create a new process to handle more children
            pid_t pid = fork();

            // CHILD FORK OPERATIONS
            if( pid == 0 )
            {
                // Print out the required tabs for this generation
                for( iCount = 0; iCount < tabCount; iCount++ )
                    fprintf( stdout, "\t" );

                // Print out the couple
                fprintf( stdout, "%c", fileData[ currentIndex ] );
                fprintf( stdout, "-" );
                fprintf( stdout, "%c", fileData[ spouseLocation ] );
                fprintf( stdout, "\n" );

                // Create the couple's family tree
                createFamilyTree( fileData, tabCount + 1, spouseLocation + 1 );

                return EXIT_SUCCESS;
            }
            // PARENT FORK OPERATIONS
            else if( pid > 0 )
            {
                // Wait until child operations are done
                waitpid( pid, NULL, 0 );
            }
            else
            {
                // Something went wrong
                fprintf( stderr, "Fork Error: ID less than zero.\n");
                return EXIT_FAILURE;
            }
        }
        // Spouse does not exist
        else
        {
            // Print out the required tabs for this generation
            int printCount = 0;
            for( printCount = 0; printCount < tabCount; printCount++ )
                fprintf( stdout, "\t" );

            // Print out the loner's name
            fprintf( stdout, "%c\n", fileData[ currentIndex ] );
        }

        // Check the next person
        currentIndex++;
    }

    // Family tree created!
    return EXIT_SUCCESS;
}

/**
* Finds the current person's spouse within the given data context.
* @param fileData[] The character array to search for the spouse.
* @param currentIndex The person whose spouse to look for.
* @return int The index location of the person's spouse.
*/
int findSpouse( char fileData[], int currentIndex )
{
    // Set the count to not be the same person
    int iCount = 0;
    // Search the entire character array
    while( fileData[ iCount ] != '\0' )
    {
        // If the spouse is found, return the index
        if( fileData[ iCount ] == fileData[ currentIndex ] && 
            iCount != currentIndex )
            return ( iCount + 1 );
        iCount++;
    }

    // Spouse not found
    return -1;
}

/**
* Helper function to print out character arrays.
* @param data[] The character array to print out.
*/
void printDataArray( char data[] )
{
    int currentIndex = 0;
    while( data[ currentIndex ] != '\0' )
    {
        fprintf( stdout, "%c", data[ currentIndex ] );
        currentIndex++;
    }
    fprintf( stdout, "\n" );
}